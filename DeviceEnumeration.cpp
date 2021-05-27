// Thorlabs Kinesis device adapter for Micro-Manager
// Author: Mark A. Tsuchida
//
// Copyright 2019-2020 The Board of Regents of the University of Wisconsin
// System
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

#include "DeviceEnumeration.h"

#include "DLLAccess.h"

// In the main part of this device adapter we want device enumeration, but not
// control of each device type. In other words we need only the TLI_*
// functions and typedefs. These are defined (over and over) in all of the
// Thorlabs.MotionControl.*.h headers, so we arbitrarily choose one to include.
// Control of each device type (BMC_*, BDC_*, etc., functions) is done in
// separate compilation units because only one Thorlabs header can be included
// at a time.
#include <Thorlabs.MotionControl.Benchtop.BrushlessMotor.h>

#include <sstream>


static DLLAccess kinesisDll{ "Thorlabs.MotionControl.Benchtop.BrushlessMotor.dll" };


struct KinesisDeviceInfo::Impl {
    TLI_DeviceInfo info_;

    // TLI_GetDeviceInfo() writes beyond the end of the TLI_DeviceInfo struct
    // (observed as of Kinesis version 1.14.18). Actually, what appears to be
    // happening is that the Kinesis headers define TLI_DeviceInfo to be
    // #pragma pack(1), but TLI_GetDeviceInfo() fills in the values as if it
    // were default packing (natural alignment).
    //
    // - sizeof(TLI_DeviceInfo) == 93 according to header with pack(1)
    // - sizeof(TLI_DeviceInfo) == 100 if we were to assume default packing
    // Indeed, we observe that exactly 7 bytes past the end of the struct get
    // modified by TLI_GetDeviceInfo(), hence the 7 bytes of extra allowance
    // below.
    //
    // Of course the incorrect packing directive also means that the fields are
    // misaligned. Only the first 3 fields: typeID, description, and serialNo,
    // are correctly aligned. Fortunately, we do not need the rest of the
    // fields as the information they provide is largely redundant.
    //
    // Other structs that have a pack(1) directive in the headers are:
    // KIM_DriveOPParameters (!),
    // TIM_DriveOPParameters (!),
    // MOT_VelocityParameters (Modular Rack Stepper version only),
    // NT_HVComponent,
    // PZ_FeedbackLoopConstants (Modular Rack Piezo version only),
    // TC_LoopParameters
    // However, all but the ones marked with (!) will (by luck) have unaffected
    // layout as long as used as a scalar (array stride is a different story).
    // For the two DriveOPParameters structs, we can avoid the structs by using
    // the non-struct functions (e.g. KIM_GetDriveOPParameters).
    char extra_[7];

    Impl() {
        memset(&info_, 0, sizeof(info_) + sizeof(extra_)); // Defensive
    }
};


KinesisDeviceInfo::KinesisDeviceInfo(std::string const& serialNo) :
    impl_{ std::make_unique<Impl>() }
{
    STATIC_DLL_FUNC(kinesisDll, TLI_GetDeviceInfo, getDeviceInfoFunc);

    if (!getDeviceInfoFunc(serialNo.c_str(), &impl_->info_)) {
        // Device not present or not valid
        memset(&impl_->info_, 0, sizeof(impl_->info_));
    }
}


KinesisDeviceInfo::KinesisDeviceInfo(KinesisDeviceInfo const& other) :
    impl_{ std::make_unique<Impl>(*other.impl_) }
{
}


KinesisDeviceInfo&
KinesisDeviceInfo::operator=(KinesisDeviceInfo const& other) {
    if (this != &other) {
        impl_ = std::make_unique<Impl>(*other.impl_);
    }
    return *this;
}


KinesisDeviceInfo::KinesisDeviceInfo(KinesisDeviceInfo&& other) noexcept = default;


KinesisDeviceInfo&
KinesisDeviceInfo::operator=(KinesisDeviceInfo&& other) noexcept = default;


KinesisDeviceInfo::~KinesisDeviceInfo() = default;


bool
KinesisDeviceInfo::IsDevicePresent() const {
    return impl_->info_.serialNo[0] != '\0';
}


std::string
KinesisDeviceInfo::GetSerialNo() const {
    return impl_->info_.serialNo;
}


uint32_t
KinesisDeviceInfo::GetTypeID() const {
    return impl_->info_.typeID;
}


std::string
KinesisDeviceInfo::GetDescription() const {
    if (!IsDevicePresent())
        return "(device not available)";
    return impl_->info_.description;
}


bool IsKinesisDriverAvailable() {
    return kinesisDll.IsValid();
}


void EnableSimulatedDevices() {
    STATIC_DLL_FUNC(kinesisDll, TLI_InitializeSimulations, func);
    func();
}


void DisableSimulatedDevices() {
    STATIC_DLL_FUNC(kinesisDll, TLI_UninitializeSimulations, func);
    func();
}


std::vector<std::string> EnumerateSerialNumbers() {
    STATIC_DLL_FUNC(kinesisDll, TLI_BuildDeviceList, buildDeviceListFunc);
    STATIC_DLL_FUNC(kinesisDll, TLI_GetDeviceListSize, getDeviceListSizeFunc);
    STATIC_DLL_FUNC(kinesisDll, TLI_GetDeviceListExt, getDeviceListExtFunc);

    // TLI_BuildDeviceList() fails with error 16 (FT_NoDLLLoaded) if we don't
    // help it load this FTDI DLL in the Thorlabs/Kinesis directory.
    DLLAccess ftdiDll{ "ftd2xx.dll" };
    ftdiDll.IsValid(); // Trigger the lazy load

    short err;
    err = buildDeviceListFunc();
    if (err)
        return {};

    size_t deviceCount = getDeviceListSizeFunc();

    // Serial numbers are 8 digits each. Compute size needed for a
    // comma-separated list, plus some extra bytes.
    size_t buflen = 9 * deviceCount + 100;
    auto serialNoBuffer = std::make_unique<char[]>(buflen);

    // (Pass size minus one just to be safe.)
    err = getDeviceListExtFunc(serialNoBuffer.get(), static_cast<DWORD>(buflen - 1));
    if (err)
        return {};

    std::vector<std::string> ret;
    std::istringstream serialNoStream{ serialNoBuffer.get() };
    std::string serialNo;
    while (std::getline(serialNoStream, serialNo, ',').good())
        ret.push_back(serialNo);

    return ret;
}
