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

#include <memory>
#include <string>
#include <unordered_map>


// Get the connection to the given device if one exists; otherwise make the
// connection using the given callback.
// This is not thread safe, but MMCore ensures synchronization for us.
inline std::shared_ptr<KinesisDeviceConnection> UniqueConnection(
    std::unique_ptr<KinesisDeviceAccess> access) {

    // Connections uniqued by serial number
    static std::unordered_map<std::string, std::weak_ptr<KinesisDeviceConnection>> connections;

    // Prune closed connections
    decltype(connections) pruned;
    for (auto&& item : connections) {
        if (!item.second.expired())
            pruned.emplace(std::move(item));
    }
    connections.swap(pruned);

    auto existing = connections.find(access->SerialNo());
    if (existing != connections.end()) {
        auto ret = existing->second.lock();
        if (ret)
            return ret;
    }

    auto newConn = std::make_shared<KinesisDeviceConnection>(std::move(access));
    connections[newConn->SerialNo()] = newConn;
    return newConn;
}


std::shared_ptr<KinesisDeviceConnection> MakeConnection(std::string const& serialNo);

std::unique_ptr<MotorDrive> MakeKinesisMotorDrive(
    std::shared_ptr<KinesisDeviceConnection> connection, short channel);
