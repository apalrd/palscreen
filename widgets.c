//Touchscreen interface code
//Copyright 2014 Andrew Palardy

#include <sys/types.h>
#include "typedefs.h"
#include <stdio.h>
#include "touchstk.h"
#include "widgets.h"
#include "gfx.h"


//Clear screen to a color
void widget_clear_screen(uint16_t color)
{
    //Clear the touch callbacks
    touch_stack_clear();
    
    //Redraw the screen a fixed color
    GFXFillScreen(color);
}

//Internal button functions


//Draw a button with overlay text and a pressed callback
//Colors fg is the 'drawn' color, bg is the 'pressed' color
//Text is drawn using the opposite colors (bg normally, fg when pressed)
//BG color is always used to draw a box around the element, FG when pressed
void widget_draw_btn(int x1, int x2, int y1, int y2, uint16_t color_fg, uint16_t color_bg, void *cbk, int usr_int, char *text)
{
    //Draw a filled box of color bg
    GFXFillRect(x1, y1, (x2-x1), (y2-y1), color_bg);
    
    //Draw an open rect of color fg
    GFXDrawRect(x1, y1, (x2-x1), (y2-y1), color_fg);
    
    //Calculate center
    int center_x = x1 + (x2-x1)>>1;
    int center_y = y1 + (y2-y1)>>1;
    
    //Draw text
    GFXPrintString(center_x,center_y,color_fg,color_bg,text);
    
    //Malloc a new data structire
    struct widget_data_btn *wdata = malloc(sizeof(widget_data_btn));
    
    //Fill the struct
    wdata->x1 = x1;
    wdata->x2 = x2;
    wdata->y1 = y1;
    wdata->y2 = y2;
    wdata->color_fg = color_fg;
    wdata->color_bg = color_bg;
    wdata->cbk = cbk;
    wdata->user_int = usr_int;
    wdata->text = text;
    
    //Create the touch callback
    touch_stack_alloc(x1, x2, y1, y2, wdata, widget_int_btn_td, widget_int_btn_lo, NULL, cbk);
}