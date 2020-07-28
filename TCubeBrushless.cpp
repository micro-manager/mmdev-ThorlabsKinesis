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

#include "TCubeBrushless.h"

#include "DLLAccess.h"

#include "Thorlabs.MotionControl.TCube.BrushlessMotor.h"


static DLLAccess kinesisDll{ "Thorlabs.MotionControl.TCube.BrushlessMotor.dll" };


bool
TCubeBrushlessAccess::IsKinesisDriverAvailable() {
    return kinesisDll.IsValid();
}


short
TCubeBrushlessAccess::Kinesis_Open() {
    STATIC_DLL_FUNC(kinesisDll, BMC_Open, func);
    return func(CSerialNo());
}


short
TCubeBrushlessAccess::Kinesis_Close() {
    STATIC_DLL_FUNC(kinesisDll, BMC_Close, func);
    func(CSerialNo());
    return 0;
}


short
TCubeBrushless::Kinesis_RequestSettings() {
    STATIC_DLL_FUNC(kinesisDll, BMC_RequestSettings, func);
    return func(CSerialNo());
}


short
TCubeBrushless::Kinesis_RequestStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, BMC_RequestStatusBits, func);
    return func(CSerialNo());
}


bool
TCubeBrushless::Kinesis_StartPolling(int intervalMs) {
    STATIC_DLL_FUNC(kinesisDll, BMC_StartPolling, func);
    return func(CSerialNo(), intervalMs);
}


void
TCubeBrushless::Kinesis_StopPolling() {
    STATIC_DLL_FUNC(kinesisDll, BMC_StopPolling, func);
    func(CSerialNo());
}


short
TCubeBrushless::Kinesis_GetHardwareInfo(char* modelNo, DWORD sizeOfModelNo,
    WORD* type, WORD* numChannels, char* notes, DWORD sizeOfNotes,
    DWORD* firmwareVersion, WORD* hardwareVersion, WORD* modificationState) {

    STATIC_DLL_FUNC(kinesisDll, BMC_GetHardwareInfo, func);

    short shortNumChannels;

    short ret = func(CSerialNo(), modelNo, sizeOfModelNo,
        type, &shortNumChannels, notes, sizeOfNotes, firmwareVersion,
        hardwareVersion, modificationState);

    *numChannels = shortNumChannels;

    return ret;
}


DWORD
TCubeBrushless::Kinesis_GetStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, BMC_GetStatusBits, func);
    return func(CSerialNo());
}


short
TCubeBrushless::Kinesis_EnableChannel() {
    STATIC_DLL_FUNC(kinesisDll, BMC_EnableChannel, func);
    return func(CSerialNo());
}


short
TCubeBrushless::Kinesis_DisableChannel() {
    STATIC_DLL_FUNC(kinesisDll, BMC_DisableChannel, func);
    return func(CSerialNo());
}


int
TCubeBrushless::Kinesis_GetMotorTravelMode() {
    STATIC_DLL_FUNC(kinesisDll, BMC_GetMotorTravelMode, func);
    return func(CSerialNo());
}


short
TCubeBrushless::Kinesis_SetMotorTravelMode(int mode) {
    STATIC_DLL_FUNC(kinesisDll, BMC_SetMotorTravelMode, func);
    return func(CSerialNo(), static_cast<MOT_TravelModes>(mode));
}


short
TCubeBrushless::Kinesis_ResetRotationModes() {
    STATIC_DLL_FUNC(kinesisDll, BMC_ResetRotationModes, func);
    return func(CSerialNo());
}


short
TCubeBrushless::Kinesis_SetRotationModes(int mode, int direction) {
    STATIC_DLL_FUNC(kinesisDll, BMC_SetRotationModes, func);
    return func(CSerialNo(), static_cast<MOT_MovementModes>(mode),
        static_cast<MOT_MovementDirections>(direction));
}


short
TCubeBrushless::Kinesis_RequestPosition() {
    STATIC_DLL_FUNC(kinesisDll, BMC_RequestPosition, func);
    return func(CSerialNo());
}


int
TCubeBrushless::Kinesis_GetPosition() {
    STATIC_DLL_FUNC(kinesisDll, BMC_GetPosition, func);
    return func(CSerialNo());
}


long
TCubeBrushless::Kinesis_GetPositionCounter() {
    STATIC_DLL_FUNC(kinesisDll, BMC_GetPositionCounter, func);
    return func(CSerialNo());
}


short
TCubeBrushless::Kinesis_MoveToPosition(int index) {
    STATIC_DLL_FUNC(kinesisDll, BMC_MoveToPosition, func);
    return func(CSerialNo(), index);
}


bool
TCubeBrushless::Kinesis_CanHome() {
    STATIC_DLL_FUNC(kinesisDll, BMC_CanHome, func);
    return func(CSerialNo());
}


short
TCubeBrushless::Kinesis_Home() {
    STATIC_DLL_FUNC(kinesisDll, BMC_Home, func);
    return func(CSerialNo());
}


short
TCubeBrushless::Kinesis_GetRealValueFromDeviceUnit(int deviceUnits,
    double* realValue, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, BMC_GetRealValueFromDeviceUnit, func);
    return func(CSerialNo(), deviceUnits, realValue, unitType);
}


short
TCubeBrushless::Kinesis_GetDeviceUnitFromRealValue(double realValue,
    int* deviceUnits, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, BMC_GetDeviceUnitFromRealValue, func);
    return func(CSerialNo(), realValue, deviceUnits, unitType);
}


long
TCubeBrushless::Kinesis_GetEncoderCounter() {
    STATIC_DLL_FUNC(kinesisDll, BMC_GetEncoderCounter, func);
    return func(CSerialNo());
}
