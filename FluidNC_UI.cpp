//--------------------------------
// FluidNC_UI.cpp
//--------------------------------

#include "FluidNC_UI.h"
#include "gApp.h"
#include "myTFT.h"
#include <System.h>		// FluidNC (just to poke and test sys.state)

//--------------------------------
// FluidNC_UI.cpp
//--------------------------------

#include "FluidNC_UI.h"
#include "gApp.h"
#include "myTFT.h"
#include <System.h>		// FluidNC (just to poke and test sys.state)

#define KLUDGE_FIX_SD_UPLOAD
	// I finally roughly determined that the SDCard upload was failing with TFT_eSPI
	// due to the current drain on the ESP32 while keeping the CS and DC pins high.
	// A temporary fix is to set those pins to input mode while uploading, pending
	// a better long term solution, likely as simple as adding some resistors to the
	// lcd_connector.  If that works, it would likely increase the reliability of
	// sdcard jobs as well!

#ifdef KLUDGE_FIX_SD_UPLOAD
	#include <SDCard.h>                 // FluidNC
	#include <Machine/MachineConfig.h>  // FluidNC
#endif


#define TOUCHSCREEN_UPDATE_MS   33



void gDisplayTask(void* pvParameters)
{
	g_debug("gDisplayTask started on core %d",xPortGetCoreID());
	the_app.begin();

	while (true)
	{
		#ifdef KLUDGE_FIX_SD_UPLOAD

			// if the SDCard is busyUploading, stop calling the_app.update() and
			// change the TFT_CS, TFT_DC, and TOUCH_CS pins to input mode. We
			// use a longer delay, and set them back at the end.  These pin numbers
			// gotten from TFT_eSPI::prhSetup.h via including myTFT

			SDCard *sdCard = config->_sdCard;
			static bool last_busy_uploading = false;
			bool busy_uploading = sdCard && sdCard->get_state() == SDState::BusyUploading;

			if (last_busy_uploading != busy_uploading)
			{
				last_busy_uploading = busy_uploading;
				if (busy_uploading)
				{
					log_debug("start busyUploading");
					pinMode(TFT_CS, INPUT_PULLUP);
					pinMode(TFT_DC, INPUT_PULLUP);
					pinMode(TOUCH_CS, INPUT_PULLUP);
				}
				else
				{
					log_debug("end busyUploading");
					digitalWrite(TFT_CS,1);
					digitalWrite(TFT_DC,1);
					digitalWrite(TOUCH_CS,1);
					pinMode(TFT_CS, OUTPUT);
					pinMode(TFT_DC, OUTPUT);
					pinMode(TOUCH_CS, OUTPUT);
				}
			}
			if (busy_uploading)
			{
				vTaskDelay(500 / portTICK_PERIOD_MS);
			}
			else

		#endif

		{
			vTaskDelay(TOUCHSCREEN_UPDATE_MS / portTICK_PERIOD_MS);

			// Protect SPI (sdcard) against calls by the display by
			// calling SDCard.cpp::getSPISemaphore(). Could possibly
			// get rid of KLUDGE_FIX_SD_UPLOAD by implementing more
			// granular use of this semaphore in the sdcard mess.

			if (getSPISemaphore())
			{
				the_app.update();
				releaseSPISemaphore();
			}
		}
	}
}


void FluidNC_UI_init(
    const char *driver,
    int rotation)
{
	g_debug("FluidNC_UI_init(%s,%d) started %d/%dK",
			driver,
			rotation,
			xPortGetFreeHeapSize()/1024,
			xPortGetMinimumEverFreeHeapSize()/1024);

	// set FluidNC sys.state to "Sleep" so that we can tell when it goes to Idle
	// There should be a "None" state during startup.  And there should
	// be a setSystemState() method ...

	sys.state = State::Sleep;

	// start the TFT

	init_my_tft(driver,rotation);

	// splash screen

	tft.fillScreen(TFT_BLUE);  // BLACK ON BLUE is better for testing TFT_eSPI (fakeTFT.h/cpp)
	drawText("FluidNC",JUST_CENTER,FONT_BIG,
		0,70,320,30,COLOR_BLACK,COLOR_BLUE);
	drawText(UI_VERSION, JUST_CENTER,FONT_MONO,
		0,105,320,20,COLOR_BLACK,COLOR_BLUE);
	drawText(UI_VERSION_DATE, JUST_CENTER,FONT_MONO,
		0,130,320,20,COLOR_BLACK,COLOR_BLUE);
	delay(1000);  // to see splash screen

	// start the update task

	xTaskCreate(gDisplayTask,
		"gDisplayTask",
		10240,  // pretty big stack
		NULL,
		1,  	// priority
		NULL);

	// finished

	g_debug("FluidNC_UI_init() finished %d/%dK",xPortGetFreeHeapSize()/1024,xPortGetMinimumEverFreeHeapSize()/1024);
		// vTaskDelay(300 / portTICK_PERIOD_MS);
		// another delay to allow the task to start

}   // display_init()
