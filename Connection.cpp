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

#include "Connections.h"

#include "DeviceEnumeration.h"

#include "BenchtopBrushless.h"
#include "BenchtopDCServo.h"
#include "BenchtopStepper.h"
#include "KCubeBrushless.h"
#include "KCubeDCServo.h"
#include "KCubeStepper.h"
#include "TCubeBrushless.h"
#include "TCubeDCServo.h"
#include "TCubeStepper.h"
#include "IntegratedStepper.h"
#include "VerticalStage.h"


std::shared_ptr<KinesisDeviceConnection> MakeConnection(std::string const& serialNo) {
    std::unique_ptr<KinesisDeviceAccess> access;
    switch (TypeIDOfSerialNo(serialNo)) {
    case TypeIDBenchtopBrushless:
        access = std::make_unique<BenchtopBrushlessAccess>(serialNo);
        break;

    case TypeIDBenchtopDCServo1Channel:
    case TypeIDBenchtopDCServo3Channel:
        access = std::make_unique<BenchtopDCServoAccess>(serialNo);
        break;

    case TypeIDBenchtopStepper1Channel:
    case TypeIDBenchtopStepper3Channel:
        access = std::make_unique<BenchtopStepperAccess>(serialNo);
        break;

    case TypeIDKCubeBrushless:
        access = std::make_unique<KCubeBrushlessAccess>(serialNo);
        break;

    case TypeIDKCubeDCServo:
        access = std::make_unique<KCubeDCServoAccess>(serialNo);
        break;

    case TypeIDKCubeStepper:
        access = std::make_unique<KCubeStepperAccess>(serialNo);
        break;

    case TypeIDTCubeBrushless:
        access = std::make_unique<TCubeBrushlessAccess>(serialNo);
        break;

    case TypeIDTCubeDCServo:
        access = std::make_unique<TCubeDCServoAccess>(serialNo);
        break;

    case TypeIDTCubeStepper:
        access = std::make_unique<TCubeStepperAccess>(serialNo);
        break;

    case TypeIDLabJack050:
    case TypeIDLabJack490:
    case TypeIDLongTravelStage:
    case TypeIDCageRotator:
        access = std::make_unique<IntegratedStepperAccess>(serialNo);
        break;

    case TypeIDVerticalStage:
        access = std::make_unique<VerticalStageAccess>(serialNo);
        break;

    default:
        return {};
    }

    return UniqueConnection(std::move(access));
}


std::unique_ptr<MotorDrive> MakeKinesisMotorDrive(
    std::shared_ptr<KinesisDeviceConnection> connection, short channel) {

    switch (TypeIDOfSerialNo(connection->SerialNo())) {
    case TypeIDBenchtopBrushless:
        return std::make_unique<BenchtopBrushless>(connection, channel);

    case TypeIDBenchtopDCServo1Channel:
    case TypeIDBenchtopDCServo3Channel:
        return std::make_unique<BenchtopDCServo>(connection, channel);

    case TypeIDBenchtopStepper1Channel:
    case TypeIDBenchtopStepper3Channel:
        return std::make_unique<BenchtopStepper>(connection, channel);

    case TypeIDKCubeBrushless:
        return std::make_unique<KCubeBrushless>(connection);

    case TypeIDKCubeDCServo:
        return std::make_unique<KCubeDCServo>(connection);

    case TypeIDKCubeStepper:
        return std::make_unique<KCubeStepper>(connection);

    case TypeIDTCubeBrushless:
        return std::make_unique<TCubeBrushless>(connection);

    case TypeIDTCubeDCServo:
        return std::make_unique<TCubeDCServo>(connection);

    case TypeIDTCubeStepper:
        return std::make_unique<TCubeStepper>(connection);

    case TypeIDLabJack050:
    case TypeIDLabJack490:
    case TypeIDLongTravelStage:
    case TypeIDCageRotator:
        return std::make_unique<IntegratedStepper>(connection);

    case TypeIDVerticalStage:
        return std::make_unique<VerticalStage>(connection);

    default:
        return {};
    }
}
