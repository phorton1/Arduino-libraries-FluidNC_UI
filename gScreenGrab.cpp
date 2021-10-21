//--------------------------------------
// Screen Grab Implementation
//--------------------------------------
// Writes "SCREEN_GRAB(%width&X%height%)\r\n" to Uart0
// followed by len (320x240x3) bytes of data.
// Could not get it to work with SD card,
// So sending it to my console.pm program
// instead. No checksum. Lotsa holes.

#include "gApp.h"

#ifdef WITH_SCREEN_GRAB
    // empty compilation if not defined

#include "myTFT.h"
#include <Uart.h>


#define AS_TASK   1
    // run the screen grab as a one-shot task

volatile bool in_screen_grab = 0;
    // we may need to prevent the application from using SPI
    // while doing a screen grab ... so I at least stop it
    // from polling the touch screen or writing to the TFT.
    // The FluidNC SDCard is not protected.
volatile bool screen_grab_pending = 0;
    // do a screen grab on the next button press
    // triggered by ctrl-O and clicking on a button from
    // serial terminal
volatile bool do_next_screen_grab = 0;
    // do a screen grab at the end of the next UI draw cycle
    // triggered from clicking on a non-button area in uiWindow.cpp



#if AS_TASK
    void screenGrabTask(void *params)
#else
    void  gApplication::doScreenGrab(bool pending)
#endif
{
    if (in_screen_grab)     // one screen grab at a time, please!
        return;
    #if !AS_TASK
        if (pending)
        {
            screen_grab_pending = 1;
            return;
        }
    #endif

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
        vTaskDelay(1);      // 100's of a second

        for (int col=0; col<UI_SCREEN_WIDTH; col++)
        {
            yield();        // who knows?

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

    #if AS_TASK
        // you cannot just "return" from a task,
        // you must call vTaskDelete(NULL) to let
        // the system remove the task.
        vTaskDelete(NULL);
    #endif

}   // doScreenGrab()



#if AS_TASK
    void  gApplication::doScreenGrab(bool pending)
    {
        if (pending)
        {
            screen_grab_pending = true;
        }
        else if (!in_screen_grab)
        {
            xTaskCreatePinnedToCore(
                screenGrabTask,		// method
                "screenGrabTask",		// name
                8192,				// stack_size
                NULL,				// parameters
                1,  				// priority
                NULL,				// returned handle
                0);					// core 1=main FluidNC thread/task, 0=my UI and other tasks
        }
    }
#endif  // AS_TASK



#endif  // WITH_SCREEN_GRAB