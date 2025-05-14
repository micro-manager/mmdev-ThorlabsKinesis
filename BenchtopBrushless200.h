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

#include "KinesisDevice.h"


class BenchtopBrushless200Access final : public KinesisDeviceAccess {
public:
    explicit BenchtopBrushless200Access(std::string const& serialNo) :
        KinesisDeviceAccess{ serialNo }
    {}

protected:
    bool IsKinesisDriverAvailable() override;
    short Kinesis_Open() override;
    short Kinesis_Close() override;
    short Kinesis_GetNumChannels() override;
};


class BenchtopBrushless200 final : public NonStepperMotorDrive {
public:
    BenchtopBrushless200(std::shared_ptr<KinesisDeviceConnection> connection, short channel) :
        NonStepperMotorDrive{ connection, channel }
    {}

protected: // General
    short Kinesis_RequestSettings() override;
    short Kinesis_RequestStatusBits() override;
    bool Kinesis_StartPolling(int intervalMs) override;
    void Kinesis_StopPolling() override;

    short Kinesis_GetHardwareInfo(char* modelNo, DWORD sizeOfModelNo,
        WORD* type, WORD* numChannels, char* notes, DWORD sizeOfNotes,
        DWORD* firmwareVersion, WORD* hardwareVersion, WORD* modificationState)
        override;

    DWORD Kinesis_GetStatusBits() override;

protected: // Motor
    short Kinesis_EnableChannel() override;
    short Kinesis_DisableChannel() override;

    int Kinesis_GetMotorTravelMode() override;
    short Kinesis_SetMotorTravelMode(int mode) override;
    short Kinesis_ResetRotationModes() override;
    short Kinesis_SetRotationModes(int mode, int direction) override;

    short Kinesis_SetHomingParams(int direction, int limitSwitchMode, int offsetDistance, int velocity) override;
    short Kinesis_SetLimitSwitchParams(int, int, int, int, int) override { return -1; }

    short Kinesis_RequestPosition() override;
    int Kinesis_GetPosition() override;
    long Kinesis_GetPositionCounter() override;
    short Kinesis_MoveToPosition(int index) override;

    bool Kinesis_CanHome() override;
    short Kinesis_Home() override;

    short Kinesis_GetRealValueFromDeviceUnit(int deviceUnits,
        double* realValue, int unitType) override;
    short Kinesis_GetDeviceUnitFromRealValue(double realValue,
        int* deviceUnits, int unitType) override;

protected: // Non-stepper
    long Kinesis_GetEncoderCounter() override;
};
