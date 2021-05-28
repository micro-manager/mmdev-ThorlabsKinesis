Micro-Manager Thorlabs Kinesis device adapter
=============================================

This module ought to be part of the mmCoreAndDevices repository, but is
currently provided separately because it requires C++14, which is not available
in the current mmCoreAndDevices build system. C++14 features are needed in order
to generate a reasonable wrapper around the Thorlabs Kinesis SDK.

Building
--------

The device adapter is Windows-only and requires Visual Studio 2019 or later.

The Thorlabs Kinesis Software needs to be installed in its default location
(`C:\Program Files\Thorlabs\Kinesis`).

Place the mmCoreAndDevices repo at the same directory level as this repository
(mmdev-ThorlabsKinesis). The MMDevice headers and static library are referenced
using relative paths.

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

The most recenly tested version is Kinesis 1.14.27, but any close version is
expected to work.

In the Hardware Configuration Wizard, add the ThorlabsKinesis (hub) device. If
compatible devices are detected, they will show up as "peripherals" on the hub.

If you don't see your device, please test to see if simulated devices are
recognized: you can create them with the Kinesis Simulator application,
installed with the Kinesis Software (this will tell you if the problem is
finding Kinesis devices in general, or particular to your device).

For each device, you will need to set the `StageType` to `Linear` or
`Rotational`, depending on the actuator you are using. In addition, you need to
manually input either `DeviceUnitsPerMillimeter` or `DeviceUnitsPerRevolution`.

The correct value of these parameters needs to be determined in the Kinesis
application, because the Kinesis SDK does not provide a way to access the
necessary information.

In the Kinesis application, connect to the device. Then,

1. Make sure that your device is correctly configured with the actuator you are
   using, and (therefore) the displayed position (in mm or degrees) matches
   reality. Also ensure that no mechanical collision will occur during the
   following steps.
2. Home the device if required.
3. Move to the maximum position (if a linear actuator) or to 180&nbsp;degrees
   (if a rotary actuator). Write down this position as _P_&nbsp;mm or _Q_&nbsp;=
   180&nbsp;degrees. (Other positions can be used, but make sure it is far
   enough from zero to get good precision.)
4. Click on Settings, check Use Device Units, and click OK.
5. Now the display is in device units, _D_. For linear actuators,
   `DeviceUnitsPerMillimeter`&nbsp;= _D_&nbsp;/&nbsp;_P_. For rotary actuators,
   `DeviceUnitsPerRevolution`&nbsp;= _D_&nbsp;x&nbsp;360&nbsp;/&nbsp;_Q_.
6. To restore the display units, click on Settings, uncheck Use Device Units,
   and click OK.

### Homing

If your actuator requires homing before use, the easiest way is to use the
following script (replacing the stage device labels, of course):

```java
stages = new String[]{ "KST101-26000001", "KST101-26000002" };
for (int i = 0; i < stages.length; i++) {
    s = stages[i];
    s = mmc.getFocusDevice();
    print("Homing " + s + "...");
    mmc.home(s);
    mmc.waitForDevice(s);
    print("Finished homing " + s);
}
```

(Or make the homing run in parallel by putting the `waitForDevice` in a separate
`for` loop -- but it's best to do so after testing everything.)
