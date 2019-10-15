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

#include "BenchtopDCServo.h"

#include "DLLAccess.h"

#include "Thorlabs.MotionControl.Benchtop.DCServo.h"


static DLLAccess kinesisDll{ "Thorlabs.MotionControl.Benchtop.DCServo.dll" };


bool
BenchtopDCServoAccess::IsKinesisDriverAvailable() {
    return kinesisDll.IsValid();
}


short
BenchtopDCServoAccess::Kinesis_Open() {
    STATIC_DLL_FUNC(kinesisDll, BDC_Open, func);
    return func(CSerialNo());
}


short
BenchtopDCServoAccess::Kinesis_Close() {
    STATIC_DLL_FUNC(kinesisDll, BDC_Close, func);
    func(CSerialNo());
    return 0;
}


short
BenchtopDCServoAccess::Kinesis_GetNumChannels() {
    STATIC_DLL_FUNC(kinesisDll, BDC_GetNumChannels, func);
    return func(CSerialNo());
}


short
BenchtopDCServo::Kinesis_RequestSettings() {
    STATIC_DLL_FUNC(kinesisDll, BDC_RequestSettings, func);
    return func(CSerialNo(), Channel());
}


short
BenchtopDCServo::Kinesis_RequestStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, BDC_RequestStatusBits, func);
    return func(CSerialNo(), Channel());
}


bool
BenchtopDCServo::Kinesis_StartPolling(int intervalMs) {
    STATIC_DLL_FUNC(kinesisDll, BDC_StartPolling, func);
    return func(CSerialNo(), Channel(), intervalMs);
}


void
BenchtopDCServo::Kinesis_StopPolling() {
    STATIC_DLL_FUNC(kinesisDll, BDC_StopPolling, func);
    func(CSerialNo(), Channel());
}


short
BenchtopDCServo::Kinesis_GetHardwareInfo(char* modelNo, DWORD sizeOfModelNo,
    WORD* type, WORD* numChannels, char* notes, DWORD sizeOfNotes,
    DWORD* firmwareVersion, WORD* hardwareVersion, WORD* modificationState) {

    STATIC_DLL_FUNC(kinesisDll, BDC_GetHardwareInfo, func);
    return func(CSerialNo(), Channel(), modelNo, sizeOfModelNo,
        type, numChannels, notes, sizeOfNotes, firmwareVersion,
        hardwareVersion, modificationState);
}


DWORD
BenchtopDCServo::Kinesis_GetStatusBits() {
    STATIC_DLL_FUNC(kinesisDll, BDC_GetStatusBits, func);
    return func(CSerialNo(), Channel());
}


short
BenchtopDCServo::Kinesis_EnableChannel() {
    STATIC_DLL_FUNC(kinesisDll, BDC_EnableChannel, func);
    return func(CSerialNo(), Channel());
}


short
BenchtopDCServo::Kinesis_DisableChannel() {
    STATIC_DLL_FUNC(kinesisDll, BDC_DisableChannel, func);
    return func(CSerialNo(), Channel());
}


int
BenchtopDCServo::Kinesis_GetMotorTravelMode() {
    STATIC_DLL_FUNC(kinesisDll, BDC_GetMotorTravelMode, func);
    return func(CSerialNo(), Channel());
}


short
BenchtopDCServo::Kinesis_SetMotorTravelMode(int mode) {
    STATIC_DLL_FUNC(kinesisDll, BDC_SetMotorTravelMode, func);
    return func(CSerialNo(), Channel(), static_cast<MOT_TravelModes>(mode));
}


short
BenchtopDCServo::Kinesis_ResetRotationModes() {
    STATIC_DLL_FUNC(kinesisDll, BDC_ResetRotationModes, func);
    return func(CSerialNo(), Channel());
}


short
BenchtopDCServo::Kinesis_SetRotationModes(int mode, int direction) {
    STATIC_DLL_FUNC(kinesisDll, BDC_SetRotationModes, func);
    return func(CSerialNo(), Channel(), static_cast<MOT_MovementModes>(mode),
        static_cast<MOT_MovementDirections>(direction));
}


short
BenchtopDCServo::Kinesis_RequestPosition() {
    STATIC_DLL_FUNC(kinesisDll, BDC_RequestPosition, func);
    return func(CSerialNo(), Channel());
}


int
BenchtopDCServo::Kinesis_GetPosition() {
    STATIC_DLL_FUNC(kinesisDll, BDC_GetPosition, func);
    return func(CSerialNo(), Channel());
}


long
BenchtopDCServo::Kinesis_GetPositionCounter() {
    STATIC_DLL_FUNC(kinesisDll, BDC_GetPositionCounter, func);
    return func(CSerialNo(), Channel());
}


short
BenchtopDCServo::Kinesis_MoveToPosition(int index) {
    STATIC_DLL_FUNC(kinesisDll, BDC_MoveToPosition, func);
    return func(CSerialNo(), Channel(), index);
}


bool
BenchtopDCServo::Kinesis_CanHome() {
    STATIC_DLL_FUNC(kinesisDll, BDC_CanHome, func);
    return func(CSerialNo(), Channel());
}


short
BenchtopDCServo::Kinesis_Home() {
    STATIC_DLL_FUNC(kinesisDll, BDC_Home, func);
    return func(CSerialNo(), Channel());
}


short
BenchtopDCServo::Kinesis_GetRealValueFromDeviceUnit(int deviceUnits,
    double* realValue, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, BDC_GetRealValueFromDeviceUnit, func);
    return func(CSerialNo(), Channel(), deviceUnits, realValue, unitType);
}


short
BenchtopDCServo::Kinesis_GetDeviceUnitFromRealValue(double realValue,
    int* deviceUnits, int unitType) {

    STATIC_DLL_FUNC(kinesisDll, BDC_GetDeviceUnitFromRealValue, func);
    return func(CSerialNo(), Channel(), realValue, deviceUnits, unitType);
}


long
BenchtopDCServo::Kinesis_GetEncoderCounter() {
    STATIC_DLL_FUNC(kinesisDll, BDC_GetEncoderCounter, func);
    return func(CSerialNo(), Channel());
}
