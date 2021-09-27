//--------------------------------
// FluidNC_UI.cpp
//--------------------------------

#include "FluidNC_UI.h"
#include "gApp.h"
#include "myTFT.h"
#include <System.h>		// FluidNC (just to poke and test sys.state)


#define TOUCHSCREEN_UPDATE_MS   33


void gDisplayTask(void* pvParameters)
{
	the_app.begin();

	while (true)
	{
		vTaskDelay(TOUCHSCREEN_UPDATE_MS / portTICK_PERIOD_MS);
		the_app.update();
	}
}


void FluidNC_UI_init()
{
	g_debug("FluidNC_UI_init() started %d/%dK",xPortGetFreeHeapSize()/1024,xPortGetMinimumEverFreeHeapSize()/1024);

	// set FluidNC sys.state to "Sleep" so that we can tell when it goes to Idle
	// There should be a "None" state during startup.  And there should
	// be a setSystemState() method ...

	sys.state = State::Sleep;

	// start the TFT

	init_my_tft();

	// splash screen

	tft.fillScreen(TFT_BLACK);
	drawText("FluidNC",JUST_CENTER,FONT_BIG,
		0,70,320,30,COLOR_BLUE,COLOR_BLACK);
	drawText(UI_VERSION, JUST_CENTER,FONT_MONO,
		0,105,320,20,COLOR_BLUE,COLOR_BLACK);
	drawText(UI_VERSION_DATE, JUST_CENTER,FONT_MONO,
		0,130,320,20,COLOR_BLUE,COLOR_BLACK);
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
