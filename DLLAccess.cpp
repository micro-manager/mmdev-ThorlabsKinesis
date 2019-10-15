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

#include "DLLAccess.h"

#include <ShlObj_core.h> // For SHGetSpecialFolderPathA()


HMODULE
DLLAccess::Load(std::string const& name) {
    static std::string prefix;
    if (prefix.empty()) {
        char programFilesPath[MAX_PATH];
        BOOL ok = SHGetSpecialFolderPathA(nullptr, programFilesPath,
            CSIDL_PROGRAM_FILES, false);
        if (!ok)
            return nullptr;

        // Note: Forward slashes do NOT work with LoadLibrary()!

        prefix = programFilesPath;
        prefix += "\\Thorlabs\\Kinesis";
    }

    std::string dllPath = prefix + "\\" + name;

    // The Kinesis DLLs need to find their own dependencies, also in the
    // prefix directory. To allow this, we need the 'altered search path'.
    return LoadLibraryExA(dllPath.c_str(), nullptr,
        LOAD_WITH_ALTERED_SEARCH_PATH);
}


void
DLLAccess::Unload(HMODULE h) {
    if (h)
        FreeLibrary(h);
}
