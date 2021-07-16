# Grbl_TouchUI

This library provides a user interface to Grbl_Esp32 on a TFT touch screen.
It makes use of the TFT_eSPI library to drive the TFT, and the LVGL library
to provide the under-pinnings of the window system.

The system consists of a tree of uiWindows, starting with the gApplication.

The gApplication has the gTitleBar at the top and the gStatusBar at the bottom
of the screen, and in-between them is the gContentArea which contains the primary
navigable content (derived from uiWindows).

The entire system is modal depending on the users place in the window tree
and the state of the gMachine, but generally the gTitleBar and gStatusBar
are visible and active throughout.

The gTitleBar contains the Main Button, the Title text area, and
the Indicator area containing the SDCard and Wifi icons.  The Main Button
provides access to "top level" windows, like the Main window, the wDirectory
listing window, the wSettings window, and so on.

The Title provides the user with context about their location within the UI
when appropriate, and gives precedence to showing the filename of any active
SDCard job which might be running at any given time.

The gStatus bar doubles as a progress indicator when an SDCard job is active
showing the percentage of the job completed at any point in time.  Normally
the uiStatus bar will show the current work and/or machine positions as text.

## File and Object Naming Convention

grblTouchUI.h and cpp are the interface to external clients who just call
the touchUI_init() method.  Everything else just "happens" as a result
of the gDisplayTask which calls the_app->update() at the refresh rate.

Apart from that, the objects and filenames are built in layers, from the
bottom up (lower layers generally do not know about higher layers):

- **UI** - the *UI* layer is an OOP wrapper around LVGL which provides
  a basic re-usable windowing system, inluding the *uiWindow*, *uiStaticText*
  *uiLabel* and son.
- **G** - the *G* layer is the Grbl Specific Application.  It includes
  the *gApplication*, *gTitleBar*, *gStatusBar* and has the *gContentArea*
  To a degree it knows, and can instantiate, any of the application
  windows (*W* objects, below), but it treats them in a polymorphic manner
  via the uiWindow base class methods.
-- **W** - objects starting with *w* are the top level windows that
  occupy the content area and make up the grblTouch_UI application.
  This includes the *wDirectory*, *wMonitor*, *wIdle*, *wJob*, *wAlarm*,
  windows and so on.
-- **D** - object starting with *d* are somewhat reusable dialog windows
  like the *dAlert* and *dConfirm* dialogs.



## Modality

The UI is modal in nature, primarily depending on the state of the gMachine
but also upon the users location in the window tree.

There are certain parts of the UI that are not available, for instance, while
a job is running, like jog navigation and setting coordinate systems (homing,
zeroing, etc).  This modality is partiuclarly built into theMain window which
is actuall three different windows depending on the state of the gMachine.

- wIdle with jog controls, etc, when the gMachine is idle,
- wJob with Pause, Stop, Reset, and Reboot buttons while a job is running
- wAlarm when the gMachine is in an Alarm State

As available, other "top level" windows, like the wSettings or wDirectory
windows can be navigated to by pressing the Main Button which brings up
a menu of the available functions at any point in time.


## Update and Top Level Windows

All uiWindows have a virtual update() method.  The purpose of the update()
method is to allow windows to adjust their state and contents before
the system calls the underlying LVGL lv_timer_handler() method to draw
on the TFT.  gApplication::update() is called at the refresh rate.

There is the notion of a "top level window" which generally takes over the
content area or in some cases the entire screen.  gApplication calls the
top level gContentArea'supdate() method, which then passes it on, as needed,
to the current top level window (shown in the content area).  The gApplication
also calls the gTitleBar::update() and gStatusBar::update() methods.

 The top level windows include

- the coneptualized wMain window which the system starts in and reverts to based on state
  which presents itself as the wIdle, wJobW, or wAlaram window.
- the wSettings window which allows the user to modify the Grbl_Esp32 settings
- the wDirectory window which lists the contents of the SD card and allows
  the user to navigate directories, manipulate files and directories, and
  initiate jobs from SD Card files.
- the wMacros window which allows the user to associate gcode files on the
  SPIFSS or SDCard with buttons in the uiJogWindow
- the wMonitor window which allows the user to see serial output from
  the gMachine and send it line oriented commands.
- the wRender window which shows the tool path in real time and/or which
  will hopefully be implemented to work with the wDirectory window to
  render the contents of a gcode file at least when the gMachine is idle

and so on.

## Modal Dialogs

As necessary the system presents modal dialog windows for confirmation
of actions, etc. Modal dialog windows are like top level windows but there
exists a mechanism for them to return a value to an underlying uiWindow.


## The Main Window and Main Button

Due to the extremely limited space on the TFT, when in the Main Window,
which is actually 3 different windows (wIdle, wJob, wAlaram) the title
or text shown on the Main Button varies to show the state of the gMachine
(idle, alarm, etc) and while a job is running, the percentage completion as
text.  Yet it remains an active button as appropriate.'

In most other top level windows, the uiMainButton label is synonymous with
the name of the top level window, and so the label will show things like
"Set", "File", "Macro", "Mon", respectively for the wSettings, wDirectory,
wMacro, and wMonitor windows and so on.

## Generic uiWindowing system

The *UI* and to some degree *G* classes represent a more or less
general purpose ESP32 TFT windowing system. It might be worth considering
factoring the uiWindow system into a separate library, for use with
other ESP32 applications, at some point.
