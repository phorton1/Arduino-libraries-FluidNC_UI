#include "fakeTFT.h"

#ifdef USE_FAKE_TFT

#include <SPI.h>

#define CNC3018
#define SPI_FREQUENCY           20000000
#define SPI_READ_FREQUENCY      20000000
#define SPI_TOUCH_FREQUENCY     2500000

#define TFT_MISO    19
#define TFT_MOSI    23
#define TFT_SCLK    18

#ifdef CNC3018
    #define TFT_CS      22
    #define TFT_DC      21
    #define TOUCH_CS    5
#else
    #define TFT_CS      17  // was 15 // Chip select control pin
    #define TFT_DC      16  // was 2  // Data Command control pin
    #define TOUCH_CS    5   // Chip select pin (T_CS) of touch screen
#endif


#define CS_L    digitalWrite(TFT_CS,0)
#define CS_H    digitalWrite(TFT_CS,1)
#define DC_C    digitalWrite(TFT_DC,0)
#define DC_D    digitalWrite(TFT_DC,1)


inline void end_tft_write(void);
inline void begin_tft_write(void);
void writecommand(uint8_t c);
void writedata(uint8_t d);


int in_transaction = 0;
int _width = 240;
int _height = 320;


int _vpX = 0;
int _vpY = 0;
int _vpW = _width;
int _vpH = _height;

int _xDatum = 0;
int _yDatum = 0;


//------------------------------------------------
// ILI9431 320x240
//------------------------------------------------
// For M5Stack ESP32 module with integrated ILI9341 display ONLY
// #define M5STACK

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_INIT_DELAY 0x80 // Not used unless commandlist invoked

#define TFT_NOP     0x00
#define TFT_SWRST   0x01

#define TFT_CASET   0x2A
#define TFT_PASET   0x2B
#define TFT_RAMWR   0x2C

#define TFT_RAMRD   0x2E
#define TFT_IDXRD   0xDD // ILI9341 only, indexed control register read

#define TFT_MADCTL  0x36
#define TFT_MAD_MY  0x80
#define TFT_MAD_MX  0x40
#define TFT_MAD_MV  0x20
#define TFT_MAD_ML  0x10
#define TFT_MAD_BGR 0x08
#define TFT_MAD_MH  0x04
#define TFT_MAD_RGB 0x00

#ifdef TFT_RGB_ORDER
  #if (TFT_RGB_ORDER == 1)
    #define TFT_MAD_COLOR_ORDER TFT_MAD_RGB
  #else
    #define TFT_MAD_COLOR_ORDER TFT_MAD_BGR
  #endif
#else
  #define TFT_MAD_COLOR_ORDER TFT_MAD_BGR
#endif

#define TFT_INVOFF  0x20
#define TFT_INVON   0x21


// All ILI9341 specific commands some are used by init()

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0A
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_VSCRDEF 0x33
#define ILI9341_MADCTL  0x36
#define ILI9341_VSCRSADD 0x37
#define ILI9341_PIXFMT  0x3A

#define ILI9341_WRDISBV  0x51
#define ILI9341_RDDISBV  0x52
#define ILI9341_WRCTRLD  0x53

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID4   0xD3
#define ILI9341_RDINDEX 0xD9
#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDIDX   0xDD // TBC

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04



void init_ILI9341()
{
    writecommand(0xEF);
    writedata(0x03);
    writedata(0x80);
    writedata(0x02);

    writecommand(0xCF);
    writedata(0x00);
    writedata(0XC1);
    writedata(0X30);

    writecommand(0xED);
    writedata(0x64);
    writedata(0x03);
    writedata(0X12);
    writedata(0X81);

    writecommand(0xE8);
    writedata(0x85);
    writedata(0x00);
    writedata(0x78);

    writecommand(0xCB);
    writedata(0x39);
    writedata(0x2C);
    writedata(0x00);
    writedata(0x34);
    writedata(0x02);

    writecommand(0xF7);
    writedata(0x20);

    writecommand(0xEA);
    writedata(0x00);
    writedata(0x00);

    writecommand(ILI9341_PWCTR1);    //Power control
    writedata(0x23);   //VRH[5:0]

    writecommand(ILI9341_PWCTR2);    //Power control
    writedata(0x10);   //SAP[2:0];BT[3:0]

    writecommand(ILI9341_VMCTR1);    //VCM control
    writedata(0x3e);
    writedata(0x28);

    writecommand(ILI9341_VMCTR2);    //VCM control2
    writedata(0x86);  //--

    writecommand(ILI9341_MADCTL);    // Memory Access Control
  #ifdef M5STACK
    writedata(TFT_MAD_MY | TFT_MAD_MV | TFT_MAD_COLOR_ORDER); // Rotation 0 (portrait mode)
  #else
    writedata(TFT_MAD_MX | TFT_MAD_COLOR_ORDER); // Rotation 0 (portrait mode)
  #endif

    writecommand(ILI9341_PIXFMT);
    writedata(0x55);

    writecommand(ILI9341_FRMCTR1);
    writedata(0x00);
    writedata(0x13); // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz

    writecommand(ILI9341_DFUNCTR);    // Display Function Control
    writedata(0x08);
    writedata(0x82);
    writedata(0x27);

    writecommand(0xF2);    // 3Gamma Function Disable
    writedata(0x00);

    writecommand(ILI9341_GAMMASET);    //Gamma curve selected
    writedata(0x01);

    writecommand(ILI9341_GMCTRP1);    //Set Gamma
    writedata(0x0F);
    writedata(0x31);
    writedata(0x2B);
    writedata(0x0C);
    writedata(0x0E);
    writedata(0x08);
    writedata(0x4E);
    writedata(0xF1);
    writedata(0x37);
    writedata(0x07);
    writedata(0x10);
    writedata(0x03);
    writedata(0x0E);
    writedata(0x09);
    writedata(0x00);

    writecommand(ILI9341_GMCTRN1);    //Set Gamma
    writedata(0x00);
    writedata(0x0E);
    writedata(0x14);
    writedata(0x03);
    writedata(0x11);
    writedata(0x07);
    writedata(0x31);
    writedata(0xC1);
    writedata(0x48);
    writedata(0x08);
    writedata(0x0F);
    writedata(0x0C);
    writedata(0x31);
    writedata(0x36);
    writedata(0x0F);

    writecommand(ILI9341_SLPOUT);    //Exit Sleep

    end_tft_write();
    delay(120);
    begin_tft_write();

    writecommand(ILI9341_DISPON);    //Display on

}


//------------------------------
// SPI primitives
//------------------------------


#define tft_Write_8(C)      SPI.transfer(C)
#define tft_Write_32(C)     SPI.write32(C)
#define tft_Write_32C(C,D)  SPI.write32((C)<<16 | (D))


void writecommand(uint8_t c)
{
    begin_tft_write();
    DC_C;
    tft_Write_8(c);
    DC_D;
    end_tft_write();
}


void writedata(uint8_t d)
{
    begin_tft_write();
    DC_D;
    tft_Write_8(d);
    CS_L;
    end_tft_write();
}


inline void begin_tft_write(void)
{
    in_transaction++;
    if (in_transaction == 1)
    {
        SPI.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
        CS_L;
    }
}


inline void end_tft_write(void)
{
    in_transaction--;
    if (in_transaction == 0)
    {
        CS_H;
        SPI.endTransaction();
    }
}



//------------------------------
// drawing primitives
//------------------------------

void setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
  // addr_row = 0xFFFF;
  // addr_col = 0xFFFF;

    DC_C; tft_Write_8(TFT_CASET);
    DC_D; tft_Write_32C(x0, x1);
    DC_C; tft_Write_8(TFT_PASET);
    DC_D; tft_Write_32C(y0, y1);
    DC_C; tft_Write_8(TFT_RAMWR);
    DC_D;
}

void pushBlock(uint16_t color, uint32_t len)
{
    uint8_t colorBin[] = { (uint8_t) (color >> 8), (uint8_t) color };
    if (len)
        SPI.writePattern(&colorBin[0], 2, len);
}


//------------------------------
// API
//------------------------------

void TFT_eSPI::init()
{
    // SPI.begin();
    pinMode(TFT_CS,OUTPUT);
    pinMode(TFT_DC,OUTPUT);
    pinMode(TOUCH_CS,OUTPUT);

    digitalWrite(TFT_CS,1);
    digitalWrite(TFT_DC,1);
    digitalWrite(TOUCH_CS,1);

    #if 0
        delay(200);
        writecommand(TFT_SWRST); // Software reset
        delay(150); // Wait for reset to complete

        begin_tft_write();

        init_ILI9341();

        end_tft_write();
    #endif
}


void TFT_eSPI::fillScreen(uint32_t color)
{
    return;

    fillRect(0, 0, _width, _height, color);
}



void TFT_eSPI::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
    return;

    x  += _xDatum;
    y+= _yDatum;

    // Clipping
    if ((x >= _vpW) || (y >= _vpH)) return;

    if (x < _vpX) { w += x - _vpX; x = _vpX; }
    if (y < _vpY) { h += y - _vpY; y = _vpY; }

    if ((x + w) > _vpW) w = _vpW - x;
    if ((y + h) > _vpH) h = _vpH - y;

    if ((w < 1) || (h < 1)) return;

    begin_tft_write();

    setWindow(x, y, x + w - 1, y + h - 1);

    pushBlock(color, w * h);

    end_tft_write();
}


#endif  // USE_FAKE_TFT