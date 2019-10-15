// Thorlabs Kinesis device adapter for Micro-Manager
// Author: Mark A. Tsuchida
//
// Copyright 2019 The Board of Regents of the University of Wisconsin System
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "Connections.h"
#include "DeviceEnumeration.h"
#include "DeviceInstantiation.h"

#include "DeviceBase.h"

#include <sstream>

namespace {
    std::string const DEVICENAME_HUB = "ThorlabsKinesis";

    std::string const PROPERTY_ENABLE_SIMULATED = "EnableSimulatedDevices";

    std::string const PROPVALUE_YES = "Yes";
    std::string const PROPVALUE_NO = "No";


    int const ERR_KINESIS_DRIVER_NOT_FOUND = 99999;
    int const ERR_MULTIPLE_HUBS = 99998;
}


namespace {

    class KinesisHub final : public HubBase<KinesisHub> {
        std::vector<std::string> deviceSerialNos_;
        bool simulatorsEnabled_;

        // Only allow a single instance of hub to be initialized at a time.
        static bool lock_;
        bool lockHeld_;

    public:
        KinesisHub() :
            simulatorsEnabled_{ false },
            lockHeld_{ false }
        {
            CreateStringProperty(PROPERTY_ENABLE_SIMULATED.c_str(), "Yes",
                false, nullptr, true);
            AddAllowedValue(PROPERTY_ENABLE_SIMULATED.c_str(), PROPVALUE_YES.c_str());
            AddAllowedValue(PROPERTY_ENABLE_SIMULATED.c_str(), PROPVALUE_NO.c_str());

            SetErrorText(ERR_KINESIS_DRIVER_NOT_FOUND,
                "Cannot load the Thorlabs Kinesis DLLs. Make sure Kinesis is "
                "installed at the standard location");
            SetErrorText(ERR_MULTIPLE_HUBS, "Only one hub can be created");
        }

        int Initialize() override {
            if (lock_)
                return ERR_MULTIPLE_HUBS;

            if (!IsKinesisDriverAvailable())
                return ERR_KINESIS_DRIVER_NOT_FOUND;

            lock_ = true;
            lockHeld_ = true;

            char s[MM::MaxStrLength];
            GetProperty("EnableSimulatedDevices", s);
            if (s == PROPVALUE_YES) {
                EnableSimulatedDevices();
                simulatorsEnabled_ = true;
            }

            deviceSerialNos_ = EnumerateSerialNumbers();

            return DEVICE_OK;
        }

        int Shutdown() override {
            if (simulatorsEnabled_)
                DisableSimulatedDevices();
            if (lockHeld_)
                lock_ = false;
            return DEVICE_OK;
        }

        void GetName(char* name) const override {
            CDeviceUtils::CopyLimitedString(name, DEVICENAME_HUB.c_str());
        }

        bool Busy() override {
            return false;
        }

        int DetectInstalledDevices() override {
            ClearInstalledDevices();

            for (auto const& serialNo : deviceSerialNos_) {
                // We need to make a connection to the device in order to
                // determine the number of channels, and also to get the model
                // number.
                // TODO Currently the dummy device will reconnect when
                // GetName() is called. It might be good to prime the dummy
                // device with the connection to avoid that (some devices are
                // slow). This would mean passing the connection to
                // MakeDevice() et al.
                std::shared_ptr<KinesisDeviceConnection> connection = MakeConnection(serialNo);
                if (!connection) {
                    // Unsupported or (less likely) could not connect. If we
                    // can detect the device, create a placeholder to inform
                    // the user.
                    KinesisDeviceInfo info{ serialNo };
                    if (info.IsDevicePresent()) {
                        MM::Device* dummy = MakeUnsupportedDevice(serialNo);
                        if (dummy)
                            AddInstalledDevice(dummy);
                    }
                }
                else if (IsPotentiallyMultiChannel(serialNo)) {
                    short numChannels = connection->GetNumChannels();
                    for (short ch = 1; ch <= numChannels; ++ch) {
                        MM::Device* dummy = MakeDevice("", serialNo, ch, connection);
                        if (dummy)
                            AddInstalledDevice(dummy);
                    }
                }
                else {
                    MM::Device* dummy = MakeDevice("", serialNo, short(-1), connection);
                    if (dummy)
                        AddInstalledDevice(dummy);
                }
            }

            return DEVICE_OK;
        }
    };

    bool KinesisHub::lock_ = false;

} // Anonymous namespace


MODULE_API void InitializeModuleData() {
    RegisterDevice(DEVICENAME_HUB.c_str(), MM::HubDevice,
        "Thorlabs Kinesis Hub");
}


MODULE_API MM::Device* CreateDevice(char const* name) {
    if (!name)
        return nullptr;

    if (name == DEVICENAME_HUB)
        return new KinesisHub();

    // Device names are ModelNo_SerialNo or ModelNo_SerialNo-Channel.
    // ModelNo may be Error[N] or Unsupported.
    std::istringstream nameStream{ name };
    std::string modelNo, serialNo;
    if (std::getline(nameStream, modelNo, '_').good()) {
        std::getline(nameStream, serialNo, '-');
        if (nameStream.good()) {
            short channel;
            nameStream >> channel;
            if (channel < 1)
                return nullptr;
            return MakeDevice(name, serialNo, channel);
        }
        return MakeDevice(name, serialNo, short(-1));
    }

    return nullptr;
}


MODULE_API void DeleteDevice(MM::Device* device) {
    delete device;
}
