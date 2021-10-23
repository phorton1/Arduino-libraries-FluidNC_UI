# FluidNC_UI - Installation

**[Home](readme.md)** --
**[Overview](overview.md)** --
**[Software](software.md)** --
**Installation**

The installation of this **FluidNC_UI** library amounts to merly **copying** or cloning
this repository into your **Arduino libraries** folder.

It has dependencies on three libraries that must also be installed:

- Of course, you must install **FluidNC** itself!  The version that is compatible
  with this library is available at [**my fork**](https://github.com/phorton1/Arduino-libraries-FluidNC)
  of the **bdring/FluidNC** project.
- [**FluidNC_extensions**](https://github.com/phorton1/Arduino-libraries-FluidNC_extensions) -
  a library that contains a number of *extensions* to FluidNC and which is used by the FluidNC_UI.
- The [**TFT_eSPI**](https://github.com/phorton1/Arduino-libraries-TFT_eSPI) library


The [**TFT_eSPI**](https://github.com/phorton1/Arduino-libraries-TFT_eSPI) library
that I provide is **virtually unchanged** from the original **Bodmer/TFT_eSPI** library
that you might install with the *Arduino Library Manager*.  There is one small modification
to allow a timeout during TFT calibration, but otherwise, all the "changes" are encapsulated
in my **prhSettings.h** file in that project, that is included in the library as per the
author's instructions.

Please see my [**TFT_eSPI**](https://github.com/phorton1/Arduino-libraries-TFT_eSPI) library
readme for more details.


<br>
<hr>
<div style="text-align: right">
<a href='readme.md'>HOME</a><i> - Finished!</i>
</div>
