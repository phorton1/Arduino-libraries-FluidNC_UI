//--------------------------------------
// Screen Grab Implementation
//--------------------------------------
// Writes "SCREEN_GRAB(%width&X%height%)\r\n" to Uart0
// followed by len (320x240x3) bytes of data.
// Could not get it to work with SD card,
// So sending it to my console.pm program
// instead. No checksum. Lotsa holes.


#include "gApp.h"
#include "myTFT.h"
#include <Uart.h>


volatile bool in_screen_grab = 0;
    // we may need to prevent the application from using SPI
    // while doing a screen grab ... so I at least stop it
    // from polling the touch screen or writing to the TFT.
    // The FluidNC SDCard is not protected.



void  gApplication::doScreenGrab()
    // slow as molasses
{
    in_screen_grab = 1;

    Uart0.print("SCREEN_GRAB(");
    Uart0.print(UI_SCREEN_WIDTH);
    Uart0.print("x");
    Uart0.print(UI_SCREEN_HEIGHT);
    Uart0.print(")\r\n");

    // we output the bytes in native BMP ordering
    // from the bottom row up

    for (int row=UI_SCREEN_HEIGHT-1; row>=0; row--)
    {
        vTaskDelay(1);
        for (int col=0; col<UI_SCREEN_WIDTH; col++)
        {
            #if 1
                uint16_t pixel = tft.readPixel(col, row);
                uint32_t color = tft.color16to24(pixel);
            #else
                uint32_t color = 0xff0000 + row;
            #endif

            unsigned char *bytes = (unsigned char *) &color;
            Uart0.write(*bytes++);
            Uart0.write(*bytes++);
            Uart0.write(*bytes++);
        }
    }

    Uart0.print("GRAB_DONE(");
    Uart0.print(UI_SCREEN_WIDTH * UI_SCREEN_HEIGHT * 3);
    Uart0.print(")\r\n");

    in_screen_grab = 0;
}   // doScreenGrab()
