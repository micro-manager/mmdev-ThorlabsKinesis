Micro-Manager Thorlabs Kinesis device adapter
=============================================

TODO: Compatible Kinesis Software versions?


Building
--------

The device adapter is Windows-only and requires Visual Studio 2019 or later.

Place the micro-manager repo at the same directory level as this repository
(thorlabs-kinesis-mm-adapter). The MMDevice headers and static library are
referenced using relative paths.

Micro-Manager's `MMDevice-SharedRuntime` project should be built using the same
Platform, Configuration, and compiler version (this may require temporarily
switching the Platform Toolset of the `MMDevice-SharedRuntime` project).
Compiler versions must match because MMDevice is a static library.

The Thorlabs Kinesis Software needs to be installed in its default location.

Building the ThorlabsKinesis project should produce
`mmgr_dal_ThorlabsKinesis.dll`.


Using
-----

The Thorlabs Kinesis Software needs to be installed. There is no need to copy
any DLLs (other than the device adapter itself) into the Micro-Manager
directory.
