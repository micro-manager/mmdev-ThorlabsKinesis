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
#include <type_traits>
#include <utility>

#include <Windows.h>


// The Thorlabs Kinesis DLLs can be linked to statically (i.e. using the
// provided .lib files), but doing so causes issues because different DLLs
// define symbols of the same name: for example, the benchtop, K-Cube, and
// T-Cube brushless motor APIs all use the BMC_ prefix, but the functions are
// not equivalent. So we have no choice but to load the DLLs with LoadLibrary()
// and access their functions via GetProcAddress().
//
// Since we are dynamically loading anyway, we can find the DLLs in the Program
// Files folder, so that the user need not copy the DLLs to the Micro-Manager
// path.


// RAII object to load DLL, hard-coded to Kinesis install location.
class DLLAccess {
    HMODULE dll_{ nullptr };
    std::string const name_;

public:
    explicit DLLAccess(std::string const& name) :
        name_{ name }
    {
        // We defer actual call to LoadLibrary() until first use. This is
        // necessary because DLLAccess is intended to be used as static
        // variables, whose constructor gets called during DLL initialization.
        // LoadLibrary() doesn't work during DLL initialization.
    }

    ~DLLAccess() {
        Unload(dll_);
    }

    bool IsValid() {
        if (!dll_) {
            dll_ = Load(name_);
        }
        return dll_ != nullptr;
    }

    template<typename F>
    F* GetFunction(char const* func) {
        if (!IsValid())
            return nullptr;
        return reinterpret_cast<F*>(GetProcAddress(dll_, func));
    }

private:
    static HMODULE Load(std::string const& name);
    static void Unload(HMODULE h);
};


// Object to cache function pointer and wrap call to it.
// F = decltype of the function.
template<typename F>
class DLLFunc {
    F* const func_;

public:
    DLLFunc(DLLAccess& dll, char const* func) :
        func_{ dll.GetFunction<F>(func) }
    {}

    template<typename... Args>
    decltype(auto) operator()(Args&&... args) {
        return func_(std::forward<Args>(args)...);
    }
};


// Macro to define a local static variable that is initialized with the given
// function (prevents typos in function names!)
#define STATIC_DLL_FUNC(dll, func, name) \
static DLLFunc<decltype(func)> (name){ (dll), #func }
