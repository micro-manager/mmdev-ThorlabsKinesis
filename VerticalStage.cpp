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

#include "VerticalStage.h"

#include "DLLAccess.h"

#include "Thorlabs.MotionControl.VerticalStage.h"


static DLLAccess kinesisDll{ "Thorlabs.MotionControl.VerticalStage.dll" };


bool
VerticalStageAccess::IsKinesisDriverAvailable() {
    return kinesisDll.IsValid();
}


short
VerticalStageAccess::Kinesis_Open() {
    STATIC_DLL_FUNC(kinesisDll, KVS_Open, func);
    return func(CSerialNo());
}


short
VerticalStageAccess::Kinesis_Close() {
    STATIC_DLL_FUNC(kinesisDll, KVS_Close, func);
    func(CSerialNo());
    return 0;
}


short
VerticalStage::Kinesis_RequestSettings() {
    STATIC_DLL_FUNC(kinesisDll, KVS_RequestSettings, func);
    return func(CSerialNo());
}


short
VerticalStage::Kinesis_RequestStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, KVS_RequestStatusBits, func);
    return func(CSerialNo());
}


bool
VerticalStage::Kinesis_StartPolling(int intervalMs) {
    STATIC_DLL_FUNC(kinesisDll, KVS_StartPolling, func);
    return func(CSerialNo(), intervalMs);
}


void
VerticalStage::Kinesis_StopPolling() {
    STATIC_DLL_FUNC(kinesisDll, KVS_StopPolling, func);
    func(CSerialNo());
}


short
VerticalStage::Kinesis_GetHardwareInfo(char* modelNo, DWORD sizeOfModelNo,
    WORD* type, WORD* numChannels, char* notes, DWORD sizeOfNotes,
    DWORD* firmwareVersion, WORD* hardwareVersion, WORD* modificationState) {

    STATIC_DLL_FUNC(kinesisDll, KVS_GetHardwareInfo, func);
    return func(CSerialNo(), modelNo, sizeOfModelNo,
        type, numChannels, notes, sizeOfNotes, firmwareVersion,
        hardwareVersion, modificationState);
}


DWORD
VerticalStage::Kinesis_GetStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, KVS_GetStatusBits, func);
    return func(CSerialNo());
}


short
VerticalStage::Kinesis_EnableChannel() {
    STATIC_DLL_FUNC(kinesisDll, KVS_EnableChannel, func);
    return func(CSerialNo());
}


short
VerticalStage::Kinesis_DisableChannel() {
    STATIC_DLL_FUNC(kinesisDll, KVS_DisableChannel, func);
    return func(CSerialNo());
}


int
VerticalStage::Kinesis_GetMotorTravelMode() {
    STATIC_DLL_FUNC(kinesisDll, KVS_GetMotorTravelMode, func);
    return func(CSerialNo());
}


short
VerticalStage::Kinesis_SetMotorTravelMode(int mode) {
    STATIC_DLL_FUNC(kinesisDll, KVS_SetMotorTravelMode, func);
    return func(CSerialNo(), static_cast<MOT_TravelModes>(mode));
}


short
VerticalStage::Kinesis_ResetRotationModes() {
    return 18;
}


short
VerticalStage::Kinesis_SetRotationModes(int, int) {
    return 18;
}


short
VerticalStage::Kinesis_RequestPosition() {
    STATIC_DLL_FUNC(kinesisDll, KVS_RequestPosition, func);
    return func(CSerialNo());
}


int
VerticalStage::Kinesis_GetPosition() {
    STATIC_DLL_FUNC(kinesisDll, KVS_GetPosition, func);
    return func(CSerialNo());
}


long
VerticalStage::Kinesis_GetPositionCounter() {
    STATIC_DLL_FUNC(kinesisDll, KVS_GetPositionCounter, func);
    return func(CSerialNo());
}


short
VerticalStage::Kinesis_MoveToPosition(int index) {
    STATIC_DLL_FUNC(kinesisDll, KVS_MoveToPosition, func);
    return func(CSerialNo(), index);
}


bool
VerticalStage::Kinesis_CanHome() {
    STATIC_DLL_FUNC(kinesisDll, KVS_CanHome, func);
    return func(CSerialNo());
}


short
VerticalStage::Kinesis_Home() {
    STATIC_DLL_FUNC(kinesisDll, KVS_Home, func);
    return func(CSerialNo());
}


short
VerticalStage::Kinesis_GetRealValueFromDeviceUnit(int deviceUnits,
    double* realValue, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, KVS_GetRealValueFromDeviceUnit, func);
    return func(CSerialNo(), deviceUnits, realValue, unitType);
}


short
VerticalStage::Kinesis_GetDeviceUnitFromRealValue(double realValue,
    int* deviceUnits, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, KVS_GetDeviceUnitFromRealValue, func);
    return func(CSerialNo(), realValue, deviceUnits, unitType);
}


long
VerticalStage::Kinesis_GetEncoderCounter() {
    STATIC_DLL_FUNC(kinesisDll, KVS_GetEncoderCounter, func);
    return func(CSerialNo());
}
