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

#include "KCubeDCServo.h"

#include "DLLAccess.h"

#include "Thorlabs.MotionControl.KCube.DCServo.h"


static DLLAccess kinesisDll{ "Thorlabs.MotionControl.KCube.DCServo.dll" };


bool
KCubeDCServoAccess::IsKinesisDriverAvailable() {
    return kinesisDll.IsValid();
}


short
KCubeDCServoAccess::Kinesis_Open() {
    STATIC_DLL_FUNC(kinesisDll, CC_Open, func);
    return func(CSerialNo());
}


short
KCubeDCServoAccess::Kinesis_Close() {
    STATIC_DLL_FUNC(kinesisDll, CC_Close, func);
    func(CSerialNo());
    return 0;
}


short
KCubeDCServo::Kinesis_RequestSettings() {
    STATIC_DLL_FUNC(kinesisDll, CC_RequestSettings, func);
    return func(CSerialNo());
}


short
KCubeDCServo::Kinesis_RequestStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, CC_RequestStatusBits, func);
    return func(CSerialNo());
}


bool
KCubeDCServo::Kinesis_StartPolling(int intervalMs) {
    STATIC_DLL_FUNC(kinesisDll, CC_StartPolling, func);
    return func(CSerialNo(), intervalMs);
}


void
KCubeDCServo::Kinesis_StopPolling() {
    STATIC_DLL_FUNC(kinesisDll, CC_StopPolling, func);
    func(CSerialNo());
}


short
KCubeDCServo::Kinesis_GetHardwareInfo(char* modelNo, DWORD sizeOfModelNo,
    WORD* type, WORD* numChannels, char* notes, DWORD sizeOfNotes,
    DWORD* firmwareVersion, WORD* hardwareVersion, WORD* modificationState) {

    STATIC_DLL_FUNC(kinesisDll, CC_GetHardwareInfo, func);
    return func(CSerialNo(), modelNo, sizeOfModelNo,
        type, numChannels, notes, sizeOfNotes, firmwareVersion,
        hardwareVersion, modificationState);
}


DWORD
KCubeDCServo::Kinesis_GetStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, CC_GetStatusBits, func);
    return func(CSerialNo());
}


short
KCubeDCServo::Kinesis_EnableChannel() {
    STATIC_DLL_FUNC(kinesisDll, CC_EnableChannel, func);
    return func(CSerialNo());
}


short
KCubeDCServo::Kinesis_DisableChannel() {
    STATIC_DLL_FUNC(kinesisDll, CC_DisableChannel, func);
    return func(CSerialNo());
}


int
KCubeDCServo::Kinesis_GetMotorTravelMode() {
    STATIC_DLL_FUNC(kinesisDll, CC_GetMotorTravelMode, func);
    return func(CSerialNo());
}


short
KCubeDCServo::Kinesis_SetMotorTravelMode(int mode) {
    STATIC_DLL_FUNC(kinesisDll, CC_SetMotorTravelMode, func);
    return func(CSerialNo(), static_cast<MOT_TravelModes>(mode));
}


short
KCubeDCServo::Kinesis_ResetRotationModes() {
    STATIC_DLL_FUNC(kinesisDll, CC_ResetRotationModes, func);
    return func(CSerialNo());
}


short
KCubeDCServo::Kinesis_SetRotationModes(int mode, int direction) {
    STATIC_DLL_FUNC(kinesisDll, CC_SetRotationModes, func);
    return func(CSerialNo(), static_cast<MOT_MovementModes>(mode),
        static_cast<MOT_MovementDirections>(direction));
}

short
KCubeDCServo::Kinesis_SetHomingParams(int direction, int limitSwitchMode, int offsetDistance, int velocity)
{
    STATIC_DLL_FUNC(kinesisDll, CC_SetHomingParamsBlock, func);
    MOT_HomingParameters params;
    params.direction = (MOT_TravelDirection)direction;
    params.limitSwitch = (MOT_HomeLimitSwitchDirection)limitSwitchMode;
    params.offsetDistance = offsetDistance;
    params.velocity = velocity;

    return func(CSerialNo(), &params);
}

short
KCubeDCServo::Kinesis_SetLimitSwitchParams(int ccwHardwareLimitMode, int ccwSoftwareLimitPosition,
    int cwHardwareLimitMode, int cwSoftwareLimitPosition, int softwareLimitMode)
{
    STATIC_DLL_FUNC(kinesisDll, CC_SetLimitSwitchParams, func);
    return func(CSerialNo(), (MOT_LimitSwitchModes)cwHardwareLimitMode, (MOT_LimitSwitchModes)ccwHardwareLimitMode, cwSoftwareLimitPosition, ccwSoftwareLimitPosition, (MOT_LimitSwitchSWModes)softwareLimitMode);
}

short
KCubeDCServo::Kinesis_RequestPosition() {
    STATIC_DLL_FUNC(kinesisDll, CC_RequestPosition, func);
    return func(CSerialNo());
}

int
KCubeDCServo::Kinesis_GetPosition() {
    STATIC_DLL_FUNC(kinesisDll, CC_GetPosition, func);
    return func(CSerialNo());
}


long
KCubeDCServo::Kinesis_GetPositionCounter() {
    STATIC_DLL_FUNC(kinesisDll, CC_GetPositionCounter, func);
    return func(CSerialNo());
}


short
KCubeDCServo::Kinesis_MoveToPosition(int index) {
    STATIC_DLL_FUNC(kinesisDll, CC_MoveToPosition, func);
    return func(CSerialNo(), index);
}


bool
KCubeDCServo::Kinesis_CanHome() {
    STATIC_DLL_FUNC(kinesisDll, CC_CanHome, func);
    return func(CSerialNo());
}


short
KCubeDCServo::Kinesis_Home() {
    STATIC_DLL_FUNC(kinesisDll, CC_Home, func);
    return func(CSerialNo());
}


short
KCubeDCServo::Kinesis_GetRealValueFromDeviceUnit(int deviceUnits,
    double* realValue, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, CC_GetRealValueFromDeviceUnit, func);
    return func(CSerialNo(), deviceUnits, realValue, unitType);
}


short
KCubeDCServo::Kinesis_GetDeviceUnitFromRealValue(double realValue,
    int* deviceUnits, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, CC_GetDeviceUnitFromRealValue, func);
    return func(CSerialNo(), realValue, deviceUnits, unitType);
}


long
KCubeDCServo::Kinesis_GetEncoderCounter() {
    STATIC_DLL_FUNC(kinesisDll, CC_GetEncoderCounter, func);
    return func(CSerialNo());
}
