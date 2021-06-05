Micro-Manager Thorlabs Kinesis device adapter
=============================================

This module ought to be part of the mmCoreAndDevices repository, but is
currently provided separately because it requires C++14, which is not available
in the current mmCoreAndDevices build system. C++14 features are needed in order
to generate a reasonable wrapper around the Thorlabs Kinesis SDK.

Windows only.

Supported devices (many have not been tested with hardware):
**BenchtopBrushless,
BenchtopDCServo,
BenchtopStepper,
IntegratedStepper,
KCubeBrushless,
KCubeDCServo,
KCubeStepper,
TCubeBrushless,
TCubeDCServo,
TCubeStepper,
VerticalStage**.

Known Kinesis devices that are **not** currently supported:
BenchtopNanoTrak,
BenchtopPiezo,
BenchtopPrecisionPiezo,
BenchtopVoiceCoil,
FilterFlipper,
IntegratedPrecisionPiezo,
KCubeInertialMotor,
KCubeLaserDiode,
KCubeLaserSource,
KCubeNanoTrak
KCubePiezo,
KCubePositionAligner,
KCubeSolenoid,
KCubeStrainGauge,
ModularNanoTrak,
ModularPiezo,
ModularStepper,
Polarizer,
TCubeInertialMotor,
TCubeLaserDiode,
TCubeLaserSource,
TCubeNanoTrak,
TCubePiezo,
TCubeQuad (or position aligner),
TCubeSolenoid,
TCubeStrainGauge,
TCubeTEC.
Most of these should not be that difficult to support with some more work.

Only basic functionality (moving, getting position, and homing) is supported.
In particular, there are no properties that show up in Device Property Browser
at this time. Use Stage Control for manual control. Detailed configuration of
the devices should be done using the Kinesis application.


Installing
----------

Please see the
[Releases](https://github.com/micro-manager/mmdev-ThorlabsKinesis/releases) page
for pre-built binaries. Make sure to choose a version that matches the Device
Interface Version shown in `Help > About Micro-Manager...`.

Place the device adapter `mmgr_dal_ThorlabsKinesis.dll` in the Micro-Manager
folder.

The [Thorlabs Kinesis
Software](https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=10285)
needs to be installed. The most recenly tested version is Kinesis 1.14.27, but
any close version is expected to work. There is no need to copy any DLLs (other
than the device adapter itself) into the Micro-Manager directory.


Finally, the [Microsoft Visual C++
Redistributable](https://support.microsoft.com/en-us/topic/the-latest-supported-visual-c-downloads-2647da03-1eea-4433-9aff-95f26a218cc0)
([64-bit](https://aka.ms/vs/16/release/vc_redist.x64.exe) /
[32-bit](https://aka.ms/vs/16/release/vc_redist.x86.exe)) needs to be installed
or updated.

Configuring
-----------

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

Using
-----

Once the stage is configured, you should be able to control it using the
**Stage Control** window, available from the **Tools** menu.

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
