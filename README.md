Micro-Manager Thorlabs Kinesis device adapter
=============================================

This module ought to be part of the mmCoreAndDevices repository, but is
currently provided separately because it requires C++14, which is not available
in the current mmCoreAndDevices build system. C++14 features are needed in order
to generate a reasonable wrapper around the Thorlabs Kinesis SDK.

TODO: Compatible Kinesis Software versions?


Building
--------

The device adapter is Windows-only and requires Visual Studio 2019 or later.

The Thorlabs Kinesis Software needs to be installed in its default location
(`C:\Program Files\Thorlabs\Kinesis`).

Place the mmCoreAndDevices repo at the same directory level as this repository
(thorlabs-kinesis-mm-adapter). The MMDevice headers and static library are
referenced using relative paths.

Before building this project, Micro-Manager's `MMDevice-SharedRuntime` project
should be built using the same Platform, Configuration, and compiler version
(this may require temporarily switching the Platform Toolset of the
`MMDevice-SharedRuntime` project).  Compiler versions must match because
MMDevice is a static library.

Building the ThorlabsKinesis project should produce
`mmgr_dal_ThorlabsKinesis.dll`.


Using
-----

The Thorlabs Kinesis Software needs to be installed. There is no need to copy
any DLLs (other than the device adapter itself) into the Micro-Manager
directory.
