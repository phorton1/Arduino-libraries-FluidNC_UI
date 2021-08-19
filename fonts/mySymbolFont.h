const uint8_t mySymbolFontBitmaps[] PROGMEM = {

  0b00000001, 0b11111111, 0b00000000,     // WiFi symbol
  0b00000111, 0b11111111, 0b11000000,
  0b00001111, 0b00000001, 0b11100000,
  0b00011100, 0b00000000, 0b01110000,
  0b00111000, 0b00000000, 0b00111000,
  0b00100000, 0b11111110, 0b00001000,
  0b00000011, 0b11111111, 0b10000000,
  0b00001111, 0b00000001, 0b11100000,
  0b00001100, 0b00000000, 0b01100000,
  0b00000000, 0b01111100, 0b00000000,
  0b00000001, 0b11111111, 0b00000000,
  0b00000001, 0b10000011, 0b00000000,
  0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00111000, 0b00000000,
  0b00000000, 0b00111000, 0b00000000,
  0b00000000, 0b00111000, 0b00000000,

  0b00011111, 0b11110000,                 // SD Card Symbol
  0b00011111, 0b11111000,
  0b00011111, 0b11111100,
  0b00011111, 0b11111110,
  0b00011111, 0b11111110,
  0b00011111, 0b11111110,
  0b00011111, 0b11111110,
  0b00010000, 0b11000110,
  0b00010110, 0b11011010,
  0b00010111, 0b11011010,
  0b00010000, 0b11011010,
  0b00011110, 0b11011010,
  0b00011110, 0b11011010,
  0b00010000, 0b11000110,
  0b00011111, 0b11111110,
  0b00011111, 0b11111110,

};


// glyphs are
//     bitmap offset, width, height, xAdvance, dx, dy from baseline

const GFXglyph mySymbolFontGlyphs[] PROGMEM = {
  {     0,   24,  17,  24,  00,  -17 },   // 0x20 Wifi Symbol
  {    51,   16,  16,  16,  00,  -16 },   // 0x21 SDCard Symbol
};

const GFXfont mySymbolFont PROGMEM = {
  (uint8_t  *)mySymbolFontBitmaps,
  (GFXglyph *)mySymbolFontGlyphs,
  0x20, 0x21, 44 };     // ascii start, ascii end, lineheight for println

// Approx. 3007 bytes