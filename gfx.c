/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!
 
Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/


#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "linux/fb.h"
#include <sys/mman.h>
#include <stdio.h>
#include "gfx.h"
#include "glcdfont.c"
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))


//Global variables
//GFX variables
int16_t WIDTH;
int16_t HEIGHT;
int16_t _height;
int16_t _width;
int16_t cursor_x;
int16_t cursor_y;
uint16_t textcolor;
uint16_t textbgcolor;
uint8_t textsize;
uint8_t rotation;
uint8_t wrap;
//FB variables
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
int fb_fd;
long screensize;
uint8_t *fbp;	//Front buffer base pointer
uint8_t *bbp;	//Back buffer base pointer



//Init the display and open the buffer
void GFXInit(void)
{
    printf("Attempting to open framebuffer /dev/fb1\n");
    //Open a connection to the framebuffer
    fb_fd = open("/dev/fb1",O_RDWR);
    
    printf("Reading screen info variable\n");
    //Get variable screen information
    ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
    
    printf("Reading screen info fixed\n");
    //Get fixed screen information
    ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);
    
    //Setup the frame buffer variable info
    vinfo.grayscale=0;
    vinfo.bits_per_pixel=16;
    vinfo.xres=320;
    vinfo.yres=240;
    WIDTH=320;
    HEIGHT=240;
    ioctl(fb_fd, FBIOPUT_VSCREENINFO, &vinfo);
    //Read info back to make sure it set
    ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
    
    //Print some info about it
    printf("Screen configured as grey=%d Bits=%d xres=%d yres=%d r=%d g=%d b=%d\n",vinfo.grayscale,vinfo.bits_per_pixel,vinfo.xres,vinfo.yres,vinfo.red.offset,vinfo.green.offset,vinfo.blue.offset);
    
    //Calculate screensize
    screensize = vinfo.yres_virtual * finfo.line_length;
    
    //Screen is how big
    printf("Screen is %d bytes\n",screensize);
    
    //Setup mapped memory
    fbp = mmap(0, screensize*2, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, (off_t)0);
    bbp = fbp + screensize;
    
    //Got a mem pointer?
    printf("Got a mem pointer %u and %u\n",fbp,bbp);
    
    //Setup the GFX vars
    _width = WIDTH;
    _height = HEIGHT;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textsize  = 1;
  textcolor = textbgcolor = 0xFFFF;
  wrap      = 1;
}

//Swap the active buffer (draw to the screen)
void GFXSwapBuffer()
{
    if (vinfo.yoffset==0)
        vinfo.yoffset = screensize;
    else
        vinfo.yoffset=0;
    
    //"Pan" to the back buffer
    ioctl(fb_fd, FBIOPAN_DISPLAY, &vinfo);
    
    //Update the pointer to the back buffer so we don't draw on the front buffer
    long tmp;
    tmp=fbp;
    fbp=bbp;
    bbp=tmp;
}

//Convert a pixel to a color
uint16_t GFXPixelColor(uint8_t r, uint8_t g, uint8_t b, struct fb_var_screeninfo *vinfo)
{
    return (r<<vinfo->red.offset) | (g<<vinfo->green.offset) | (b<<vinfo->blue.offset);
}

//Draw a pixel on the current screen buffer (dual-buffer sliding screen method)
void GFXDrawPixel(int16_t x, int16_t y, uint16_t color)
{
    //vinfo.bits_per_pixel>>3
    //long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel>>3) + (y+vinfo.yoffset) * finfo.line_length;
    long location = (x+vinfo.xoffset) + (y+vinfo.yoffset) * finfo.line_length;
    printf("Trying to draw a pixel at location %d\n",location);
    printf("xoff=%d yoff=%d line_len=%d\n",vinfo.xoffset,vinfo.yoffset,finfo.line_length);
    bbp[location] = color;
}

// Draw a circle outline
void GFXDrawCircle(int16_t x0, int16_t y0, int16_t r,
    uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  GFXDrawPixel(x0  , y0+r, color);
  GFXDrawPixel(x0  , y0-r, color);
  GFXDrawPixel(x0+r, y0  , color);
  GFXDrawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    GFXDrawPixel(x0 + x, y0 + y, color);
    GFXDrawPixel(x0 - x, y0 + y, color);
    GFXDrawPixel(x0 + x, y0 - y, color);
    GFXDrawPixel(x0 - x, y0 - y, color);
    GFXDrawPixel(x0 + y, y0 + x, color);
    GFXDrawPixel(x0 - y, y0 + x, color);
    GFXDrawPixel(x0 + y, y0 - x, color);
    GFXDrawPixel(x0 - y, y0 - x, color);
  }
}

void GFXDrawCircleHelper( int16_t x0, int16_t y0,
               int16_t r, uint8_t cornername, uint16_t color) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      GFXDrawPixel(x0 + x, y0 + y, color);
      GFXDrawPixel(x0 + y, y0 + x, color);
    } 
    if (cornername & 0x2) {
      GFXDrawPixel(x0 + x, y0 - y, color);
      GFXDrawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      GFXDrawPixel(x0 - y, y0 + x, color);
      GFXDrawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      GFXDrawPixel(x0 - y, y0 - x, color);
      GFXDrawPixel(x0 - x, y0 - y, color);
    }
  }
}

void GFXFillCircle(int16_t x0, int16_t y0, int16_t r,
			      uint16_t color) {
  GFXDrawFastVLine(x0, y0-r, 2*r+1, color);
  GFXFillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void GFXFillCircleHelper(int16_t x0, int16_t y0, int16_t r,
    uint8_t cornername, int16_t delta, uint16_t color) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      GFXDrawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      GFXDrawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      GFXDrawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      GFXDrawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
void GFXDrawLine(int16_t x0, int16_t y0,
			    int16_t x1, int16_t y1,
			    uint16_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      GFXDrawPixel(y0, x0, color);
    } else {
      GFXDrawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a rectangle
void GFXDrawRect(int16_t x, int16_t y,
			    int16_t w, int16_t h,
			    uint16_t color) {
  GFXDrawFastHLine(x, y, w, color);
  GFXDrawFastHLine(x, y+h-1, w, color);
  GFXDrawFastVLine(x, y, h, color);
  GFXDrawFastVLine(x+w-1, y, h, color);
}

void GFXDrawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint16_t color) {
  // Update in subclasses if desired!
    printf("Attempting to draw a fast Vline...\n");
    for(int16_t i = 0;i<h;i++)
    {
        printf("Attempting to draw a pixel...X=%d Y=%d\n",x,y+i);
        GFXDrawPixel(x,y+i,color);
    }
  //GFXDrawLine(x, y, x, y+h-1, color);
}

void GFXDrawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint16_t color) {
  // Update in subclasses if desired!
    for(int16_t i = 0;i<w;i++)
    {
        GFXDrawPixel(x+i,y,color);
    }
  //GFXDrawLine(x, y, x+w-1, y, color);
}

void GFXFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
			    uint16_t color) {
    
    //Tell the user what we're trying to do
    printf("Filling screen with rectangle x=%d y=%d w=%d h=%d color=%d\n",x,y,w,h,color);
  // Update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++) {
    GFXDrawFastVLine(i, y, h, color);
  }
}

void GFXFillScreen(uint16_t color) {
    GFXFillRect(0, 0, _width, _height, color);
}

// Draw a rounded rectangle
void GFXDrawRoundRect(int16_t x, int16_t y, int16_t w,
  int16_t h, int16_t r, uint16_t color) {
  // smarter version
  GFXDrawFastHLine(x+r  , y    , w-2*r, color); // Top
  GFXDrawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
  GFXDrawFastVLine(x    , y+r  , h-2*r, color); // Left
  GFXDrawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
  // GFXDraw four corners
  GFXDrawCircleHelper(x+r    , y+r    , r, 1, color);
  GFXDrawCircleHelper(x+w-r-1, y+r    , r, 2, color);
  GFXDrawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  GFXDrawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// Fill a rounded rectangle
void GFXFillRoundRect(int16_t x, int16_t y, int16_t w,
				 int16_t h, int16_t r, uint16_t color) {
  // smarter version
  GFXFillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  GFXFillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  GFXFillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// GFXDraw a triangle
void GFXDrawTriangle(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				int16_t x2, int16_t y2, uint16_t color) {
  GFXDrawLine(x0, y0, x1, y1, color);
  GFXDrawLine(x1, y1, x2, y2, color);
  GFXDrawLine(x2, y2, x0, y0, color);
}

// GFXFill a triangle
void GFXFillTriangle ( int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1,
				  int16_t x2, int16_t y2, uint16_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    GFXDrawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
  int32_t
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    GFXDrawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    GFXDrawFastHLine(a, y, b-a+1, color);
  }
}

void GFXDrawBitmap(int16_t x, int16_t y,
			      const uint8_t *bitmap, int16_t w, int16_t h,
			      uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        GFXDrawPixel(x+i, y+j, color);
      }
    }
  }
}

// Draw a 1-bit color bitmap at the specified x, y position from the
// provided bitmap buffer (must be PROGMEM memory) using color as the
// foreground color and bg as the background color.
void GFXDrawBitmapBit(int16_t x, int16_t y,
            const uint8_t *bitmap, int16_t w, int16_t h,
            uint16_t color, uint16_t bg) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  
  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        GFXDrawPixel(x+i, y+j, color);
      }
      else {
      	GFXDrawPixel(x+i, y+j, bg);
      }
    }
  }
}

//Draw XBitMap Files (*.xbm), exported from GIMP,
//Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
//C Array can be directly used with this function
void GFXDrawXBitmap(int16_t x, int16_t y,
                              const uint8_t *bitmap, int16_t w, int16_t h,
                              uint16_t color) {
  
  int16_t i, j, byteWidth = (w + 7) / 8;
  
  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (1 << (i % 8))) {
        GFXDrawPixel(x+i, y+j, color);
      }
    }
  }
}

void GFXWrite(uint8_t c) {
    if (c == '\n') {
    cursor_y += textsize*8;
    cursor_x  = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    GFXDrawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
    cursor_x += textsize*6;
    if (wrap && (cursor_x > (_width - textsize*6))) {
      cursor_y += textsize*8;
      cursor_x = 0;
    }
  }
}

// Draw a character
void GFXDrawChar(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size) {

  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5) 
      line = 0x0;
    else 
      line = pgm_read_byte(font+(c*5)+i);
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          GFXDrawPixel(x+i, y+j, color);
        else {  // big size
          GFXFillRect(x+(i*size), y+(j*size), size, size, color);
        } 
      } else if (bg != color) {
        if (size == 1) // default size
          GFXDrawPixel(x+i, y+j, bg);
        else {  // big size
          GFXFillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
}

void GFXSetCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void GFXSetTextSize(uint8_t s) {
  textsize = (s > 0) ? s : 1;
}

void GFXSetTextColor(uint16_t c) {
  // For 'transparent' background, we'll set the bg 
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

void GFXSetTextColorBg(uint16_t c, uint16_t b) {
  textcolor   = c;
  textbgcolor = b; 
}

void GFXSetTextWrap(uint8_t w) {
  wrap = w;
}

uint8_t GFXGetRotation(void) {
  return rotation;
}

void GFXSetRotation(uint8_t x) {
  rotation = (x & 3);
  switch(rotation) {
   case 0:
   case 2:
    _width  = WIDTH;
    _height = HEIGHT;
    break;
   case 1:
   case 3:
    _width  = HEIGHT;
    _height = WIDTH;
    break;
  }
}

// Return the size of the display (per current rotation)
int16_t GFXWidth(void) {
  return _width;
}
 
int16_t GFXHeight(void) {
  return _height;
}

