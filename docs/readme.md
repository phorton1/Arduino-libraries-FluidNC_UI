# FluidNC_UI

This library provides a user interface to **FluidNC** on a TFT touch screen.
It makes use of the **TFT_eSPI** library to drive the TFT.

It is based on [my fork](https://github.com/phorton1/Arduino-libraries-FluidNC) of the
**bdring/FluidNC** project.

![ui_screen.jpg](images/animated_FluidNC_UI_001.gif)

It is designed to have a minimal impact on RAM usage and a small footprint
in program memory.  It makes no use of memory allocated dynamically at runtime.
Here is a photo of an early prototype of a CNC machine controller board that
I used to begin development of the UI:

![early_prototype_UI.jpg](images/early_prototype_UI.jpg)


## 1. The Application

The application has a **title bar** with the **main button** to the left,
and an **indicator area**, showing the state of the *SD Card* and *WiFi*,
to the right.  The title displayed in the middle depends on the uset's
location within the UI and the state of the machine.

You can *click on the title bar* to see details about the **WiFi configuration**:

![wifi_settings.png](images/wifi_settings.png)

There is a **status bar** at the bottom of the screen that
shows the **machine and work coordinates** in three columns
X,Y, and Z axes respectively.  By default, the rest of the status bar shows the
current **instantenous feed rate**,  the live **z-offset**, and if engaged, the
**mesh z offset** at the given coordinate.

![toggle_status_bar.gif](images/toggle_status_bar.gif)

Clicking on the **status bar** will toggle the machine between displaying
the current *feed rate, z-offset*, and *mesh_offset*, versus showing the
current *machine state* and *current and lowest memory available*
on the ESP32, in kilobytes, in the right portion of the status bar.

<br>

## 2. The Main Window

The system starts with the **Main Window** (above) which contains the **jog buttons**
that let you move the spindle 100 or 10 millimeters at a time,
a button to take you to the current XY **zero** (0) position,
buttons to allow you to **home** the machine, **set** the zero positions
for each axis, toggle the jog buttons into **micro** mode for smaller movements,
and/or *reboot* or *reset* the machine from the **system** button.

Additionally there are buttons in the rightmost column to allow you to
adjust the **feed rate**, **spindle speed**, and live **z-offset**.

### Main Window Modes

The main window will automatically be replaced by a number of other
windows under certain circumstances.  In these cases you can no-longer
access the *jog buttons*:

- when the machine is **running a job** from the SDCard
- when the machine is in the proccess of **Homing**, **Probing**, or **Meshing**
- when the machine is in an **alarm state** due to a limit switch or other problem.

While the machine is in one of these states, you are presented with specific
windows that allow you to do appropriate actions at that point;

### The "Busy" Window (running an SDCard Job)

When the machine is running an *SDCard job* you will be presented with
the following window, which will show the **percentage completed** at
the top of the window, and a **progress bar** in green on the *status bar*
The *SDCard indicator* changes to yellow to show that the SD Card is busy.

The name of the **current file** is shown in bold yellow (*drill.gcode* in
this example).

![busy_window.gif](images/busy_window.gif)

You can press the **Pause Button** to temporarily pause and resume the
job, the **Reset** button to stop the job in progress, but otherwise
leave the machine unchanged, or the **Reboot** button to stop everything
and entirely reboot the ESP32 FluidNC controller.

There are also buttons to allow you to adjust the **Feed Rate** (F000),
**Spindle Speed** (S000) and **Live Z Offset** (000) in realtime,
*while the job is running*.

### The Homing/Probing/Meshing Window

### The Alarm Window

### Toggling to Micro Mode


<br>

## 3. The Files Window


<br>

## 4. Other Windows (Dialogs) in the system

### Homing Dialog

### Set Positions Dialog

### System (reboot, reset) Dialog

### Feed Rates Dialog

### Mesh Settings Dialog

### Confirmation Dialog(s)


<br>

## 5. Software Architecture and details

### File and Object Naming Convention

**FluidNC_UI.h and cpp** are the interface to external clients who just call
the *FluidNC_UI_init()* method.  That is all that is required from the client,
as an ESP32 task is created which then updates the screen as needed.

The objects and filenames are built in layers, from the
bottom up (lower layers generally do not know about higher layers):

- **ui** - the *UI* layer is a somewhat general purpose windowsing system
  implemented on top of TFT_eSPI.  It includes the basic *uiElement* and
  *uiWindow* objects.
- **g** - the *G* layer is the FluidNC specific appplication and implements
  the window update and event system.
- **win** - objects starting with *win* are the top level windows that
  occupy the content area and make up the application.
- **dlg** - object starting with *dlg* are somewhat reusable dialog windows
  like the *dlgConfirm* uiWindow.



<br>

## Credits and License

This library is licensed undere the
[GNU General Public License v3.0](https://github.com/phorton1/Arduino-libraries-FluidNC_UI/tree/master/LICENSE.TXT)

**Credits**

- To **Bodmer** for the open source **TFT_eSPI** library
- To **bdring** and the **FluidNC Team**