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

#pragma once

#include "DeviceEnumeration.h"

#include "DeviceBase.h"


// A "device" that always fails to initialize, but provides an error message.
// By having these, we can show to the user that a particular device is
// correctly detected but not supported by this device adapter.
class UnsupportedDevice : public CGenericBase<UnsupportedDevice> {
    std::string const serialNo_;

    int const ERR_UNSUPPORTED_DEVICE = 99999;

public:
    UnsupportedDevice(std::string const& serialNo) :
        serialNo_{ serialNo }
    {
        SetErrorText(ERR_UNSUPPORTED_DEVICE,
            ("Device with serial number " + serialNo + " is not supported").c_str());
    }
    virtual ~UnsupportedDevice() {}

    virtual int Initialize() { return ERR_UNSUPPORTED_DEVICE; }
    virtual int Shutdown() { return DEVICE_OK; }

    virtual void GetName(char* name) const {
        std::string n{ "Unsupported_" + serialNo_ };
        CDeviceUtils::CopyLimitedString(name, n.c_str());
    }

    virtual bool Busy() { return false; }
};
