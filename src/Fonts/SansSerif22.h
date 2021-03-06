// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t SansSerif_plain_22Bitmaps[] PROGMEM = {

	// Bitmap Data:
	0x00, // ' '
	0xFF,0xFF,0xFC,0x3F, // '!'
	0xCF,0x3C,0xF3,0xCF,0x30, // '"'
	0x03,0x18,0x04,0x20,0x18,0xC0,0x31,0x87,0xFF,0xEF,0xFF,0xC1,0x18,0x06,0x30,0x0C,0x61,0xFF,0xFB,0xFF,0xF0,0xC6,0x01,0x8C,0x02,0x10,0x0C,0x60,0x00, // '#'
	0x04,0x00,0x80,0x10,0x1F,0xC7,0xFD,0xC8,0xB1,0x06,0x20,0xF4,0x0F,0xE0,0x7F,0x02,0x70,0x46,0x08,0xE1,0x3F,0xFE,0x3F,0x00,0x80,0x10,0x02,0x00, // '$'
	0x38,0x04,0x1B,0x01,0x0C,0x60,0x83,0x18,0x60,0xC6,0x10,0x31,0x88,0x0C,0x66,0x01,0xB1,0x1C,0x38,0x8D,0x80,0x66,0x30,0x11,0x8C,0x08,0x63,0x02,0x18,0xC1,0x06,0x30,0x80,0xD8,0x20,0x1C, // '%'
	0x0F,0x80,0x3F,0x80,0xE1,0x01,0x80,0x03,0x00,0x03,0x00,0x0F,0x00,0x3F,0x06,0x67,0x0D,0x87,0x33,0x07,0x66,0x07,0x8E,0x07,0x0E,0x1F,0x0F,0xF7,0x0F,0x87, // '&'
	0xFF,0xF0, // '''
	0x19,0x8C,0xC6,0x33,0x18,0xC6,0x31,0x8C,0x61,0x8C,0x61,0x8C,0x30, // '('
	0xC3,0x18,0x63,0x18,0x63,0x18,0xC6,0x31,0x8C,0xC6,0x33,0x19,0x80, // ')'
	0x08,0x04,0x22,0x2D,0x61,0xC0,0xE1,0xAD,0x11,0x08,0x04,0x00, // '*'
	0x03,0x00,0x0C,0x00,0x30,0x00,0xC0,0x03,0x00,0x0C,0x0F,0xFF,0xFF,0xFF,0x03,0x00,0x0C,0x00,0x30,0x00,0xC0,0x03,0x00,0x0C,0x00, // '+'
	0x6D,0xE8, // ','
	0xFF,0xF0, // '-'
	0xFC, // '.'
	0x06,0x0C,0x30,0x60,0xC3,0x06,0x0C,0x18,0x60,0xC1,0x83,0x0C,0x18,0x30,0xC1,0x80, // '/'
	0x1F,0x07,0xF1,0xC7,0x30,0x6C,0x07,0x80,0xF0,0x1E,0x03,0xC0,0x78,0x0F,0x01,0xE0,0x36,0x0C,0xE3,0x8F,0xE0,0xF8, // '0'
	0x3C,0x3F,0x0C,0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x0C,0x03,0x0F,0xFF,0xFF, // '1'
	0x7E,0x3F,0xE8,0x1C,0x03,0x00,0xC0,0x30,0x1C,0x06,0x03,0x01,0x80,0xC0,0x60,0x30,0x18,0x0F,0xFF,0xFF, // '2'
	0x7E,0x1F,0xF3,0x07,0x00,0x60,0x0C,0x01,0x80,0x60,0xF8,0x1F,0x80,0x18,0x01,0x80,0x30,0x07,0x01,0xFF,0xF3,0xF8, // '3'
	0x03,0x80,0xF0,0x1E,0x06,0xC0,0x98,0x33,0x0C,0x61,0x0C,0x61,0x98,0x33,0xFF,0xFF,0xF0,0x18,0x03,0x00,0x60,0x0C, // '4'
	0x7F,0x9F,0xE6,0x01,0x80,0x60,0x1F,0x87,0xF9,0x0E,0x01,0xC0,0x30,0x0C,0x03,0x01,0xE0,0xEF,0xF9,0xF8, // '5'
	0x0F,0x87,0xF8,0xC1,0x30,0x06,0x01,0x80,0x33,0xC7,0xFE,0xF0,0xDC,0x0F,0x81,0xF0,0x36,0x06,0xE1,0x8F,0xF0,0xF8, // '6'
	0xFF,0xFF,0xF0,0x18,0x06,0x03,0x80,0xC0,0x30,0x18,0x06,0x01,0x80,0xC0,0x30,0x1C,0x06,0x01,0x80,0xC0, // '7'
	0x1F,0x0F,0xFB,0x83,0xE0,0x3C,0x07,0x80,0xD8,0x31,0xFC,0x3F,0x8C,0x1B,0x01,0xE0,0x3C,0x07,0xC1,0xDF,0xF0,0xF8, // '8'
	0x1F,0x0F,0xF1,0x87,0x60,0x6C,0x0F,0x81,0xF0,0x3B,0x0F,0x7F,0xE3,0xCC,0x01,0x80,0x60,0x0C,0x83,0x1F,0xE1,0xF0, // '9'
	0xFC,0x00,0xFC, // ':'
	0x6D,0x80,0x00,0x6D,0xE8, // ';'
	0x00,0x04,0x00,0xF0,0x1F,0x83,0xF0,0x3F,0x03,0xE0,0x0F,0x80,0x0F,0xC0,0x0F,0xC0,0x07,0xE0,0x03,0xC0,0x01, // '<'
	0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xC0, // '='
	0x80,0x03,0xC0,0x07,0xE0,0x03,0xF0,0x01,0xF0,0x01,0xF0,0x07,0xC0,0x7C,0x0F,0xC1,0xF8,0x0F,0x00,0x20,0x00, // '>'
	0x7E,0x7F,0xA0,0xE0,0x30,0x18,0x18,0x38,0x18,0x18,0x0C,0x06,0x00,0x00,0x00,0xC0,0x60,0x30, // '?'
	0x01,0xF8,0x01,0xFF,0xC0,0x78,0x1C,0x1C,0x00,0xC7,0x00,0x0C,0xC3,0xD9,0xF8,0xFF,0x1E,0x38,0xE3,0xC6,0x0C,0x78,0xC1,0x8F,0x18,0x33,0xE3,0x8E,0xEE,0x3F,0xF8,0xC3,0xDC,0x1C,0x00,0x01,0xC0,0x10,0x1E,0x0E,0x01,0xFF,0x80,0x0F,0xC0,0x00, // '@'
	0x03,0x80,0x07,0x00,0x1B,0x00,0x36,0x00,0xEE,0x01,0x8C,0x03,0x18,0x0C,0x18,0x18,0x30,0x30,0x60,0xFF,0xE1,0xFF,0xC7,0x01,0xCC,0x01,0x98,0x03,0x60,0x03, // 'A'
	0xFF,0x1F,0xF3,0x07,0x60,0x6C,0x0D,0x81,0xB0,0x77,0xFC,0xFF,0x98,0x1B,0x01,0xE0,0x3C,0x07,0x81,0xFF,0xF7,0xF8, // 'B'
	0x07,0xE0,0xFF,0x8E,0x06,0xE0,0x16,0x00,0x60,0x03,0x00,0x18,0x00,0xC0,0x06,0x00,0x30,0x00,0xC0,0x07,0x00,0x9C,0x0C,0x7F,0xC0,0xFC, // 'C'
	0xFF,0x07,0xFE,0x30,0x39,0x80,0x6C,0x03,0xE0,0x0F,0x00,0x78,0x03,0xC0,0x1E,0x00,0xF0,0x07,0x80,0x6C,0x03,0x60,0x73,0xFF,0x1F,0xE0, // 'D'
	0xFF,0xFF,0xFC,0x03,0x00,0xC0,0x30,0x0C,0x03,0xFE,0xFF,0xB0,0x0C,0x03,0x00,0xC0,0x30,0x0F,0xFF,0xFF, // 'E'
	0xFF,0xFF,0xF0,0x18,0x0C,0x06,0x03,0x01,0xFE,0xFF,0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x80, // 'F'
	0x07,0xC0,0x7F,0xC3,0x81,0x98,0x02,0x60,0x03,0x00,0x0C,0x00,0x30,0x3F,0xC0,0xFF,0x00,0x3C,0x00,0xD8,0x03,0x70,0x0C,0xE0,0x71,0xFF,0x81,0xF8, // 'G'
	0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3F,0xFF,0xFF,0xFC,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03, // 'H'
	0xFF,0xFF,0xFF,0xFF, // 'I'
	0x18,0xC6,0x31,0x8C,0x63,0x18,0xC6,0x31,0x8C,0x63,0x19,0xFD,0xC0, // 'J'
	0xC0,0xE6,0x0E,0x30,0xE1,0x8E,0x0C,0xE0,0x6E,0x03,0xE0,0x1E,0x00,0xF8,0x06,0xE0,0x33,0x81,0x8E,0x0C,0x38,0x60,0xE3,0x03,0x98,0x0E, // 'K'
	0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x0C,0x03,0x00,0xC0,0x30,0x0F,0xFF,0xFF, // 'L'
	0xF0,0x1F,0xE0,0x3F,0xC0,0x7E,0xC1,0xBD,0x83,0x7B,0x8E,0xF3,0x19,0xE6,0x33,0xC6,0xC7,0x8D,0x8F,0x1F,0x1E,0x1C,0x3C,0x38,0x78,0x00,0xF0,0x01,0xE0,0x03, // 'M'
	0xE0,0x3F,0x03,0xF0,0x3F,0x83,0xD8,0x3C,0xC3,0xCC,0x3C,0x63,0xC6,0x3C,0x33,0xC3,0x3C,0x1B,0xC1,0xFC,0x0F,0xC0,0xFC,0x07, // 'N'
	0x07,0xC0,0x3F,0xE0,0xE0,0xE3,0x80,0xE6,0x00,0xD8,0x00,0xF0,0x01,0xE0,0x03,0xC0,0x07,0x80,0x0F,0x00,0x1B,0x00,0x67,0x01,0xC7,0x07,0x07,0xFC,0x03,0xE0, // 'O'
	0xFF,0x3F,0xEC,0x1F,0x03,0xC0,0xF0,0x3C,0x0F,0x07,0xFF,0xBF,0xCC,0x03,0x00,0xC0,0x30,0x0C,0x03,0x00, // 'P'
	0x07,0xC0,0x3F,0xE0,0xE0,0xE3,0x80,0xE6,0x00,0xD8,0x00,0xF0,0x01,0xE0,0x03,0xC0,0x07,0x80,0x0F,0x00,0x1B,0x00,0x67,0x01,0xC7,0x07,0x07,0xFC,0x03,0xF0,0x00,0x70,0x00,0x60,0x00,0x60, // 'Q'
	0xFF,0x0F,0xF8,0xC1,0xCC,0x0C,0xC0,0xCC,0x0C,0xC1,0xCF,0xF8,0xFF,0x0C,0x38,0xC1,0x8C,0x0C,0xC0,0xCC,0x06,0xC0,0x6C,0x03, // 'R'
	0x1F,0x8F,0xFB,0x81,0x60,0x0C,0x01,0x80,0x3C,0x03,0xF8,0x1F,0xC0,0x1C,0x01,0x80,0x30,0x07,0x81,0xFF,0xF1,0xF8, // 'S'
	0xFF,0xFF,0xFF,0xF0,0x30,0x00,0xC0,0x03,0x00,0x0C,0x00,0x30,0x00,0xC0,0x03,0x00,0x0C,0x00,0x30,0x00,0xC0,0x03,0x00,0x0C,0x00,0x30,0x00,0xC0, // 'T'
	0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xC0,0x3C,0x03,0xE0,0x77,0x0E,0x3F,0xC1,0xF8, // 'U'
	0xC0,0x06,0xC0,0x19,0x80,0x33,0x80,0xE3,0x01,0x86,0x03,0x06,0x0C,0x0C,0x18,0x18,0x30,0x18,0xC0,0x31,0x80,0x77,0x00,0x6C,0x00,0xD8,0x00,0xE0,0x01,0xC0, // 'V'
	0xC0,0xF0,0x3C,0x0F,0x03,0x60,0xF0,0x66,0x0F,0x06,0x61,0x98,0x66,0x19,0x86,0x31,0x98,0xC3,0x19,0x8C,0x33,0x0C,0xC3,0x30,0xCC,0x1B,0x0D,0x81,0xB0,0xD8,0x1E,0x07,0x81,0xE0,0x78,0x0E,0x07,0x00,0xE0,0x70, // 'W'
	0x70,0x1C,0xC0,0x61,0x83,0x07,0x18,0x0C,0xE0,0x1B,0x00,0x78,0x00,0xE0,0x07,0x80,0x1F,0x00,0xCC,0x07,0x18,0x18,0x70,0xC0,0xC7,0x01,0xB8,0x07, // 'X'
	0xE0,0x1D,0xC0,0xE3,0x03,0x06,0x18,0x1C,0xE0,0x33,0x00,0x78,0x01,0xE0,0x03,0x00,0x0C,0x00,0x30,0x00,0xC0,0x03,0x00,0x0C,0x00,0x30,0x00,0xC0, // 'Y'
	0xFF,0xFF,0xFF,0xC0,0x0C,0x00,0xC0,0x0E,0x00,0xE0,0x0E,0x00,0x60,0x06,0x00,0x70,0x07,0x00,0x70,0x03,0x00,0x30,0x03,0xFF,0xFF,0xFF, // 'Z'
	0xFF,0xF1,0x8C,0x63,0x18,0xC6,0x31,0x8C,0x63,0x18,0xC6,0x3F,0xF0, // '['
	0xC1,0x81,0x83,0x06,0x06,0x0C,0x18,0x30,0x30,0x60,0xC1,0x81,0x83,0x06,0x06,0x0C, // '\'
	0xFF,0xC6,0x31,0x8C,0x63,0x18,0xC6,0x31,0x8C,0x63,0x18,0xFF,0xF0, // ']'
	0x07,0x80,0x3F,0x01,0xCE,0x0E,0x1C,0x70,0x3B,0x80,0x70, // '^'
	0xFF,0xFF,0xFC, // '_'
	0xC3,0x0C,0x30, // '`'
	0x3F,0x1F,0xE4,0x1C,0x03,0x3F,0xDF,0xFE,0x0F,0x03,0xC1,0xF8,0xF7,0xFC,0xF3, // 'a'
	0xC0,0x18,0x03,0x00,0x60,0x0C,0x01,0x9E,0x3F,0xE7,0x8E,0xE0,0xF8,0x0F,0x01,0xE0,0x3C,0x07,0xC1,0xFC,0x77,0xFC,0xCF,0x00, // 'b'
	0x1F,0x8F,0xF7,0x07,0x80,0xC0,0x30,0x0C,0x03,0x00,0xE0,0x1C,0x13,0xFC,0x7E, // 'c'
	0x00,0x60,0x0C,0x01,0x80,0x30,0x06,0x3C,0xCF,0xFB,0x8F,0xE0,0xF8,0x0F,0x01,0xE0,0x3C,0x07,0xC1,0xDC,0x79,0xFF,0x1E,0x60, // 'd'
	0x1F,0x07,0xF9,0xC3,0x70,0x3C,0x07,0xFF,0xFF,0xFE,0x00,0xE0,0x0E,0x04,0xFF,0x8F,0xE0, // 'e'
	0x0F,0x1F,0x30,0x30,0x30,0xFE,0xFE,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30, // 'f'
	0x1E,0x67,0xFD,0xC7,0xF0,0x7C,0x07,0x80,0xF0,0x1E,0x03,0xE0,0xEE,0x3C,0xFF,0x8F,0x30,0x06,0x01,0xD0,0x73,0xFC,0x3F,0x00, // 'g'
	0xC0,0x30,0x0C,0x03,0x00,0xC0,0x33,0xCF,0xFB,0x87,0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0xC0, // 'h'
	0xFC,0x3F,0xFF,0xFF,0xC0, // 'i'
	0x33,0x30,0x03,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0xEE, // 'j'
	0xC0,0x18,0x03,0x00,0x60,0x0C,0x01,0x83,0xB0,0xE6,0x38,0xCE,0x1F,0x83,0xC0,0x7C,0x0D,0xC1,0x9C,0x31,0xC6,0x1C,0xC1,0xC0, // 'k'
	0xFF,0xFF,0xFF,0xFF,0xC0, // 'l'
	0xCF,0x0F,0x3F,0xEF,0xEE,0x1E,0x1F,0x03,0x03,0xC0,0xC0,0xF0,0x30,0x3C,0x0C,0x0F,0x03,0x03,0xC0,0xC0,0xF0,0x30,0x3C,0x0C,0x0F,0x03,0x03, // 'm'
	0xCF,0x3F,0xEE,0x1F,0x03,0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3C,0x0F,0x03, // 'n'
	0x1F,0x07,0xF1,0xC7,0x70,0x7C,0x07,0x80,0xF0,0x1E,0x03,0xE0,0xEE,0x38,0xFE,0x0F,0x80, // 'o'
	0xCF,0x1F,0xF3,0xC7,0x70,0x7C,0x07,0x80,0xF0,0x1E,0x03,0xE0,0xFE,0x3B,0xFE,0x67,0x8C,0x01,0x80,0x30,0x06,0x00,0xC0,0x00, // 'p'
	0x1E,0x67,0xFD,0xC7,0xF0,0x7C,0x07,0x80,0xF0,0x1E,0x03,0xE0,0xEE,0x3C,0xFF,0x8F,0x30,0x06,0x00,0xC0,0x18,0x03,0x00,0x60, // 'q'
	0xCF,0xFF,0x87,0x0C,0x18,0x30,0x60,0xC1,0x83,0x06,0x00, // 'r'
	0x3E,0x3F,0xB8,0x58,0x0E,0x03,0xF0,0x7C,0x03,0x01,0xC1,0xFF,0xCF,0xC0, // 's'
	0x60,0xC1,0x87,0xFF,0xEC,0x18,0x30,0x60,0xC1,0x83,0x06,0x0F,0xCF,0x80, // 't'
	0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0xF8,0x77,0xFC,0xF3, // 'u'
	0xC0,0x36,0x06,0x60,0x66,0x06,0x30,0xC3,0x0C,0x19,0x81,0x98,0x19,0x80,0xF0,0x0F,0x00,0xF0, // 'v'
	0xC3,0xC3,0xC3,0xC3,0x63,0xC6,0x63,0xC6,0x66,0x66,0x66,0x6E,0x36,0x6C,0x36,0x6C,0x3C,0x3C,0x3C,0x3C,0x1C,0x38,0x1C,0x38, // 'w'
	0xE0,0x77,0x0E,0x39,0xC1,0x98,0x0F,0x00,0xF0,0x0F,0x01,0xF8,0x19,0x83,0x0C,0x70,0xEE,0x07, // 'x'
	0xC0,0x36,0x06,0x60,0x67,0x0E,0x30,0xC3,0x0C,0x19,0x81,0x98,0x0F,0x00,0xF0,0x0F,0x00,0x60,0x06,0x00,0xC0,0x0C,0x07,0x80,0x78,0x00, // 'y'
	0xFF,0xFF,0xF0,0x18,0x0C,0x07,0x03,0x81,0xC0,0xE0,0x30,0x18,0x0F,0xFF,0xFF, // 'z'
	0x0F,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x38,0xF0,0xF0,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x0F, // '{'
	0xFF,0xFF,0xFF,0xFF,0xFF,0xF0, // '|'
	0xF0,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x1C,0x0F,0x0F,0x1C,0x18,0x18,0x18,0x18,0x18,0x18,0xF8,0xF0 // '}'
};
const GFXglyph SansSerif_plain_22Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
	  {     0,   1,   1,   8,    0,    0 }, // ' '
	  {     1,   2,  16,  10,    3,  -16 }, // '!'
	  {     5,   6,   6,  11,    2,  -16 }, // '"'
	  {    10,  15,  15,  19,    2,  -15 }, // '#'
	  {    39,  11,  20,  15,    2,  -17 }, // '$'
	  {    67,  18,  16,  22,    1,  -16 }, // '%'
	  {   103,  15,  16,  18,    1,  -16 }, // '&'
	  {   133,   2,   6,   7,    2,  -16 }, // '''
	  {   135,   5,  20,  10,    2,  -17 }, // '('
	  {   148,   5,  20,  10,    2,  -17 }, // ')'
	  {   161,   9,  10,  12,    1,  -16 }, // '*'
	  {   173,  14,  14,  19,    2,  -14 }, // '+'
	  {   198,   3,   5,   8,    2,   -3 }, // ','
	  {   200,   6,   2,   9,    1,   -7 }, // '-'
	  {   202,   2,   3,   8,    3,   -3 }, // '.'
	  {   203,   7,  18,   8,    0,  -16 }, // '/'
	  {   219,  11,  16,  15,    1,  -16 }, // '0'
	  {   241,  10,  16,  15,    2,  -16 }, // '1'
	  {   261,  10,  16,  15,    1,  -16 }, // '2'
	  {   281,  11,  16,  15,    1,  -16 }, // '3'
	  {   303,  11,  16,  15,    1,  -16 }, // '4'
	  {   325,  10,  16,  15,    1,  -16 }, // '5'
	  {   345,  11,  16,  15,    1,  -16 }, // '6'
	  {   367,  10,  16,  15,    1,  -16 }, // '7'
	  {   387,  11,  16,  15,    1,  -16 }, // '8'
	  {   409,  11,  16,  15,    1,  -16 }, // '9'
	  {   431,   2,  11,   8,    3,  -11 }, // ':'
	  {   434,   3,  13,   8,    2,  -11 }, // ';'
	  {   439,  14,  12,  19,    2,  -13 }, // '<'
	  {   460,  14,   7,  19,    2,  -11 }, // '='
	  {   473,  14,  12,  19,    2,  -13 }, // '>'
	  {   494,   9,  16,  13,    2,  -16 }, // '?'
	  {   512,  19,  19,  23,    1,  -15 }, // '@'
	  {   558,  15,  16,  16,    0,  -16 }, // 'A'
	  {   588,  11,  16,  16,    2,  -16 }, // 'B'
	  {   610,  13,  16,  16,    1,  -16 }, // 'C'
	  {   636,  13,  16,  18,    2,  -16 }, // 'D'
	  {   662,  10,  16,  15,    2,  -16 }, // 'E'
	  {   682,   9,  16,  14,    2,  -16 }, // 'F'
	  {   700,  14,  16,  18,    1,  -16 }, // 'G'
	  {   728,  12,  16,  18,    2,  -16 }, // 'H'
	  {   752,   2,  16,   7,    2,  -16 }, // 'I'
	  {   756,   5,  20,   7,   -1,  -16 }, // 'J'
	  {   769,  13,  16,  15,    2,  -16 }, // 'K'
	  {   795,  10,  16,  13,    2,  -16 }, // 'L'
	  {   815,  15,  16,  20,    2,  -16 }, // 'M'
	  {   845,  12,  16,  17,    2,  -16 }, // 'N'
	  {   869,  15,  16,  18,    1,  -16 }, // 'O'
	  {   899,  10,  16,  14,    2,  -16 }, // 'P'
	  {   919,  15,  19,  18,    1,  -16 }, // 'Q'
	  {   955,  12,  16,  16,    2,  -16 }, // 'R'
	  {   979,  11,  16,  15,    1,  -16 }, // 'S'
	  {  1001,  14,  16,  14,    0,  -16 }, // 'T'
	  {  1029,  12,  16,  17,    2,  -16 }, // 'U'
	  {  1053,  15,  16,  16,    0,  -16 }, // 'V'
	  {  1083,  20,  16,  23,    1,  -16 }, // 'W'
	  {  1123,  14,  16,  17,    1,  -16 }, // 'X'
	  {  1151,  14,  16,  14,    0,  -16 }, // 'Y'
	  {  1179,  13,  16,  16,    1,  -16 }, // 'Z'
	  {  1205,   5,  20,  10,    2,  -17 }, // '['
	  {  1218,   7,  18,   8,    0,  -16 }, // '\'
	  {  1234,   5,  20,  10,    2,  -17 }, // ']'
	  {  1247,  14,   6,  19,    2,  -16 }, // '^'
	  {  1258,  11,   2,  12,    0,    3 }, // '_'
	  {  1261,   5,   4,  12,    2,  -18 }, // '`'
	  {  1264,  10,  12,  14,    1,  -12 }, // 'a'
	  {  1279,  11,  17,  15,    2,  -17 }, // 'b'
	  {  1303,  10,  12,  13,    1,  -12 }, // 'c'
	  {  1318,  11,  17,  15,    1,  -17 }, // 'd'
	  {  1342,  11,  12,  14,    1,  -12 }, // 'e'
	  {  1359,   8,  17,   9,    0,  -17 }, // 'f'
	  {  1376,  11,  17,  15,    1,  -12 }, // 'g'
	  {  1400,  10,  17,  15,    2,  -17 }, // 'h'
	  {  1422,   2,  17,   7,    2,  -17 }, // 'i'
	  {  1427,   4,  22,   7,    0,  -17 }, // 'j'
	  {  1438,  11,  17,  14,    2,  -17 }, // 'k'
	  {  1462,   2,  17,   7,    2,  -17 }, // 'l'
	  {  1467,  18,  12,  23,    2,  -12 }, // 'm'
	  {  1494,  10,  12,  15,    2,  -12 }, // 'n'
	  {  1509,  11,  12,  14,    1,  -12 }, // 'o'
	  {  1526,  11,  17,  15,    2,  -12 }, // 'p'
	  {  1550,  11,  17,  15,    1,  -12 }, // 'q'
	  {  1574,   7,  12,  10,    2,  -12 }, // 'r'
	  {  1585,   9,  12,  12,    1,  -12 }, // 's'
	  {  1599,   7,  15,  10,    1,  -15 }, // 't'
	  {  1613,  10,  12,  15,    2,  -12 }, // 'u'
	  {  1628,  12,  12,  15,    1,  -12 }, // 'v'
	  {  1646,  16,  12,  19,    1,  -12 }, // 'w'
	  {  1670,  12,  12,  15,    1,  -12 }, // 'x'
	  {  1688,  12,  17,  15,    1,  -12 }, // 'y'
	  {  1714,  10,  12,  13,    1,  -12 }, // 'z'
	  {  1729,   8,  20,  15,    3,  -17 }, // '{'
	  {  1749,   2,  22,   8,    3,  -17 }, // '|'
	  {  1755,   8,  20,  15,    3,  -17 } // '}'
};
const GFXfont SansSerif_plain_22 PROGMEM = {
(uint8_t  *)SansSerif_plain_22Bitmaps,(GFXglyph *)SansSerif_plain_22Glyphs,0x20, 0x7E, 27};