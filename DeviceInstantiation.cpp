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

#include "DeviceInstantiation.h"

#include "DeviceEnumeration.h"

#include "SingleAxisStage.h"
#include "UnsupportedDevice.h"


MM::Device* MakeDevice(std::string const& name, std::string const& serialNo,
    short channel, std::shared_ptr<KinesisDeviceConnection> connection) {
    if (!IsValidSerialNo(serialNo))
        return nullptr;

    switch (TypeIDOfSerialNo(serialNo)) {
    case TypeIDKCubeBrushless:
    case TypeIDKCubeDCServo:
    case TypeIDKCubeStepper:
    case TypeIDTCubeBrushless:
    case TypeIDTCubeDCServo:
    case TypeIDTCubeStepper:
    case TypeIDLabJack050:
    case TypeIDLabJack490:
    case TypeIDLongTravelStage:
    case TypeIDCageRotator:
    case TypeIDVerticalStage:
        return new SingleAxisStage{ name, serialNo, -1, connection };

    case TypeIDBenchtopBrushless:
    case TypeIDBenchtopDCServo1Channel:
    case TypeIDBenchtopDCServo3Channel:
    case TypeIDBenchtopStepper1Channel:
    case TypeIDBenchtopStepper3Channel:
        return new SingleAxisStage{ name, serialNo, channel, connection };

    default:
        // Unsupported device: create placeholder only for first channel if it
        // is multi-channel
        if (channel == 1 || channel < 0) {
            return new UnsupportedDevice{ serialNo };
        }
        else {
            return nullptr;
        }
    }
}


MM::Device* MakeUnsupportedDevice(std::string const& serialNo) {
    if (!IsValidSerialNo(serialNo))
        return nullptr;

    return new UnsupportedDevice{ serialNo };
}
