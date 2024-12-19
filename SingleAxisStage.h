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

#include "DeviceBase.h"

#include <memory>


class SingleAxisStage final : public CStageBase<SingleAxisStage> {
    std::string const serialNo_;
    short const channel_;

    // The device name, if created via CreateDevice() (empty if created via
    // DetectInstalledDevices())
    std::string const givenName_;

    // Connection that is optionally passed in to constructor, so that existing
    // connection is kept instead of creating a new one when needed.
    std::shared_ptr<KinesisDeviceConnection> retainedConnection_;

    // Set during Initialize():
    std::unique_ptr<MotorDrive> motorDrive_;
    std::string selectedStageName_ = "CUSTOM";
    bool supportsStageSelection_{ false };
    bool supportsAutoDetection_{ false };
    bool isRotational_{ false };
    double deviceUnitsPerUm_{ 1.0 }; // Per degree if rotational
    double motorPitch_{ 1.0 };
    double motorGearboxRatio_{ 1.0 };
    double motorStepsPerRev_{1.0};
    int pollingIntervalMs_{ 200 };
    bool didEnable_{ false };

    // Dynamic state:
    MM::MMTime lastMovementStart_{ 0.0 };

    typedef struct MOT_HomingParameters
    {
        unsigned int direction;
        unsigned int limitSwitch;
        double velocity;
        double offsetDistance;
    } MOT_HomingParameters;

    typedef struct MOT_LimitSwitchParameters
    {
        unsigned int ccwHardwareLimitMode;
        unsigned int ccwSoftwareLimitPosition;
        unsigned int cwHardwareLimitMode;
        unsigned int cwSoftwareLimitPosition;
        unsigned int softwareLimitMode;
    } MOT_LimitSwitchParameters;

public:
    SingleAxisStage(std::string const& name, std::string const& serialNo,
        short channel, std::shared_ptr<KinesisDeviceConnection> connection);
    ~SingleAxisStage() override;

    int Initialize() override;
    int Shutdown() override;

    void GetName(char* name) const override;
    bool Busy() override;

    int GetPositionUm(double& pos) override;
    int SetPositionUm(double pos) override;
    int GetPositionSteps(long& steps) override;
    int SetPositionSteps(long steps) override;
    int SetOrigin() override { return DEVICE_UNSUPPORTED_COMMAND; }
    int GetLimits(double&, double&) override { return DEVICE_UNSUPPORTED_COMMAND; }
    int Home();

    int OnStageNameChange(MM::PropertyBase* pProp, MM::ActionType eAct);

    bool IsContinuousFocusDrive() const override { return false; }
    int IsStageSequenceable(bool& f) const override { f = false; return DEVICE_OK; }

private:
    std::unique_ptr<MotorDrive> Connect() const;
    std::string MakeName(MotorDrive* motorDrive) const;
};
