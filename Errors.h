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

#include <map>
#include <string>


int const ERR_OFFSET = 20000;


inline auto& KinesisErrorCodes() {
    static std::map<short, std::string> const codes = {
        // Error codes are straight from Thorlabs documentation

        // FTDI and communication errors
        { short(1), "The FTDI functions have not been initialized" },
        { short(2), "The device could not be found" },
        { short(3), "The device must be opened before it can be accessed" },
        { short(4), "An I/O error has occurred" },
        { short(5), "There are insufficient resources to run this application" },
        { short(6), "An invalid parameter has been supplied to the device" },
        { short(7), "The device is no longer present" },
        { short(8), "The device detected does not match that expected" },
        { short(16), "The library for this device could not be found" },
        { short(17), "No functions available for this device" },
        { short(18), "The function is not available for this device" },
        { short(19), "Bad function pointer detected" },
        { short(20), "The function failed to complete successfully" },
        { short(21), "The function failed to complete successfully" },

        // General DLL control errors
        { short(32), "Attempt to open a device that was already open" },
        { short(33), "The device has stopped responding" },
        { short(34), "This function has not been implemented" },
        { short(35), "The device has reported a fault" },
        { short(36), "The function could not be completed at this time" },
        { short(41), "The firmware has thrown an error" },
        { short(42), "The device has failed to initialize" },
        { short(43), "An invalid channel address was supplied" },

        // Motor specific errors
        { short(37), "The device cannot perform this function until it has been homed" },
        { short(38), "The function cannot be performed as it would result in an illegal position" },
        { short(39), "An invalid velocity parameter was supplied / The velocity must be greater than zero" },
        { short(44), "This device does not support homing / Check that the limit switch parameters are correct" },
        { short(45), "An invalid jog mode was supplied for the jog function" },
        { short(46), "There are no motor parameters available to convert real world units" },
        { short(47), "Command temporarily unavailable; device may be busy" },
    };
    return codes;
}
