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

#pragma once

#include <string>
#include <memory>

#include <Windows.h>


// Abstraction layer for Kinesis API. These classes wrap the Kinesis C API so
// that different devices (e.g. benchtop DC servo vs K-Cube stepper motor) can
// be controled through a common interface. The root class is KinesisDevice.
//
// While the Kinesis API functions are (nearly) analogous across different
// device categories, one major difference is that some devices (e.g. benchtop
// motor controllers) are multi-channel. Most API functions for multi-channel
// devices take a channel parameter, except for Open and Close, which need to
// be called once per multi-channel device. So we use a separate RAII class to
// open the device. These "connection" objects should be managed with
// shared_ptr. They form a class hierarchy just like KinesisDeivce.

// Common base class for KinesisDeviceConnection and KinesisDevice
class SerialNumbered {
    std::string const serialNo_;

public:
    virtual ~SerialNumbered() = default;

    // Noncopyable
    SerialNumbered(SerialNumbered const&) = delete;
    SerialNumbered& operator=(SerialNumbered const&) = delete;

    std::string SerialNo() const { return serialNo_; }

protected: // Functions available to subclass implementations
    explicit SerialNumbered(std::string const& serialNo) :
        serialNo_{ serialNo }
    {}

    // Returned pointer is valid while this object is valid
    char const* CSerialNo() const { return serialNo_.c_str(); }
};


class KinesisDeviceAccess : public SerialNumbered {
public:
    explicit KinesisDeviceAccess(std::string const& serialNo) :
        SerialNumbered{ serialNo }
    {}

private:
    friend class KinesisDeviceConnection;
    short Open() {
        if (!IsKinesisDriverAvailable())
            return short(-1);
        return Kinesis_Open();
    }
    short Close() { return Kinesis_Close(); }
    short GetNumChannels() { return Kinesis_GetNumChannels(); }

protected: // Functions to be implemented by derived classes
    virtual bool IsKinesisDriverAvailable() = 0;
    virtual short Kinesis_Open() = 0;
    virtual short Kinesis_Close() = 0;

    // Should be overridden only for multi-channel device types
    virtual short Kinesis_GetNumChannels() {
        return 0; // Not a multi-channel device
    }
};


// RAII object for connection (open/close); we manage these with shared_ptr
class KinesisDeviceConnection {
    std::unique_ptr<KinesisDeviceAccess> access_;
    short const connectionError_;

public:
    explicit KinesisDeviceConnection(std::unique_ptr<KinesisDeviceAccess> access) :
        access_{ std::move(access) },
        connectionError_{ access_->Open() }
    {}

    virtual ~KinesisDeviceConnection() {
        if (connectionError_ == 0)
            access_->Close();
    }

    std::string SerialNo() const {
        return access_->SerialNo();
    }

    short GetNumChannels() {
        return access_->GetNumChannels();
    }

    bool IsValid() const {
        return connectionError_ == 0;
    }

    short ConnectionError() const {
        return connectionError_;
    }
};


class KinesisDevice : public SerialNumbered {
    short const channel_;
    std::shared_ptr<KinesisDeviceConnection> const connection_;

public:
    explicit KinesisDevice(std::shared_ptr<KinesisDeviceConnection> connection) :
        KinesisDevice{ connection, -1 }
    {}

    KinesisDevice(std::shared_ptr<KinesisDeviceConnection> connection, short channel) :
        SerialNumbered{ connection->SerialNo() },
        channel_{ channel },
        connection_{ connection }
    {}

    std::shared_ptr<KinesisDeviceConnection> GetConnection() { return connection_; }

    // Return channel, or -1 if device is not multi-channel
    short Channel() const { return channel_; }

    short RequestSettings() { return Kinesis_RequestSettings(); }
    short RequestStatusBits() { return Kinesis_RequestStatusBits(); }
    bool StartPolling(int intervalMs) { return Kinesis_StartPolling(intervalMs); }
    void StopPolling() { Kinesis_StopPolling(); }

    std::string GetModelNo();

    DWORD GetStatusBits() { return Kinesis_GetStatusBits(); }

protected: // 1:1 wrappers for Kinesis API functions
    virtual short Kinesis_RequestSettings() = 0;
    virtual short Kinesis_RequestStatusBits() = 0;
    virtual bool Kinesis_StartPolling(int intervalMs) = 0;
    virtual void Kinesis_StopPolling() = 0;

    virtual short Kinesis_GetHardwareInfo(char* modelNo, DWORD sizeOfModelNo,
        WORD* type, WORD* numChannels, char* notes, DWORD sizeOfNotes,
        DWORD* firmwareVersion, WORD* hardwareVersion, WORD* modificationState) = 0;

    virtual DWORD Kinesis_GetStatusBits() = 0;
};


class MotorDrive : public KinesisDevice {
public:
    explicit MotorDrive(std::shared_ptr<KinesisDeviceConnection> connection) :
        KinesisDevice{ connection }
    {}

    MotorDrive(std::shared_ptr<KinesisDeviceConnection> connection, short channel) :
        KinesisDevice{ connection, channel }
    {}

    // Below are wrapped Kinesis functions. Several are kept even though they
    // do not work (see comments below), in case they get fixed in the future.

    // Note: I have checked that the definitions of the enum constants match
    // exactly between all motor API headers. Please make sure to do the same
    // if you add any new enum types. (There are buggy examples such as
    // MOT_TravelDirection and MOT_DirectionSense, whose integer values are
    // consistent but enum constant names are not!)

    enum StatusBits : DWORD {
        StatusBitsHardwareLimitCW = 0x1,
        StatusBitsHardwareLimitCCW = 0x2,
        StatusBitsSoftwareLimitCW = 0x4, // Not applicable to KVS vertical stage
        StatusBitsSoftwareLimitCCW = 0x8, // Not applicable to KVS vertical stage
        StatusBitsMovingCW = 0x10,
        StatusBitsMovingCCW = 0x20,
        StatusBitsJoggingCW = 0x40,
        StatusBitsJoggingCCW = 0x80,
        StatusBitsMotorConnected = 0x100, // Not applicable to KVS vertical stage
        StatusBitsHoming = 0x200,
        StatusBitsHomed = 0x400,
        // There are several more for digital input etc.
        StatusBitsChannelEnabled = 0x80000000,
    };

    // This function appears to be meaningless: although it does change the
    // "channel enabled" bit in the status bits, disabling a channel does not
    // actually prevent movement.
    short SetChannelEnabled(bool enabled) {
        return enabled ? Kinesis_EnableChannel() : Kinesis_DisableChannel();
    }

    enum TravelMode {
        TravelModeLinear = 1,
        TravelModeRotational = 2,
    };

    // This function is unusable: it seems to always return 1 (Linear) (Kinesis
    // 1.14.18).
    TravelMode GetMotorTravelMode() {
        switch (Kinesis_GetMotorTravelMode()) {
        case 1: return TravelModeLinear;
        case 2: return TravelModeRotational;
        default: return TravelModeLinear;
        }
    }

    // This function is unusable: it seems to always return error 1 (a few
    // devices tested; Kinesis 1.14.18).
    short SetMotorTravelMode(TravelMode mode) {
        return Kinesis_SetMotorTravelMode(mode);
    }

    enum RotationMode {
        RotationModeLinearRange = 0,
        RotationModeUnlimited = 1,
        RotationModeWrapping = 2,
    };
    enum RotationDirection {
        RotationDirectionQuickest = 0,
        RotationDirectionForward = 1,
        RotationDirectionReverse = 2,
    };
    short ResetRotationMode() { return Kinesis_ResetRotationModes(); }

    // This function is unusable: either it crashes, or causes subsequent moves
    // to crash (a few devices tested; Kinesis 1.14.18).
    short SetRotationMode(RotationMode mode, RotationDirection direction) {
        return Kinesis_SetRotationModes(mode, direction);
    }

    short RequestPosition() { return Kinesis_RequestPosition(); }
    int GetPosition() { return Kinesis_GetPosition(); }
    long GetPositionCounter() { return Kinesis_GetPositionCounter(); }
    short MoveToPosition(int index) { return Kinesis_MoveToPosition(index); }

    bool CanHome() { return Kinesis_CanHome(); }
    short Home() { return Kinesis_Home(); }

    // These conversion functions seem to always return an error (tested with
    // cage rotator K10CR1; Kinesis 1.14.18)
    short DeviceToPhysicalPosition(int deviceUnits, double& physicalUnits) {
        return Kinesis_GetRealValueFromDeviceUnit(deviceUnits, &physicalUnits, 0);
    }
    short PhysicalToDevicePosition(double physicalUnits, int& deviceUnits) {
        return Kinesis_GetDeviceUnitFromRealValue(physicalUnits, &deviceUnits, 0);
    }

protected:
    virtual short Kinesis_EnableChannel() = 0;
    virtual short Kinesis_DisableChannel() = 0;

    virtual int Kinesis_GetMotorTravelMode() = 0;
    virtual short Kinesis_SetMotorTravelMode(int mode) = 0;
    virtual short Kinesis_ResetRotationModes() = 0;
    virtual short Kinesis_SetRotationModes(int mode, int direction) = 0;

    virtual short Kinesis_RequestPosition() = 0;
    virtual int Kinesis_GetPosition() = 0;
    virtual long Kinesis_GetPositionCounter() = 0;
    virtual short Kinesis_MoveToPosition(int index) = 0;

    virtual bool Kinesis_CanHome() = 0;
    virtual short Kinesis_Home() = 0;

    virtual short Kinesis_GetRealValueFromDeviceUnit(int deviceUnits,
        double* realValue, int unitType) = 0;
    virtual short Kinesis_GetDeviceUnitFromRealValue(double realValue,
        int* deviceUnits, int unitType) = 0;
};


class NonStepperMotorDrive : public MotorDrive {
public:
    explicit NonStepperMotorDrive(std::shared_ptr<KinesisDeviceConnection> connection) :
        MotorDrive{ connection }
    {}

    NonStepperMotorDrive(std::shared_ptr<KinesisDeviceConnection> connection, short channel) :
        MotorDrive{ connection, channel }
    {}

    long GetEncoderCounter() { return Kinesis_GetEncoderCounter(); }

protected:
    virtual long Kinesis_GetEncoderCounter() = 0;
};
