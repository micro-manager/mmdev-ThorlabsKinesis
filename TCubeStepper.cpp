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

#include "TCubeStepper.h"

#include "DLLAccess.h"

#include "Thorlabs.MotionControl.TCube.StepperMotor.h"


static DLLAccess kinesisDll{ "Thorlabs.MotionControl.TCube.StepperMotor.dll" };


bool
TCubeStepperAccess::IsKinesisDriverAvailable() {
    return kinesisDll.IsValid();
}


short
TCubeStepperAccess::Kinesis_Open() {
    STATIC_DLL_FUNC(kinesisDll, SCC_Open, func);
    return func(CSerialNo());
}


short
TCubeStepperAccess::Kinesis_Close() {
    STATIC_DLL_FUNC(kinesisDll, SCC_Close, func);
    func(CSerialNo());
    return 0;
}


short
TCubeStepper::Kinesis_RequestSettings() {
    STATIC_DLL_FUNC(kinesisDll, SCC_RequestSettings, func);
    return func(CSerialNo());
}


short
TCubeStepper::Kinesis_RequestStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, SCC_RequestStatusBits, func);
    return func(CSerialNo());
}


bool
TCubeStepper::Kinesis_StartPolling(int intervalMs) {
    STATIC_DLL_FUNC(kinesisDll, SCC_StartPolling, func);
    return func(CSerialNo(), intervalMs);
}


void
TCubeStepper::Kinesis_StopPolling() {
    STATIC_DLL_FUNC(kinesisDll, SCC_StopPolling, func);
    func(CSerialNo());
}


short
TCubeStepper::Kinesis_GetHardwareInfo(char* modelNo, DWORD sizeOfModelNo,
    WORD* type, WORD* numChannels, char* notes, DWORD sizeOfNotes,
    DWORD* firmwareVersion, WORD* hardwareVersion, WORD* modificationState) {

    STATIC_DLL_FUNC(kinesisDll, SCC_GetHardwareInfo, func);
    return func(CSerialNo(), modelNo, sizeOfModelNo,
        type, numChannels, notes, sizeOfNotes, firmwareVersion,
        hardwareVersion, modificationState);
}


DWORD
TCubeStepper::Kinesis_GetStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, SCC_GetStatusBits, func);
    return func(CSerialNo());
}


short
TCubeStepper::Kinesis_EnableChannel() {
    STATIC_DLL_FUNC(kinesisDll, SCC_EnableChannel, func);
    return func(CSerialNo());
}


short
TCubeStepper::Kinesis_DisableChannel() {
    STATIC_DLL_FUNC(kinesisDll, SCC_DisableChannel, func);
    return func(CSerialNo());
}


int
TCubeStepper::Kinesis_GetMotorTravelMode() {
    STATIC_DLL_FUNC(kinesisDll, SCC_GetMotorTravelMode, func);
    return func(CSerialNo());
}


short
TCubeStepper::Kinesis_SetMotorTravelMode(int mode) {
    STATIC_DLL_FUNC(kinesisDll, SCC_SetMotorTravelMode, func);
    return func(CSerialNo(), static_cast<MOT_TravelModes>(mode));
}


short
TCubeStepper::Kinesis_ResetRotationModes() {
    STATIC_DLL_FUNC(kinesisDll, SCC_ResetRotationModes, func);
    return func(CSerialNo());
}


short
TCubeStepper::Kinesis_SetRotationModes(int mode, int direction) {
    STATIC_DLL_FUNC(kinesisDll, SCC_SetRotationModes, func);
    return func(CSerialNo(), static_cast<MOT_MovementModes>(mode),
        static_cast<MOT_MovementDirections>(direction));
}


short
TCubeStepper::Kinesis_RequestPosition() {
    STATIC_DLL_FUNC(kinesisDll, SCC_RequestPosition, func);
    return func(CSerialNo());
}


int
TCubeStepper::Kinesis_GetPosition() {
    STATIC_DLL_FUNC(kinesisDll, SCC_GetPosition, func);
    return func(CSerialNo());
}


long
TCubeStepper::Kinesis_GetPositionCounter() {
    STATIC_DLL_FUNC(kinesisDll, SCC_GetPositionCounter, func);
    return func(CSerialNo());
}


short
TCubeStepper::Kinesis_MoveToPosition(int index) {
    STATIC_DLL_FUNC(kinesisDll, SCC_MoveToPosition, func);
    return func(CSerialNo(), index);
}


bool
TCubeStepper::Kinesis_CanHome() {
    STATIC_DLL_FUNC(kinesisDll, SCC_CanHome, func);
    return func(CSerialNo());
}


short
TCubeStepper::Kinesis_Home() {
    STATIC_DLL_FUNC(kinesisDll, SCC_Home, func);
    return func(CSerialNo());
}


short
TCubeStepper::Kinesis_GetRealValueFromDeviceUnit(int deviceUnits,
    double* realValue, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, SCC_GetRealValueFromDeviceUnit, func);
    return func(CSerialNo(), deviceUnits, realValue, unitType);
}


short
TCubeStepper::Kinesis_GetDeviceUnitFromRealValue(double realValue,
    int* deviceUnits, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, SCC_GetDeviceUnitFromRealValue, func);
    return func(CSerialNo(), realValue, deviceUnits, unitType);
}
