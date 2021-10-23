# FluidNC_UI - Functional Overview

**[Home](readme.md)** --
**Overview** --
**[Software](software.md)** --
**[Installation](installation.md)**

![ui_screen.jpg](images/animated_FluidNC_UI_001.gif)

## 1. The Application

The application has a **title bar** with the **main button** to the left,
and an **indicator area**, showing the state of the *SD Card* and *WiFi*,
to the right.  The title displayed in the middle shows the
ESP32's IP address by default or other information while running,
depending on the user's location within the UI and the state of the machine.

You can *click on the title bar* to see details about the **WiFi configuration**:

![wifi_settings.png](images/wifi_settings.png)

There is a **status bar** at the bottom of the screen that
shows the **machine and work coordinates** in two rows of three columns
for the X,Y, and Z axes, respectively.  By default, the rest of the status bar shows the
current **instantaneous feed rate**,  the live **z-offset**, and if engaged, the
**mesh z offset** at the given coordinate.

![toggle_status_bar.gif](images/toggle_status_bar.gif)

Clicking on the bottom **status bar** will toggle between displaying
the current *feed rate and z-offset* (and possibly the *mesh_offset*), versus showing the
current *machine state* and *current and lowest memory available*
on the ESP32, in kilobytes, in the right portion of the status bar,
as shown above.

<br>

## 2. The Main Window

The system starts with the **Main Window** (above) which contains the **jog buttons**
that let you move the spindle 100 or 10 millimeters at a time,
a button to take you to the current XY **zero** (0) work position,
buttons to allow you to **home** the machine, **set** the zero positions
for each axis, toggle the jog buttons into **micro** mode for smaller movements,
and/or *reboot* or *reset* the machine from the **system** button.
The z-axis **safe** button will move the z-Axis to it's *home* position
with the z-axis homing **pulloff** amount applied.

*Generally the first thing you will do upon booting up the machine
is to **home** it.*

Additionally there are buttons in the rightmost column to allow you to
adjust the **feed rate**, **spindle speed**, and live **z-offset**.

<br>

### Toggling to Micro Mode

If you press the **micro** button, the jog values will change to
allow you jog the X and Y axis in increment of 1 and 1/10mm,
and the Z axis in increments of 1/10th and 1/100th of a mm
at a time.

![micro_mode.gif](images/micro_mode.gif)

The micro button will show in **bright green** if micro
mode is enabled, or in blue if it is not.


<br>

### Main Window Modes

The main window will automatically be replaced by a number of other
windows under certain circumstances.  In these cases you can no-longer
access the *jog buttons*:

- when the machine is **running a job** from the SDCard
- when the machine is in the process of **Homing**, **Probing**, or **Meshing**
- when the machine is in an **alarm state** due to a limit switch or other problem.

While the machine is in one of these states, you are presented with specific
windows that allow you to do appropriate actions at that point;

<br>

### 2a. The "Busy" Window (running an SDCard Job)

When the machine is running an *SDCard job* you will be presented with
the following window, which will show the **percentage completed** at
the top of the window, and a **progress bar** in green on the *status bar*
The *SDCard indicator* changes to yellow to show that the SD Card is busy.

The name of the **current file** is shown in bold yellow (*drill.gcode* in
this example).

![busy_window.gif](images/busy_window.gif)

You can press the **Pause/Resume** button to safely pause and resume the
job, the **Reset** button to stop the job in progress, but otherwise
leave the machine unchanged, or the **Reboot** button to stop everything
and entirely reboot the ESP32 FluidNC controller.

There are also buttons to allow you to adjust the **Feed Rate** (F100),
**Spindle Speed** (S100) and **Live Z Offset** (000) in realtime,
*while the job is running*.

<br>

### 2b. The Homing and Probing Windows

You can choose to *"home** one, or all of the axes, or to perform
a **probe** via the *Homing Dialog* window.

While *homing* or *probing* the window will change to something like
this:

![homing.png](images/homing.png)

Once you have started a *homing* or *probing* cycle, your only
alternatives are to (a) let it **complete**, (b) **reset** the
CNC controller (*FluidNC* state machine), or to (c) entirely
**reboot** the ESP32.

**Homing** issues the standard **$H**, **$HX**, **$HZ**,
or **$HZ** commands to the *FluidNC* GRBL gcode interpreter.

**Probing** issues the standard **G38.2**,
gcode command to the *FluidNC* GRBL gcode interpreter,
and if successful, **sets the z-axis zero position**
and *pulls off* a few millimeters.

Note that probing **requires a board/machine that supports
a probe input**.

TODO: disable probe UI if no probe pin defined in the yaml file.

<br>

### 2c. The "Meshing" busy window

The FluidNC_UI supports **mesh bed levelling** as implemented in the
[**FluidNC_extensions**](https://github.com/phorton1/Arduino-libraries-FluidNC_extensions)
library.

*Mesh bed levelling* may be useful when a high accuracy in the z-axis is required,
for example while **milling PCB boards**.

To use the mesh, you configure the *height and width* of the mesh, and the
number of "steps" in the x and y directions with the **Mesh Settings**
dialog (as shown in the "Other Windows" section of this readme, below).

Then, once you select **Mesh** from the "Homing
Dialog* the "meshing" process begins. Since creating the mesh can take a
substantial amount of time, the "meshing" window shows the progress
of the process as it proceeds:

![meshing.gif](images/meshing.gif)

The machine will **probe** a grid as defined in the mesh settings
and create a *map* of the heights that it will then use going
forward.  The mesh is created at whatever location the spindle
happens to be at when the command is issued.    When the mesh
generation is finished the **z-Axis zero position will be set**
and the spindle will be returned to its original location.

When a mesh exists it is **active** and the *status bar* will
show the mesh offset at any given point as the spindle moves
around:

![mesh_offset.png](images/mesh_offset.png)

In the above example, the mesh calculates that the *work
surface* is 0.114 mm above the "zero point" at
absolute machine coordinates X=25 and Y=5, which in
this case corresponds to work coordinates X=-80 and Y=-10.

Note that meshing **requires a board/machine that supports
a probe input** and that this library is compiled with the
**WITH_MESH** definition in *gDefs.h*, and that the machine
itself implements the *FluidNC_extensions* mesh feature.

Once a mesh is created *it remains in effect until it
is **cleared**, including through **reboots** and **resets**
of the machine.  You can *clear the mesh* by pressing the
**Clear** button in the *Homing Dialog" window as shown
below.

![clear_mesh.gif](images/clear_mesh.gif)

When the *Clear button* is **green** there is a
mesh active.  When it is **greyed out** there is
no mesh active.

The mesh is stored on the SPIFFS file system in a
file called **mesh_data.txt**.  You can access it,
modify it, and/or remove it by using the **WebUI**
to get to the SPIFF file system.

<br>

### 2d. The Alarm Window

The **Alarm Window** will be presented when the system enters an *alarm state*,
for example due to a *homing fail*, or a *limit switch*, or because
**$start/must_home** is set to *true* in the *config.yaml* file.

![critical_alarm_window.png](images/critical_alarm_window.png)

*Limit switches* are considered **critical** alarms, and when encountered,
the window only contains two buttons, **Reset** and **Reboot**.

![alarm_window.png](images/alarm_window.png)

In other alarm situations you also have the option of **Clearing** the
alarm and continuing at your own risk, and/or **Homing** the machine.




<br>

## 3. The Files Window

You access the **Files Window** by pressing the **Files** button from the *Main Window*.
Once in it, you can press the **Main** button to return to the *Main Window*.


If there is an *SDCard* this window lets you **traverse directories** and select files
on it to "run" as jobs on the cnc machine:

![files_window.png](images/files_window.png)

The traversal buttons are only shown when appropriate, so you *may not
see them* if there are no subdirectories and 5 or less files on the SDCard.

Sub-folders are shown with a leading slash **/**.
You traverse to (enter) a sub-folder by clicking on the directory name
in the list.

Once you **click on a file**, you will be asked if you want to "run" it.
If you then click the "Yes" button from the *confirmation dialog*,
the given **gcode file** will begin to run on the CNC machine.

![start_job.gif](images/start_job.gif)

The recognized GCode extensions are:

- **.G**
- **.GC**
- **.GCODE**
- **.NGC**

If you click on a file without one of the above extensions
you will receive an error message.




<br>

## 4. Other Windows (Dialogs) in the system

The **Homing**, **Set Positions**, **System**, and
**Confirmation** dialog windows have been touched on
briefly elsewhere in this document.  Besides those
dialog windows there are two others that may merit
a bit of further discussion.


### 4a. Feed Rates Dialog

You get to the **Feed Rates** dialog by pressing one of the
*feed rate buttons* in the *Main* or *Busy* windows.  Pressing
the button will put you on the correct line in the dialog,
so for example, pressing the **S100** button will put you
on the *Spindle Rate* line:

![spindle_rate_button.png](images/spindle_rate_button.png)

From there you can press the **plus or minus buttons** to
change the rate by 10 percent, 1 percent, or return it to
it's default value of 100:

![spindle_rate.gif](images/spindle_rate.gif)

In the Main window, you can see the current rate has been
modified to 130 in the below image by the title of the button:

![spindle_button_result.png](images/spindle_button_result.png)

If the machine implements the *FluidNC_extensions* **mesh** feature,
You can change the **live Z offset** in increments of
**0.02 or 0.002 mm** at a time.

![live_z_offset.gif](images/live_z_offset.gif)

Note that the live Z offset only *takes effect on the
next line of GCode executed* ... it does not happen
immediately!

Also note that you can use the **ctrl-QWERTY**
*realtime commands*
in the serial terminal to adjust the live Z offset:

- **ctrl-Q** = +0.020
- **ctrl-W** = +0.002
- **ctrl-R** = 0
- **ctrl-T** = -0.002
- **ctrl-Y** = -0.020

If the machine does not implement the *FluidNC_extensions*
**mesh** feature, the *live Z offset* is not available.


### 4b. Mesh Settings Dialog

You access the **Mesh Settings Dialog** via the **Settings**
button in the *Homing Dialog* window.
If the machine does not implement the *FluidNC_extensions*
**mesh** feature, the *Settings button is not available*.

The **Mesh Settings Dialog** allows you to adjust the
**height and width** of the mesh (in mm), the number
of **steps** in each direction, and the **number of
probes** done at each step.   **Up to 4 probes** can
performed at each point and the results will be averaged
together.  *The selected item is shown in **cyan***.


![mesh_settings.gif](images/mesh_settings.gif)

When you change a mesh setting, it turns **red** to indicate
it has changed and the **SAVE** button is enabled (turns *blue*).
In order for the change to take effect you must **SAVE** the setting
before changing to another line.  Settings changed in this
dialog are shown in **green** when we change to another line.




<br>
<hr>
<div style="text-align: right">
<a href='software.md'>NEXT</a><i> - Software Architecture and Details ...</i>
</div>
