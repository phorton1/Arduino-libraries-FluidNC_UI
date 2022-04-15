//-------------------------------------------------
// definition and slight extensions to TFT_eSPI
//-------------------------------------------------

#include "myTFT.h"


#define DEBUG_CALIBRATION


#include <SPIFFS.h>

#ifdef DEBUG_CALIBRATION
    #include "FluidNC_UI.h"
#endif

#define CALIBRATION_SANITY_VALUE   6000
	// calibration data should not be zero or larger than this

const char *my_tft_driver = "ILI9431_32";
int my_tft_rotation   = 3;

const uint16_t calibration_data_28_1[5] = {252,3404,308,3417,7};
	// ILI9341 2.8" 320x280 in rotation(1?)
const uint16_t calibration_data_32_3[5] = {449,3431,374,3437,1};
	// ILI9341 3.2" 320x280 in rotation(3)
const uint16_t calibration_data_32_1[5] = {487,3385,300,3437,7};
	// TODO:  ILI9341 3.2" 320x280 in rotation(1)
uint16_t calibration_data[5];


TFT_eSPI tft = TFT_eSPI();

#include "fonts/Dialog_plain_14.h"
#include "fonts/Dialog_plain_16.h"
#include "fonts/Dialog_plain_18.h"
#include "fonts/Dialog_plain_22.h"
#include "fonts/FreeMonoBold8pt7b.h" // "fonts/Roboto_Mono_Bold_14.h"
#include "fonts/mySymbolFont.h"


String getCalibrationFilename()
{
	String retval = "/tft_";
	retval += my_tft_driver;
	retval += "_";
	retval += my_tft_rotation;
	retval += "_data.txt";
	return retval;
}



void drawText(                   // draw clipped and justified text
        const char *text,
        JustifyType just,
        FontType font,
        int16_t x, int16_t y, int16_t w, int16_t h,
        uint16_t fg,
        uint16_t bg)
{
    tft.setTextColor(fg,bg);

    // setTextPadding() tells TFT to "fill the bounding box with background color"
    // otherwise, TFT draws the  background color to the end of the screen width (weird)

    if (fg != bg)
        tft.setTextPadding(w);

    // JUSTIFICATION
    // weird .. if we want to tell TFT to draw something centered we have to both tell
    // it that we want it centered, by setting the "datum" which is the origin point of the text to use,
    // and then specify the center of the rectangle ourselves, thus the "x,y" passed in must
    // be coupled to the "datum" via these if and switch statements.
    //
    // NOTE that "top" and "bottom" depend on if the font has "ascenders" or "descenders" and
    // does not always line up "baseline" to zero (but at least doesn't overflow "bottom" or "top")

    int16_t use_x =
        just == JUST_CENTER ||
        just == JUST_CENTER_TOP ||
        just == JUST_CENTER_BOTTOM ? w / 2 :
        just == JUST_RIGHT ||
        just == JUST_RIGHT_TOP ||
        just == JUST_RIGHT_BOTTOM ? w :
        0;
    int16_t use_y =
        just == JUST_LEFT ||
        just == JUST_CENTER ||
        just == JUST_RIGHT ? h / 2 :
        just == JUST_LEFT_BOTTOM ||
        just == JUST_CENTER_BOTTOM ||
        just == JUST_RIGHT_BOTTOM  ? h :
        0;

    uint8_t use_datum;
    switch (just)
    {
        case  JUST_LEFT:
            use_datum = ML_DATUM;
            break;
        case  JUST_RIGHT:
            use_datum = MR_DATUM;
            break;
        case  JUST_LEFT_TOP:
            use_datum = TL_DATUM;
            break;
        case  JUST_CENTER_TOP:
            use_datum = TC_DATUM;
            break;
        case  JUST_RIGHT_TOP:
            use_datum = TR_DATUM;
            break;
        case  JUST_LEFT_BOTTOM:
            use_datum = BL_DATUM;
            break;
        case  JUST_CENTER_BOTTOM:
            use_datum = BC_DATUM;
            break;
        case  JUST_RIGHT_BOTTOM:
            use_datum = BR_DATUM;
            break;
        case  JUST_CENTER:
            use_datum = MC_DATUM;
            break;
    }

    // FONTS - were also poorly implemented in TFT with a bad API, so I
    //    #ifdef'd out any fonts included in TFT_eSPI but leave LOAD_GFXFF defined.
    //    The original GFX fonts will not be linked unless they are referenced.
    //    Created a new set of fonts in my fonts/ folder, at http://oleddisplay.squix.ch
    //    and added new ones that look good and make sense.

    switch (font)
    {
        case FONT_TINY:
            tft.setFreeFont(&Dialog_plain_14);
            break;
        case FONT_SMALL:
            tft.setFreeFont(&Dialog_plain_16);
            break;
        case FONT_BIG:
            tft.setFreeFont(&Dialog_plain_22);
            break;
        case FONT_MONO:
            tft.setFreeFont(&FreeMonoBold8pt7b);
            break;
        case FONT_SYMBOL:
            tft.setFreeFont(&mySymbolFont);
            break;
        default:
        case FONT_NORMAL:
            tft.setFreeFont(&Dialog_plain_18);
            break;
    }

    // So now we can finally draw the text.
    // Set the "viewport" to implement clipping

    tft.setViewport(x,y,w,h);
    tft.setTextDatum(use_datum);
    tft.drawString(text, use_x, use_y);
    tft.resetViewport();

}



void calibrate_my_tft()
{
    #ifdef DEBUG_CALIBRATION
        g_debug("Doing TFT Calibration");
    #endif
    tft.fillScreen(TFT_BLACK);
    drawText(
        "Touch corners as indicated",
        JUST_CENTER,
        FONT_NORMAL,
        0,0,UI_SCREEN_WIDTH,UI_SCREEN_HEIGHT,
        COLOR_WHITE,
        COLOR_BLACK);

    uint16_t t_caldata[5];
    bool ok = tft.calibrateTouch(t_caldata, TFT_MAGENTA, TFT_BLACK, 15, 6000);

    if (ok)
    {
		for (int i=0; i<5; i++)     // sanity check
		{
			if (t_caldata[i] == 0 || t_caldata[i] > CALIBRATION_SANITY_VALUE)
			{
				g_debug("ABORTING calibration due to bogus calibration data");
				return;
			}
		}

        for (int i=0; i<5; i++)
            calibration_data[i] = t_caldata[i];

        #ifdef DEBUG_CALIBRATION
            g_debug("writing TFT Calibration data: %d,%d,%d,%d,%d",
                calibration_data[0],
                calibration_data[1],
                calibration_data[2],
                calibration_data[3],
                calibration_data[4]);
        #endif

		String filename = getCalibrationFilename();
        File f = SPIFFS.open(filename.c_str(), "w");
        if (f)
        {
            f.write((const unsigned char *)calibration_data, 14);
            f.close();
        }
        else
        {
            #ifdef DEBUG_CALIBRATION
                g_debug("WARNING: Could not open %s for writing. Using default calibration data",filename.c_str());
            #endif
        }
    }
    else
    {
        #ifdef DEBUG_CALIBRATION
            g_debug("TFT Calibration Failed");
        #endif
    }
}



bool getCalibrationData()
{
    bool ok = false;
	String filename = getCalibrationFilename();
    if (SPIFFS.exists(filename.c_str()))
    {
        File f = SPIFFS.open(filename.c_str(), "r");
        if (f)
        {
            uint16_t t_caldata[5];
            if (f.readBytes((char *)t_caldata, 14) == 14)
            {
                #ifdef DEBUG_CALIBRATION
                    g_debug("got TFT Calibration data: %d,%d,%d,%d,%d",
                        t_caldata[0],
                        t_caldata[1],
                        t_caldata[2],
                        t_caldata[3],
                        t_caldata[4]);
                #endif

                for (int i=0; i<5; i++)     // sanity check
                {
                    if (t_caldata[i] == 0 || t_caldata[i] > CALIBRATION_SANITY_VALUE)
                    {
                        g_debug("ABORTING read due to bogus calibration data");
                        return false;
                    }
                }

                for (int i=0; i<5; i++)
                    calibration_data[i] = t_caldata[i];

                ok = 1;

            }
            f.close();
        }
    }
    return ok;
}





void init_my_tft(
	const char *driver,
    int rotation )
{
	my_tft_driver = driver;
	my_tft_rotation = rotation;

    tft.init();
    tft.setRotation(my_tft_rotation);

	if (!strcmp(my_tft_driver,"ILI9431_28") && my_tft_rotation == 2)
	{
		memcpy(calibration_data,calibration_data_28_1,5 * sizeof(int));
	}
	else if (!strcmp(my_tft_driver,"ILI9431_32") && my_tft_rotation == 3)
	{
		memcpy(calibration_data,calibration_data_32_3,5 * sizeof(int));
	}
	else if (!strcmp(my_tft_driver,"ILI9431_32") && my_tft_rotation == 1)
	{
		memcpy(calibration_data,calibration_data_32_1,5 * sizeof(int));
	}
	else
	{
		g_error("FluidNC_UI/myTFT.cpp - UNKNOWN DRIVER(%s) ROTATION(%d) combination",my_tft_driver,my_tft_rotation);
		memcpy(calibration_data,calibration_data_32_3,5 * sizeof(int));
	}

    if (!getCalibrationData())
        calibrate_my_tft();
    tft.setTouch(calibration_data);
}
