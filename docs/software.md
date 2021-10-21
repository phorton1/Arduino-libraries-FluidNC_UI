# FluidNC_UI - Software Architecture and Details

**[Home](readme.md)** --
**[Overview](overview.md)** --
**Software** --
**[Installation](installation.md)**


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
<hr>
<div style="text-align: right">
<a href='installation.md'>NEXT</a><i> - Installation ...</i>
</div>
