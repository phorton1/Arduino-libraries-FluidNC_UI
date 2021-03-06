// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Arimo_Bold_24Bitmaps[] PROGMEM = {

	// Bitmap Data:
	0x00, // ' '
	0xEE,0xEE,0xEE,0xEE,0xEE,0xE0,0x0E,0xEE,0xE0, // '!'
	0xEF,0x77,0xBB,0x9C,0xCE,0x66,0x30, // '"'
	0x04,0x30,0x30,0x80,0xC6,0x03,0x18,0x0C,0x61,0xFF,0xE7,0xFF,0x86,0x30,0x18,0xC0,0x63,0x0F,0xFF,0x3F,0xFC,0x31,0x80,0xC6,0x03,0x18,0x08,0x60,0x61,0x00, // '#'
	0x02,0x00,0x08,0x01,0xFC,0x0F,0xF8,0x7A,0xF1,0xC9,0xC7,0x27,0x1E,0x80,0x7F,0x00,0xFF,0x00,0xFF,0x00,0xBC,0x02,0x79,0x88,0xE7,0x27,0x9F,0xBC,0x3F,0xF0,0x7F,0x00,0x20,0x00,0x80, // '$'
	0x78,0x18,0x7E,0x1C,0x33,0x0C,0x19,0x8E,0x0C,0xE6,0x06,0x77,0x03,0x33,0x01,0x9B,0xBC,0xFD,0xBF,0x3D,0xD9,0x80,0xCC,0xC0,0xEE,0x60,0x67,0x30,0x71,0x98,0x30,0xCC,0x38,0x7E,0x18,0x1E,0x00, // '%'
	0x07,0xC0,0x0F,0xE0,0x1C,0x60,0x1C,0x70,0x1C,0x60,0x1D,0xE0,0x0F,0xC0,0x1F,0x00,0x3E,0x18,0x7F,0x1C,0xF3,0x9C,0xE3,0xB8,0xE1,0xF8,0xE0,0xF0,0x71,0xFE,0x7F,0xFE,0x1F,0x0E, // '&'
	0xF7,0x38,0xC6,0x30, // '''
	0x1E,0x1C,0x38,0x38,0x78,0x70,0x70,0xF0,0xF0,0xE0,0xE0,0xE0,0xE0,0xF0,0xF0,0x70,0x70,0x78,0x38,0x38,0x1C,0x1E, // '('
	0xF0,0x70,0x38,0x38,0x3C,0x1C,0x1C,0x1E,0x1E,0x1E,0x0E,0x0E,0x1E,0x1E,0x1E,0x1C,0x1C,0x3C,0x38,0x38,0x70,0xF0, // ')'
	0x1C,0x07,0x05,0xD3,0xFE,0x7F,0x07,0x83,0x61,0xDC,0x22,0x00, // '*'
	0x06,0x00,0x30,0x01,0x80,0x0C,0x00,0x60,0x7F,0xFB,0xFF,0xDF,0xFE,0x06,0x00,0x30,0x01,0x80,0x0C,0x00, // '+'
	0xEE,0xEE,0x66,0x6C, // ','
	0xFD,0xFB,0xF0, // '-'
	0xEE,0xEE, // '.'
	0x0E,0x1C,0x1C,0x1C,0x1C,0x1C,0x38,0x38,0x38,0x38,0x38,0x70,0x70,0x70,0x70,0x70,0xE0, // '/'
	0x1F,0x03,0xF8,0x7F,0xC7,0x1C,0xF1,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xF1,0xE7,0x1C,0x7F,0xC3,0xF8,0x1F,0x00, // '0'
	0x0F,0x01,0xF8,0x3F,0xC1,0xDE,0x08,0xF0,0x07,0x80,0x3C,0x01,0xE0,0x0F,0x00,0x78,0x03,0xC0,0x1E,0x00,0xF0,0x07,0x80,0x3C,0x1F,0xFE,0xFF,0xF0, // '1'
	0x1F,0x07,0xFC,0x7F,0xCF,0x1E,0xE1,0xE0,0x1E,0x01,0xE0,0x3C,0x07,0x80,0xF0,0x1E,0x03,0xC0,0x78,0x0F,0x00,0xFF,0xEF,0xFE,0xFF,0xE0, // '2'
	0x1F,0x07,0xFC,0x7F,0xCF,0x1E,0xE1,0xE0,0x1E,0x01,0xC0,0xF8,0x0F,0x00,0xFC,0x01,0xE0,0x0E,0xE0,0xEE,0x1E,0xFF,0xE7,0xFC,0x1F,0x00, // '3'
	0x01,0xE0,0x0F,0x80,0x3E,0x01,0xB8,0x0E,0xE0,0x33,0x81,0xCE,0x0E,0x38,0x38,0xE1,0xC3,0x86,0x0E,0x3F,0xFE,0xFF,0xF8,0x03,0x80,0x0E,0x00,0x38,0x00,0xE0, // '4'
	0x7F,0xC7,0xFC,0x70,0x07,0x00,0xF0,0x0F,0x00,0xFF,0x8F,0xFC,0xFF,0xEF,0x1E,0x00,0xE0,0x0E,0xE0,0xEF,0x1E,0xFF,0xE7,0xFC,0x1F,0x00, // '5'
	0x1F,0x03,0xFC,0x7F,0xC7,0x1E,0xF0,0x0E,0x00,0xEF,0x8F,0xFC,0xFB,0xEF,0x1E,0xE0,0xEE,0x0E,0xF0,0xE7,0x1E,0x7F,0xC3,0xFC,0x1F,0x00, // '6'
	0xFF,0xEF,0xFE,0xFF,0xE0,0x1C,0x01,0xC0,0x38,0x03,0x80,0x70,0x0F,0x00,0xE0,0x0E,0x01,0xE0,0x1C,0x01,0xC0,0x1C,0x03,0xC0,0x3C,0x00, // '7'
	0x1F,0x07,0xFC,0xFB,0xCF,0x1E,0xE1,0xEF,0x1E,0x71,0xC3,0xF8,0x3F,0x87,0x1C,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xF1,0xE7,0xFC,0x1F,0x00, // '8'
	0x1F,0x07,0xF8,0x7F,0xCF,0x1C,0xE1,0xEE,0x0E,0xE0,0xEF,0x1E,0xFB,0xE7,0xFE,0x3C,0xE0,0x0E,0x01,0xEE,0x1C,0xFF,0xC7,0xF8,0x3F,0x00, // '9'
	0xEE,0xEE,0x00,0x00,0x0E,0xEE,0xE0, // ':'
	0xEE,0xEE,0x00,0x00,0x0E,0xEE,0xE2,0x66,0xC0, // ';'
	0x00,0x10,0x07,0x81,0xFC,0x3F,0x8F,0xE0,0x7C,0x03,0x00,0x1F,0x00,0xFE,0x00,0xFE,0x00,0xFC,0x01,0xE0,0x01,0x00, // '<'
	0xFF,0xF7,0xFF,0xBF,0xFC,0x00,0x00,0x00,0x7F,0xFB,0xFF,0xDF,0xFE, // '='
	0x80,0x07,0x80,0x3F,0x80,0x7F,0x00,0x7F,0x00,0xF8,0x00,0xC0,0x3E,0x07,0xF1,0xFC,0x3F,0x81,0xE0,0x08,0x00,0x00, // '>'
	0x0F,0xC0,0xFF,0x87,0xFF,0x3C,0x1C,0xF0,0x78,0x01,0xC0,0x0F,0x00,0x78,0x07,0x80,0x1C,0x00,0xF0,0x00,0x00,0x00,0x00,0x3C,0x00,0xF0,0x03,0xC0,0x0F,0x00, // '?'
	0x00,0xFE,0x00,0x0F,0xFE,0x00,0xF0,0x1C,0x07,0x00,0x18,0x18,0x00,0x30,0xC0,0x00,0xC6,0x0F,0xB1,0x18,0x7F,0xC6,0x63,0x07,0x19,0x0C,0x18,0x6C,0x60,0x61,0x31,0x81,0x8C,0xC6,0x0E,0x33,0x18,0x71,0x86,0x3F,0x7C,0x18,0x79,0xE0,0x70,0x00,0x00,0xC0,0x02,0x01,0xC0,0x38,0x03,0xFF,0xC0,0x03,0xF8,0x00, // '@'
	0x07,0xC0,0x07,0xC0,0x07,0xC0,0x0E,0xE0,0x0E,0xE0,0x0E,0xE0,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x38,0x38,0x3F,0xF8,0x7F,0xF8,0x7F,0xFC,0x70,0x1C,0xF0,0x1E,0xF0,0x1E,0xE0,0x0E, // 'A'
	0xFF,0xC1,0xFF,0xE3,0xFF,0xE7,0x03,0xCE,0x07,0x9C,0x0F,0x38,0x1C,0x7F,0xF8,0xFF,0xE1,0xFF,0xF3,0x80,0xF7,0x01,0xEE,0x03,0xDC,0x07,0xBF,0xFE,0x7F,0xFC,0xFF,0xE0, // 'B'
	0x07,0xE0,0x1F,0xF8,0x3F,0xFC,0x78,0x1E,0x70,0x0C,0xF0,0x00,0xF0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xF0,0x00,0xF0,0x00,0x70,0x0C,0x78,0x1E,0x3F,0xFC,0x1F,0xF8,0x07,0xE0, // 'C'
	0xFF,0x81,0xFF,0xC3,0xFF,0xC7,0x07,0xCE,0x07,0x9C,0x07,0xB8,0x0F,0x70,0x1E,0xE0,0x1D,0xC0,0x7B,0x80,0xF7,0x01,0xEE,0x07,0x9C,0x1F,0x3F,0xFC,0x7F,0xF0,0xFF,0x80, // 'D'
	0xFF,0xFB,0xFF,0xEF,0xFF,0xB8,0x00,0xE0,0x03,0x80,0x0E,0x00,0x3F,0xFC,0xFF,0xF3,0xFF,0xCE,0x00,0x38,0x00,0xE0,0x03,0x80,0x0F,0xFF,0xBF,0xFE,0xFF,0xF8, // 'E'
	0xFF,0xF7,0xFF,0xBF,0xFD,0xC0,0x0E,0x00,0x70,0x03,0x80,0x1F,0xFE,0xFF,0xF7,0xFF,0xB8,0x01,0xC0,0x0E,0x00,0x70,0x03,0x80,0x1C,0x00,0xE0,0x00, // 'F'
	0x07,0xF0,0x0F,0xFE,0x0F,0xFF,0x8F,0x81,0xE7,0x80,0x67,0x80,0x03,0xC0,0x01,0xE0,0x00,0xE0,0x7F,0x78,0x3F,0xBC,0x1F,0xDE,0x00,0xE7,0x80,0x73,0xE0,0x78,0xFF,0xFC,0x3F,0xFC,0x07,0xF0,0x00, // 'G'
	0xE0,0x3B,0x80,0xEE,0x03,0xB8,0x0E,0xE0,0x3B,0x80,0xEE,0x03,0xBF,0xFE,0xFF,0xFB,0xFF,0xEE,0x03,0xB8,0x0E,0xE0,0x3B,0x80,0xEE,0x03,0xB8,0x0E,0xE0,0x38, // 'H'
	0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xE0, // 'I'
	0x07,0xE0,0x7E,0x07,0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE0,0x0E,0xF0,0xE7,0x0E,0x7F,0xE3,0xFC,0x1F,0x80, // 'J'
	0xE0,0x3C,0xE0,0x78,0xE0,0xF0,0xE1,0xE0,0xE3,0xC0,0xE7,0x80,0xEF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x80,0xE3,0xC0,0xE1,0xE0,0xE1,0xE0,0xE0,0xF0,0xE0,0x78,0xE0,0x3C,0xE0,0x3C, // 'K'
	0xE0,0x07,0x00,0x38,0x01,0xC0,0x0E,0x00,0x70,0x03,0x80,0x1C,0x00,0xE0,0x07,0x00,0x38,0x01,0xC0,0x0E,0x00,0x70,0x03,0xFF,0xDF,0xFE,0xFF,0xF0, // 'L'
	0xF0,0x07,0xBE,0x03,0xEF,0x80,0xFB,0xF0,0x7E,0xFC,0x1F,0xBF,0x07,0xEE,0xE3,0xBB,0xB8,0xEE,0xEE,0x3B,0xB9,0xDC,0xEE,0x77,0x3B,0x9D,0xCE,0xE3,0x63,0xB8,0xF8,0xEE,0x3E,0x3B,0x87,0x0E,0xE1,0xC3,0x80, // 'M'
	0xF0,0x3B,0xC0,0xEF,0x83,0xBE,0x0E,0xDC,0x3B,0xF0,0xEE,0xE3,0xBB,0xCE,0xE7,0x3B,0x9E,0xEE,0x3B,0xB8,0xFE,0xE1,0xFB,0x87,0xEE,0x0F,0xB8,0x1E,0xE0,0x78, // 'N'
	0x07,0xE0,0x0F,0xFC,0x0F,0xFF,0x0F,0x03,0xC7,0x01,0xE7,0x80,0x7B,0xC0,0x3D,0xC0,0x1E,0xE0,0x0F,0x70,0x07,0xBC,0x03,0xDE,0x01,0xE7,0x01,0xE3,0xC0,0xF0,0xFF,0xF0,0x3F,0xF0,0x07,0xE0,0x00, // 'O'
	0xFF,0xC3,0xFF,0xCF,0xFF,0x38,0x1E,0xE0,0x3B,0x80,0xEE,0x03,0xB8,0x1E,0xFF,0xF3,0xFF,0xCF,0xFC,0x38,0x00,0xE0,0x03,0x80,0x0E,0x00,0x38,0x00,0xE0,0x00, // 'P'
	0x07,0xE0,0x0F,0xFC,0x0F,0xFF,0x0F,0x03,0xC7,0x01,0xE7,0x80,0x7B,0xC0,0x3D,0xC0,0x1E,0xE0,0x0F,0x70,0x07,0xBC,0x03,0xDE,0x01,0xE7,0x00,0xE3,0xC0,0xF0,0xFF,0xF0,0x3F,0xF0,0x0F,0xF0,0x00,0xE0,0x00,0x78,0x00,0x1F,0x80,0x0F,0xC0,0x01,0xE0, // 'Q'
	0xFF,0xE0,0xFF,0xF0,0xFF,0xF8,0xE0,0x38,0xE0,0x3C,0xE0,0x3C,0xE0,0x3C,0xE0,0x38,0xFF,0xF8,0xFF,0xF0,0xFF,0xC0,0xE1,0xE0,0xE0,0xF0,0xE0,0xF0,0xE0,0x78,0xE0,0x3C,0xE0,0x3C, // 'R'
	0x0F,0xC0,0x7F,0xE1,0xF7,0xE3,0x81,0xCF,0x03,0xCE,0x00,0x1F,0x00,0x3F,0xE0,0x1F,0xF0,0x07,0xF0,0x00,0xF0,0x00,0xEE,0x01,0xDE,0x07,0x9F,0xFE,0x3F,0xFC,0x1F,0xE0, // 'S'
	0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80, // 'T'
	0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1C,0x70,0x1C,0x78,0x3C,0x7F,0xF8,0x3F,0xF0,0x0F,0xE0, // 'U'
	0xF0,0x0F,0x38,0x07,0x1E,0x07,0x8F,0x03,0xC3,0x81,0xC1,0xC0,0xE0,0xF0,0xF0,0x38,0x70,0x1C,0x38,0x07,0x38,0x03,0x9C,0x01,0xCE,0x00,0x7E,0x00,0x3F,0x00,0x1F,0x80,0x07,0x80,0x03,0xC0,0x00, // 'V'
	0xF0,0x7C,0x1E,0xF0,0x7C,0x1E,0x70,0x7C,0x1C,0x70,0x7C,0x1C,0x78,0x6E,0x3C,0x78,0xEE,0x3C,0x38,0xEE,0x38,0x38,0xEE,0x38,0x38,0xC6,0x38,0x3D,0xC7,0x78,0x1D,0xC7,0x70,0x1D,0xC7,0x70,0x1D,0xC7,0x70,0x1F,0x83,0x70,0x0F,0x83,0xE0,0x0F,0x83,0xE0,0x0F,0x83,0xE0, // 'W'
	0x78,0x1E,0x1C,0x0E,0x0F,0x0F,0x03,0x87,0x00,0xE7,0x80,0x7F,0x80,0x1F,0x80,0x07,0xC0,0x03,0xE0,0x03,0xF0,0x03,0xFC,0x01,0xCE,0x01,0xE7,0x81,0xE1,0xE0,0xE0,0x70,0xF0,0x3C,0x70,0x0E,0x00, // 'X'
	0x70,0x1C,0x78,0x3C,0x38,0x38,0x3C,0x78,0x1C,0x70,0x1E,0xF0,0x0E,0xE0,0x07,0xE0,0x07,0xC0,0x03,0xC0,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80, // 'Y'
	0x7F,0xF3,0xFF,0x9F,0xFC,0x03,0xC0,0x1C,0x01,0xE0,0x1E,0x00,0xE0,0x0F,0x00,0xF0,0x07,0x00,0x78,0x07,0x80,0x78,0x03,0xFF,0xDF,0xFE,0xFF,0xF0, // 'Z'
	0xFE,0xFE,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xFE,0xFE, // '['
	0xE0,0x70,0x70,0x70,0x70,0x70,0x38,0x38,0x38,0x38,0x38,0x1C,0x1C,0x1C,0x1C,0x1C,0x0E, // '\'
	0xFE,0xFE,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0xFE,0xFE, // ']'
	0x0F,0x00,0xFC,0x07,0xE0,0x73,0x03,0x9C,0x18,0x61,0xC3,0x8C,0x0C,0xE0,0x76,0x01,0x80, // '^'
	0xFF,0xF8, // '_'
	0xE0,0xE0,0xE0,0xE0, // '`'
	0x1F,0x03,0xFC,0x1C,0xF1,0xE3,0x80,0x1C,0x1F,0xE1,0xFF,0x1E,0x38,0xE1,0xC7,0x0E,0x3D,0xB9,0xFD,0xE7,0xC7,0x00, // 'a'
	0xE0,0x07,0x00,0x38,0x01,0xC0,0x0E,0x78,0x77,0xF3,0xE7,0x9E,0x1E,0xE0,0xF7,0x03,0xB8,0x1D,0xC0,0xEE,0x0F,0x78,0x7B,0xE7,0x9D,0xFC,0xE7,0x80, // 'b'
	0x1F,0x03,0xFC,0x7B,0xEF,0x0E,0xF0,0x0E,0x00,0xE0,0x0E,0x00,0xF0,0xEF,0x0E,0x79,0xE3,0xFC,0x1F,0x00, // 'c'
	0x00,0x70,0x03,0x80,0x1C,0x00,0xE1,0xE7,0x3F,0xB9,0xE7,0xDE,0x1E,0xF0,0x77,0x03,0xB8,0x1D,0xC0,0xEF,0x07,0x78,0x79,0xE7,0xCF,0xEE,0x1E,0x70, // 'd'
	0x1F,0x03,0xFC,0x71,0xCF,0x0E,0xE0,0xEF,0xFE,0xFF,0xEE,0x00,0xE0,0x0F,0x0E,0x79,0xC3,0xFC,0x1F,0x00, // 'e'
	0x1F,0x0F,0x8F,0x07,0x8F,0xF7,0xF8,0xF0,0x78,0x3C,0x1E,0x0F,0x07,0x83,0xC1,0xE0,0xF0,0x78,0x3C,0x00, // 'f'
	0x1E,0x73,0xFB,0x9E,0x7D,0xE1,0xEF,0x07,0x70,0x3B,0x81,0xDC,0x0E,0xF0,0x77,0x87,0x9E,0x7C,0xFF,0xE1,0xE7,0x00,0x39,0xC1,0xCF,0x1E,0x3F,0xE0,0xFC,0x00, // 'g'
	0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE7,0x8E,0xFE,0xFF,0xEF,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xE0, // 'h'
	0xEE,0x00,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xE0, // 'i'
	0x38,0xE0,0x00,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x9E,0xFB,0xC0, // 'j'
	0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE1,0xCE,0x38,0xE7,0x8E,0xF0,0xEE,0x0F,0xE0,0xFE,0x0F,0xF0,0xE7,0x0E,0x78,0xE3,0xCE,0x1C,0xE1,0xE0, // 'k'
	0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xE0, // 'l'
	0xE7,0x0F,0x1D,0xF3,0xF3,0xFF,0xDE,0x78,0xF1,0xCE,0x1C,0x3D,0xC3,0x87,0xB8,0x70,0xF7,0x0E,0x1E,0xE1,0xC3,0xDC,0x38,0x7B,0x87,0x0F,0x70,0xE1,0xEE,0x1C,0x3C, // 'm'
	0xE7,0x8E,0xFE,0xFF,0xEF,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xE0, // 'n'
	0x0F,0x80,0xFF,0x87,0x8F,0x3C,0x1E,0xF0,0x7B,0x80,0xEE,0x03,0xB8,0x0E,0xF0,0x7B,0xC1,0xE7,0x8F,0x0F,0xF8,0x0F,0x80, // 'o'
	0xE7,0x87,0x7F,0x3E,0x79,0xE1,0xEE,0x0F,0x70,0x3B,0x81,0xDC,0x0E,0xE0,0xF7,0x87,0xBE,0x79,0xFF,0xCE,0x78,0x70,0x03,0x80,0x1C,0x00,0xE0,0x07,0x00,0x00, // 'p'
	0x1E,0x73,0xFB,0x9E,0x7D,0xE1,0xEF,0x07,0x70,0x3B,0x81,0xDC,0x0E,0xF0,0x77,0x87,0x9E,0x7C,0xFF,0xE1,0xE7,0x00,0x38,0x01,0xC0,0x0E,0x00,0x70,0x03,0x80, // 'q'
	0xEE,0xFE,0xFE,0xF0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0, // 'r'
	0x1F,0x07,0xFC,0x71,0xCE,0x0E,0xF0,0x07,0xF0,0x3F,0xC0,0xFE,0x00,0xEE,0x0E,0xF1,0xE7,0xFC,0x3F,0x80, // 's'
	0x18,0x0C,0x0E,0x1F,0xEF,0xF1,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x07,0x03,0x81,0xE0,0xFC,0x3E, // 't'
	0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x1E,0xFF,0xEF,0xEE,0x3C,0xE0, // 'u'
	0xF0,0x79,0xC1,0xC7,0x07,0x1E,0x3C,0x38,0xE0,0xE3,0x83,0x9E,0x07,0x70,0x1D,0xC0,0x77,0x00,0xF8,0x03,0xE0,0x0F,0x80, // 'v'
	0xE0,0xE0,0xEE,0x1F,0x0E,0x71,0xB1,0xC7,0x1B,0x1C,0x73,0xB1,0xC7,0x3B,0x9C,0x33,0xB9,0x83,0xB1,0xB8,0x3F,0x1B,0x83,0xF1,0xF8,0x1E,0x1F,0x01,0xE0,0xF0,0x1E,0x0F,0x00, // 'w'
	0x70,0x71,0xE3,0xC3,0x8E,0x07,0x70,0x1F,0xC0,0x3E,0x00,0xF8,0x03,0xE0,0x1D,0xC0,0xF7,0x83,0x8E,0x1E,0x3C,0xF0,0x78, // 'x'
	0xF0,0x79,0xC1,0xC7,0x07,0x1E,0x3C,0x38,0xE0,0xE3,0x81,0xCE,0x07,0x70,0x1D,0xC0,0x37,0x00,0xF8,0x03,0xE0,0x07,0x80,0x1C,0x00,0xF0,0x07,0x80,0x7E,0x01,0xE0,0x00, // 'y'
	0xFF,0xDF,0xF8,0x0E,0x03,0xC0,0xF0,0x1C,0x07,0x81,0xE0,0x38,0x0E,0x03,0xC0,0x7F,0xEF,0xFC, // 'z'
	0x0F,0x87,0xE1,0xE0,0x70,0x1C,0x07,0x01,0xC0,0x70,0x1C,0x0F,0x0F,0x83,0xC0,0x3C,0x07,0x01,0xC0,0x70,0x1C,0x07,0x01,0xC0,0x78,0x1F,0x83,0xE0, // '{'
	0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE, // '|'
	0xF8,0x3F,0x03,0xC0,0x70,0x1C,0x07,0x01,0xC0,0x70,0x1C,0x07,0x80,0xF8,0x3E,0x1E,0x07,0x01,0xC0,0x70,0x1C,0x07,0x01,0xC0,0xF0,0xFC,0x3E,0x00 // '}'
};
const GFXglyph Arimo_Bold_24Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
	  {     0,   1,   1,   8,    0,    0 }, // ' '
	  {     1,   4,  17,   8,    2,  -17 }, // '!'
	  {    10,   9,   6,  12,    2,  -17 }, // '"'
	  {    17,  14,  17,  14,    0,  -17 }, // '#'
	  {    47,  14,  20,  14,    0,  -18 }, // '$'
	  {    82,  17,  17,  19,    1,  -17 }, // '%'
	  {   119,  16,  17,  18,    1,  -17 }, // '&'
	  {   153,   5,   6,   7,    1,  -17 }, // '''
	  {   157,   8,  22,   9,    1,  -17 }, // '('
	  {   179,   8,  22,   9,    0,  -17 }, // ')'
	  {   201,  10,   9,  10,    0,  -17 }, // '*'
	  {   213,  13,  12,  15,    1,  -14 }, // '+'
	  {   233,   4,   8,   8,    2,   -4 }, // ','
	  {   237,   7,   3,   9,    1,   -8 }, // '-'
	  {   240,   4,   4,   8,    2,   -4 }, // '.'
	  {   242,   8,  17,   8,    0,  -17 }, // '/'
	  {   259,  12,  17,  14,    1,  -17 }, // '0'
	  {   285,  13,  17,  14,    1,  -17 }, // '1'
	  {   313,  12,  17,  14,    1,  -17 }, // '2'
	  {   339,  12,  17,  14,    1,  -17 }, // '3'
	  {   365,  14,  17,  14,    0,  -17 }, // '4'
	  {   395,  12,  17,  14,    1,  -17 }, // '5'
	  {   421,  12,  17,  14,    1,  -17 }, // '6'
	  {   447,  12,  17,  14,    1,  -17 }, // '7'
	  {   473,  12,  17,  14,    1,  -17 }, // '8'
	  {   499,  12,  17,  14,    1,  -17 }, // '9'
	  {   525,   4,  13,   8,    2,  -13 }, // ':'
	  {   532,   4,  17,   8,    2,  -13 }, // ';'
	  {   541,  13,  13,  15,    1,  -14 }, // '<'
	  {   563,  13,   8,  15,    1,  -12 }, // '='
	  {   576,  13,  13,  15,    1,  -14 }, // '>'
	  {   598,  14,  17,  16,    1,  -17 }, // '?'
	  {   628,  22,  21,  24,    1,  -17 }, // '@'
	  {   686,  16,  17,  18,    1,  -17 }, // 'A'
	  {   720,  15,  17,  18,    2,  -17 }, // 'B'
	  {   752,  16,  17,  18,    1,  -17 }, // 'C'
	  {   786,  15,  17,  18,    2,  -17 }, // 'D'
	  {   818,  14,  17,  17,    2,  -17 }, // 'E'
	  {   848,  13,  17,  16,    2,  -17 }, // 'F'
	  {   876,  17,  17,  20,    1,  -17 }, // 'G'
	  {   913,  14,  17,  18,    2,  -17 }, // 'H'
	  {   943,   4,  17,   8,    2,  -17 }, // 'I'
	  {   952,  12,  17,  14,    0,  -17 }, // 'J'
	  {   978,  16,  17,  18,    2,  -17 }, // 'K'
	  {  1012,  13,  17,  16,    2,  -17 }, // 'L'
	  {  1040,  18,  17,  22,    2,  -17 }, // 'M'
	  {  1079,  14,  17,  18,    2,  -17 }, // 'N'
	  {  1109,  17,  17,  19,    1,  -17 }, // 'O'
	  {  1146,  14,  17,  17,    2,  -17 }, // 'P'
	  {  1176,  17,  22,  19,    1,  -17 }, // 'Q'
	  {  1223,  16,  17,  18,    2,  -17 }, // 'R'
	  {  1257,  15,  17,  17,    1,  -17 }, // 'S'
	  {  1289,  16,  17,  16,    0,  -17 }, // 'T'
	  {  1323,  16,  17,  18,    1,  -17 }, // 'U'
	  {  1357,  17,  17,  17,    0,  -17 }, // 'V'
	  {  1394,  24,  17,  24,    0,  -17 }, // 'W'
	  {  1445,  17,  17,  17,    0,  -17 }, // 'X'
	  {  1482,  16,  17,  16,    0,  -17 }, // 'Y'
	  {  1516,  13,  17,  15,    1,  -17 }, // 'Z'
	  {  1544,   8,  22,   9,    1,  -17 }, // '['
	  {  1566,   8,  17,   8,    0,  -17 }, // '\'
	  {  1583,   8,  22,   9,    0,  -17 }, // ']'
	  {  1605,  13,  10,  15,    1,  -17 }, // '^'
	  {  1622,  14,   1,  14,    0,    2 }, // '_'
	  {  1624,   7,   4,   9,    1,  -18 }, // '`'
	  {  1628,  13,  13,  14,    1,  -13 }, // 'a'
	  {  1650,  13,  17,  16,    2,  -17 }, // 'b'
	  {  1678,  12,  13,  14,    1,  -13 }, // 'c'
	  {  1698,  13,  17,  16,    1,  -17 }, // 'd'
	  {  1726,  12,  13,  14,    1,  -13 }, // 'e'
	  {  1746,   9,  17,   9,    0,  -17 }, // 'f'
	  {  1766,  13,  18,  16,    1,  -13 }, // 'g'
	  {  1796,  12,  17,  16,    2,  -17 }, // 'h'
	  {  1822,   4,  17,   8,    2,  -17 }, // 'i'
	  {  1831,   6,  22,   8,    0,  -17 }, // 'j'
	  {  1848,  12,  17,  14,    2,  -17 }, // 'k'
	  {  1874,   4,  17,   8,    2,  -17 }, // 'l'
	  {  1883,  19,  13,  22,    2,  -13 }, // 'm'
	  {  1914,  12,  13,  16,    2,  -13 }, // 'n'
	  {  1934,  14,  13,  16,    1,  -13 }, // 'o'
	  {  1957,  13,  18,  16,    2,  -13 }, // 'p'
	  {  1987,  13,  18,  16,    1,  -13 }, // 'q'
	  {  2017,   8,  13,  10,    2,  -13 }, // 'r'
	  {  2030,  12,  13,  14,    1,  -13 }, // 's'
	  {  2050,   9,  16,   9,    0,  -16 }, // 't'
	  {  2068,  12,  13,  16,    2,  -13 }, // 'u'
	  {  2088,  14,  13,  14,    0,  -13 }, // 'v'
	  {  2111,  20,  13,  20,    0,  -13 }, // 'w'
	  {  2144,  14,  13,  14,    0,  -13 }, // 'x'
	  {  2167,  14,  18,  14,    0,  -13 }, // 'y'
	  {  2199,  11,  13,  13,    1,  -13 }, // 'z'
	  {  2217,  10,  22,  10,    0,  -17 }, // '{'
	  {  2245,   4,  22,   8,    2,  -17 }, // '|'
	  {  2256,  10,  22,  10,    0,  -17 } // '}'
};
const GFXfont Arimo_Bold_24 PROGMEM = {
(uint8_t  *)Arimo_Bold_24Bitmaps,(GFXglyph *)Arimo_Bold_24Glyphs,0x20, 0x7E, 28};