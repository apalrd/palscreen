#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#define swap(a, b) { int16_t t = a; a = b; b = t; }

#include <sys/types.h>
#include "typedefs.h"

void GFXInit();
void GFXSwapBuffer();
uint16_t GFXPixelColor(uint8_t r, uint8_t g, uint8_t b);
void GFXDrawPixel(int16_t x, int16_t y, uint16_t color);
void GFXDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void GFXDrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void GFXDrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void GFXDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void GFXFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void GFXFillScreen(uint16_t color);
void GFXDrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void GFXDrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                    uint16_t color);
void GFXFillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void GFXFillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                    int16_t delta, uint16_t color);
void GFXDrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                    int16_t x2, int16_t y2, uint16_t color);
void GFXFillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                    int16_t x2, int16_t y2, uint16_t color);
void GFXDrawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                    int16_t radius, uint16_t color);
void GFXFillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                    int16_t radius, uint16_t color);
void GFXDrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
                   int16_t w, int16_t h, uint16_t color);
void GFXDrawBitmapBit(int16_t x, int16_t y, const uint8_t *bitmap,
                   int16_t w, int16_t h, uint16_t color, uint16_t bg);
void GFXDrawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
                    int16_t w, int16_t h, uint16_t color);
void GFXDrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
                 uint16_t bg, uint8_t size);
void GFXSetCursor(int16_t x, int16_t y);
void GFXSetTextColor(uint16_t c);
void GFXSetTextColorBg(uint16_t c, uint16_t bg);
void GFXSetTextSize(uint8_t s);
void GFXSetTextWrap(uint8_t w);
void GFXSetRotation(uint8_t r);
void   GFXWrite(uint8_t);
void GFXPrintString(int16_t xin, int16_t yin, uint16_t fgcolor, uint16_t bgcolor,char *text);

int16_t GFXHeight(void);
int16_t GFXWidth(void);

uint8_t GFXGetRotation(void);


#endif // _ADAFRUIT_GFX_H
