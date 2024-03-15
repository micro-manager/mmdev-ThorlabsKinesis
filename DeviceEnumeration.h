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

#pragma once

#include <memory>
#include <string>
#include <vector>



// This is currently only useful for getting the "description" (see
// implementation comments for why).
class KinesisDeviceInfo {
    struct Impl;
    std::unique_ptr<Impl> impl_;

public:
    explicit KinesisDeviceInfo(std::string const& serialNo);
    KinesisDeviceInfo(KinesisDeviceInfo const& other);
    KinesisDeviceInfo& operator=(KinesisDeviceInfo const& other);
    KinesisDeviceInfo(KinesisDeviceInfo&& other) noexcept;
    KinesisDeviceInfo& operator=(KinesisDeviceInfo&& other) noexcept;
    ~KinesisDeviceInfo();
    bool IsDevicePresent() const;
    std::string GetSerialNo() const;
    uint32_t GetTypeID() const;
    std::string GetDescription() const;
};

inline bool IsValidSerialNo(std::string const& serialNo) {
    // 'serialNo' is an exactly 8-character string consisting of digits
    if (serialNo.size() > 9 || serialNo.size() < 8)
        return false;
    for (char d : serialNo) {
        if (d < '0' || d > '9')
            return false;
    }
    return true;
}

inline uint32_t TypeIDOfSerialNo(std::string const& serialNo) {
    if (!IsValidSerialNo(serialNo))
        return 0;
    if (serialNo.size() == 8)
    {
        return (serialNo[0] - '0') * 10 + (serialNo[1] - '0');
    }
    else
    {
        return (serialNo[0] - '0') * 100 + serialNo[0] - '0' * 10 + (serialNo[1] - '0');
    }
}

enum KinesisDeviceTypeID {
    // Sorted ascii-lexicographically.
    // Sources:
    //   'doc' - Kinesis C API documentation
    //   'net' - Kinesis .NET API documentation (only indicated if not in 'doc')
    //   'sim' - Kinesis Simulator (only indicated if not in 'doc' or 'net')
    TypeIDBenchtopBrushless200 = 73, // doc
    TypeIDBenchtopBrushless300 = 103, // doc
    TypeIDBenchtopDCServo1Channel = 43, // net
    TypeIDBenchtopDCServo3Channel = 79, // net
    TypeIDBenchtopNanotrak = 22, // doc
    TypeIDBenchtopPiezo1Channel = 41, // doc
    TypeIDBenchtopPiezo3Channel = 71, // doc
    TypeIDBenchtopPrecisionPiezo1Channel = 44, // net
    TypeIDBenchtopPrecisionPiezo2Channel = 95, // net
    TypeIDBenchtopStepper1Channel = 40, // doc
    TypeIDBenchtopStepper3Channel = 70, // doc
    TypeIDCageRotator = 55, // doc
    TypeIDFilterFlipper = 37, // doc
    TypeIDFilterWheel = 47, // doc
    TypeIDKCubeBrushless = 28, // doc
    TypeIDKCubeDCServo = 27, // doc
    TypeIDKCubeInertialMotor = 97, // doc
    TypeIDKCubeLaserDiode = 98, // net
    TypeIDKCubeLaserSource = 56, // doc
    TypeIDKCubeNanoTrak = 57, // doc
    TypeIDKCubePiezo = 29, // doc
    TypeIDKCubePositionAligner = 69, // net
    TypeIDKCubeSolenoid = 68, // doc
    TypeIDKCubeStepper = 26, // doc
    TypeIDKCubeStrainGauge = 59, // sim
    TypeIDLabJack050 = 49, // doc; includes 150 (sim)
    TypeIDLabJack490 = 46, // doc
    TypeIDLongTravelStage = 45, // doc
    TypeIDModularNanoTrak = 52, // doc
    TypeIDModularPiezo = 51, // doc
    TypeIDModularStepper = 50, // doc
    TypeIDPolarizer = 38, // doc
    TypeIDTCubeBrushless = 67, // doc
    TypeIDTCubeDCServo = 83, // doc
    TypeIDTCubeInertialMotor = 65, // doc
    TypeIDTCubeLaserDiode = 64, // doc
    TypeIDTCubeLaserSource = 86, // doc
    TypeIDTCubeNanoTrak = 82, // doc
    TypeIDTCubePiezo = 81, // sim
    TypeIDTCubeQuad = 89, // doc; aka position aligner
    TypeIDTCubeSolenoid = 85, // doc
    TypeIDTCubeStepper = 80, // doc
    TypeIDTCubeStrainGauge = 84, // doc
    TypeIDTCubeTEC = 87, // doc
    TypeIDVerticalStage = 24, // doc
};

// Return true if API is multi-channel (even if device is single-channel)
inline bool IsPotentiallyMultiChannel(std::string const& serialNo) {
    switch (TypeIDOfSerialNo(serialNo)) {
    case TypeIDBenchtopBrushless200:
    case TypeIDBenchtopBrushless300:
    case TypeIDBenchtopDCServo1Channel:
    case TypeIDBenchtopDCServo3Channel:
    case TypeIDBenchtopPiezo1Channel:
    case TypeIDBenchtopPiezo3Channel:
    case TypeIDBenchtopPrecisionPiezo2Channel: // Note only PPC2_, not PPC_
    case TypeIDBenchtopStepper1Channel:
    case TypeIDBenchtopStepper3Channel:
    case TypeIDModularPiezo:
    case TypeIDModularStepper:
        return true;
    default:
        return false;
    }
}


// If this returns false, all functions that access Kinesis will crash.
bool IsKinesisDriverAvailable();

// Must be called before EnumerateSerialNumbers() if using simulated devices
void EnableSimulatedDevices();

void DisableSimulatedDevices();

// Must be called before making connections to devices or getting info
std::vector<std::string> EnumerateSerialNumbers();
