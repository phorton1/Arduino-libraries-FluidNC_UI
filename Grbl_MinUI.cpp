
//-----------------------------------------------------------
// TFT pins (defined in TFT_eSPI/User_Setup.h - prh_setup.h)
//-----------------------------------------------------------
// I am using TFT_eSPI with a ILI9341 320x240 XPT2046 TFT.
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



#include "Grbl_MinUI.h"

#ifdef WITH_TFT
	#include "myTFT.h"
#else
	#include <Arduino.h>	// for va_list?!?
#endif



#ifdef WITH_GRBL
	#include <System.h>		// to initialize sys.state to Sleep
	#include <Logging.h>
#endif

#ifdef WITH_APPLICATION
	#include "gApp.h"
#endif



void g_debug(const char *format, ...)
{
	va_list var;
	va_start(var, format);
	char display_buffer[255];
	vsprintf(display_buffer,format,var);
	#ifdef WITH_GRBL
		log_debug(display_buffer);
	#else
		Serial.print("GDBG: ");
		Serial.println(display_buffer);
	#endif
	va_end(var);
}



#ifdef WITH_INIT_UI


	//---------------------------------------------------------------
	// gDisplayTask()
	//---------------------------------------------------------------

	#define TOUCHSCREEN_UPDATE_MS   33

	void gDisplayTask(void* pvParameters)
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
			// delay to allow g_debug from touchUI_init() to complete
		g_debug("gDisplayTask running on core %d at priority %d",xPortGetCoreID(),uxTaskPriorityGet(NULL));

		#ifdef WITH_APPLICATION
			the_app.begin();
		#endif

		while (true)
		{
			vTaskDelay(TOUCHSCREEN_UPDATE_MS / portTICK_PERIOD_MS);

			#ifdef WITH_APPLICATION
				the_app.update();
			#endif
		}
	}



	//-------------------------------------------------------
	// touchUI_init()
	//-------------------------------------------------------

	void Grbl_MinUI_init()
	{
		g_debug("Grbl_MinUI_init() started %d/%dK",xPortGetFreeHeapSize()/1024,xPortGetMinimumEverFreeHeapSize()/1024);

		// set Grbl_Esp32 sys.state to "Sleep" so that we can tell when it goes to Idle
		// in Grbl.cpp.  there should be a "None" state during startup.  And there should
		// be a setSystemState() method ...

		#ifdef WITH_GRBL
			sys.state = State::Sleep;
		#else

		// Denormalized define of vMachine SDCard CS pin.
		// If not linked to GRBL, wherein the vMachine
		// initializes the SD Card at startup, somebody
		// needs to *at least* set the pin HIGH or else
		// the Touch portion of the TFT does not work,
		// presumably due to the SDCard's non-standard
		// use of the CS bus.

			#ifdef WITH_APPLICATION
				pinMode(V_SDCARD_CS,OUTPUT);
				digitalWrite(V_SDCARD_CS,1);
			#endif

		#endif

		#ifdef WITH_TFT
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
		#endif


		// start the update task

		xTaskCreate(gDisplayTask,
			"gDisplayTask",
			10240,  // pretty big stack
			NULL,
			1,  	// priority
			NULL);

		// finished

		g_debug("Grbl_MinUI_init() finished %d/%dK",xPortGetFreeHeapSize()/1024,xPortGetMinimumEverFreeHeapSize()/1024);
			// vTaskDelay(300 / portTICK_PERIOD_MS);
			// another delay to allow the task to start

	}   // display_init()

#endif 	// WITH_TFT
