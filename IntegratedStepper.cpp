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

#include "IntegratedStepper.h"

#include "DLLAccess.h"

#include "Thorlabs.MotionControl.IntegratedStepperMotors.h"


static DLLAccess kinesisDll{ "Thorlabs.MotionControl.IntegratedStepperMotors.dll" };


bool
IntegratedStepperAccess::IsKinesisDriverAvailable() {
    return kinesisDll.IsValid();
}


short
IntegratedStepperAccess::Kinesis_Open() {
    STATIC_DLL_FUNC(kinesisDll, ISC_Open, func);
    return func(CSerialNo());
}


short
IntegratedStepperAccess::Kinesis_Close() {
    STATIC_DLL_FUNC(kinesisDll, ISC_Close, func);
    func(CSerialNo());
    return 0;
}


short
IntegratedStepper::Kinesis_RequestSettings() {
    STATIC_DLL_FUNC(kinesisDll, ISC_RequestSettings, func);
    return func(CSerialNo());
}


short
IntegratedStepper::Kinesis_RequestStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, ISC_RequestStatusBits, func);
    return func(CSerialNo());
}


bool
IntegratedStepper::Kinesis_StartPolling(int intervalMs) {
    STATIC_DLL_FUNC(kinesisDll, ISC_StartPolling, func);
    return func(CSerialNo(), intervalMs);
}


void
IntegratedStepper::Kinesis_StopPolling() {
    STATIC_DLL_FUNC(kinesisDll, ISC_StopPolling, func);
    func(CSerialNo());
}


short
IntegratedStepper::Kinesis_GetHardwareInfo(char* modelNo, DWORD sizeOfModelNo,
    WORD* type, WORD* numChannels, char* notes, DWORD sizeOfNotes,
    DWORD* firmwareVersion, WORD* hardwareVersion, WORD* modificationState) {

    STATIC_DLL_FUNC(kinesisDll, ISC_GetHardwareInfo, func);
    return func(CSerialNo(), modelNo, sizeOfModelNo,
        type, numChannels, notes, sizeOfNotes, firmwareVersion,
        hardwareVersion, modificationState);
}


DWORD
IntegratedStepper::Kinesis_GetStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, ISC_GetStatusBits, func);
    return func(CSerialNo());
}


short
IntegratedStepper::Kinesis_EnableChannel() {
    STATIC_DLL_FUNC(kinesisDll, ISC_EnableChannel, func);
    return func(CSerialNo());
}


short
IntegratedStepper::Kinesis_DisableChannel() {
    STATIC_DLL_FUNC(kinesisDll, ISC_DisableChannel, func);
    return func(CSerialNo());
}


int
IntegratedStepper::Kinesis_GetMotorTravelMode() {
    STATIC_DLL_FUNC(kinesisDll, ISC_GetMotorTravelMode, func);
    return func(CSerialNo());
}


short
IntegratedStepper::Kinesis_SetMotorTravelMode(int mode) {
    STATIC_DLL_FUNC(kinesisDll, ISC_SetMotorTravelMode, func);
    return func(CSerialNo(), static_cast<MOT_TravelModes>(mode));
}


short
IntegratedStepper::Kinesis_ResetRotationModes() {
    STATIC_DLL_FUNC(kinesisDll, ISC_ResetRotationModes, func);
    return func(CSerialNo());
}


short
IntegratedStepper::Kinesis_SetRotationModes(int mode, int direction) {
    STATIC_DLL_FUNC(kinesisDll, ISC_SetRotationModes, func);
    return func(CSerialNo(), static_cast<MOT_MovementModes>(mode),
        static_cast<MOT_MovementDirections>(direction));
}


short
IntegratedStepper::Kinesis_RequestPosition() {
    STATIC_DLL_FUNC(kinesisDll, ISC_RequestPosition, func);
    return func(CSerialNo());
}


int
IntegratedStepper::Kinesis_GetPosition() {
    STATIC_DLL_FUNC(kinesisDll, ISC_GetPosition, func);
    return func(CSerialNo());
}


long
IntegratedStepper::Kinesis_GetPositionCounter() {
    STATIC_DLL_FUNC(kinesisDll, ISC_GetPositionCounter, func);
    return func(CSerialNo());
}


short
IntegratedStepper::Kinesis_MoveToPosition(int index) {
    STATIC_DLL_FUNC(kinesisDll, ISC_MoveToPosition, func);
    return func(CSerialNo(), index);
}


bool
IntegratedStepper::Kinesis_CanHome() {
    STATIC_DLL_FUNC(kinesisDll, ISC_CanHome, func);
    return func(CSerialNo());
}


short
IntegratedStepper::Kinesis_Home() {
    STATIC_DLL_FUNC(kinesisDll, ISC_Home, func);
    return func(CSerialNo());
}


short
IntegratedStepper::Kinesis_GetRealValueFromDeviceUnit(int deviceUnits,
    double* realValue, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, ISC_GetRealValueFromDeviceUnit, func);
    return func(CSerialNo(), deviceUnits, realValue, unitType);
}


short
IntegratedStepper::Kinesis_GetDeviceUnitFromRealValue(double realValue,
    int* deviceUnits, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, ISC_GetDeviceUnitFromRealValue, func);
    return func(CSerialNo(), realValue, deviceUnits, unitType);
}
