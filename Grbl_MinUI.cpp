
//-----------------------------------------------------------
// TFT pins (defined in TFT_eSPI/User_Setup.h - prh_setup.h)
//-----------------------------------------------------------
// I am using TFT_eSPI with a ILI9341 320x240 XPT2046 TFT.
// It is recommended you get the TFT working before trying LVGL.
// Please see https://github.com/phorton1/Arduino-libraries-TFT_eSPI
// commit history, README.md, and and prh_setup.h for more information.
//
//      VCC         5V
//      GND         GND
//      CS          17          explicit in my User_Setup.h - TFT_eSPI was 15
//      RESET       3.3V
//      D/C         16          explicit in my User_Setup.h - TFT_eSPI was 2
//      SDI         23          default ESP32 SPI MOSI
//      SCK         18          default ESP32 SPI CLK
//      LED         5V          use 100 ohm resistor
//      SDO         19          default ESP32 SPI MISO
//      T_CLK       18  jump    default ESP32 SPI CLK
//      T_CS        5           default ESP32 SPI CS explicit in my User_Setup.h - TFT_eSPI was 2
//      T_DIN       23  jump    default ESP32 SPI MOSI
//      T_DO        19  jump    MISO
//      T_IRQ       -1          Not Used
//
// LVGL took a little work. I cloned the repository and added some notes.
// Please see http://github.com/phorton1/Arduino-libraries-lvgl/README.md
// information.

#define WITH_APPLICATION 1

#include "gDefs.h"			// for version number/date
#include "Grbl_MinUI.h"		// for TEST_STANDALONE_UI
#include "myTFT.h"



#if !TEST_STANDALONE_UI
	#include <System.h>		// to initialize sys.state to Sleep
#endif

#if WITH_APPLICATION
	#include "gApp.h"
#endif




//---------------------------------------------------------------
// gDisplayTask()
//---------------------------------------------------------------

#define TOUCHSCREEN_UPDATE_MS   33

void gDisplayTask(void* pvParameters)
{
	vTaskDelay(200 / portTICK_PERIOD_MS);
		// short delay to allow debug_serial from touchUI_init() to complete
	debug_serial("gDisplayTask running on core %d at priority %d",xPortGetCoreID(),uxTaskPriorityGet(NULL));

	#if WITH_APPLICATION
		the_app.begin();
	#endif

	while (true)
    {
		vTaskDelay(TOUCHSCREEN_UPDATE_MS / portTICK_PERIOD_MS);

		#if WITH_APPLICATION
			the_app.update();
		#endif
    }
}



//-------------------------------------------------------
// touchUI_init()
//-------------------------------------------------------

void Grbl_MinUI_init()
{
    debug_serial("Grbl_MinUI_init() started %d/%dK",xPortGetFreeHeapSize()/1024,xPortGetMinimumEverFreeHeapSize()/1024);

	// set Grbl_Esp32 sys.state to "Sleep" so that we can tell when it goes to Idle
	// in Grbl.cpp.  there should be a "None" state during startup.  And there should
	// be a setSystemState() method ...

	#if !TEST_STANDALONE_UI
		sys.state = State::Sleep;
	#endif

	// initialize tft

    tft.init();
    tft.setRotation(3);
    uint16_t cal_data[5] = {252,3404,308,3417,7};
    tft.setTouch(cal_data);

    // splash screen

    tft.setTouch(cal_data);
    tft.fillScreen(TFT_BLACK);
	drawText("ESP32_GRBL",JUST_CENTER,FONT_BIG,
		0,70,320,30,COLOR_BLUE,COLOR_BLACK);
	drawText(UI_VERSION, JUST_CENTER,FONT_MONO,
		0,105,320,20,COLOR_BLUE,COLOR_BLACK);
	drawText(UI_VERSION_DATE, JUST_CENTER,FONT_MONO,
		0,130,320,20,COLOR_BLUE,COLOR_BLACK);

	// delay(5000);  // to see splash screen

	// start the update task

    xTaskCreate(gDisplayTask,
		"gDisplayTask",
		10240,  // pretty big stack
		NULL,
		1,  	// priority
		NULL);

	// finished

	debug_serial("Grbl_MinUI_init() finished %d/%dK",xPortGetFreeHeapSize()/1024,xPortGetMinimumEverFreeHeapSize()/1024);
	vTaskDelay(300 / portTICK_PERIOD_MS);
		// another delay to allow the task to start

}   // display_init()
