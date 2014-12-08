//Touchscreen interface code
//Copyright 2014 Andrew Palardy

#include <sys/types.h>
#include "typedefs.h"
#include <stdio.h>
#include <stdlib.h>
#include "touchstk.h"
#include "widgets.h"
#include "gfx.h"


//Clear screen to a color
void widget_clear_screen(uint16_t color)
{
    //Clear the touch callbacks
    touch_stack_reset();
    
    //Redraw the screen a fixed color
    GFXFillScreen(color);
}

//Internal button functions
void widget_int_btn_td(int id,struct widget_data_btn *wdata)
{
    printf("WIDGET BUTTON: callback received, touch down, user in t %d\n",wdata->user_int);
    //touch down function - Draw the boxes in inverted colors
    int w = wdata->x2 - wdata->x1;
    int h = wdata->y2 - wdata->y1;
    GFXFillRect(wdata->x1,wdata->y1,w,h,wdata->color_fg);
    
    //Draw the outline box too
    GFXDrawRect(wdata->x1,wdata->y1,w,h,wdata->color_bg);
    
    //Redraw the text the opposite color too
   
    //Skip that for now
    id = id;
}
void widget_int_btn_lo(int id,struct widget_data_btn *wdata)
{
    printf("WIDGET BUTTON: callback received, lift off, user in t %d\n",wdata->user_int);
    //lift off function - Draw the boxes in correct colors
    int w = wdata->x2 - wdata->x1;
    int h = wdata->y2 - wdata->y1;
    GFXFillRect(wdata->x1,wdata->y1,w,h,wdata->color_bg);
    
    //Draw the outline box too
    GFXDrawRect(wdata->x1,wdata->y1,w,h,wdata->color_fg);
    
    //Redraw the text the opposite color too
    //Skip that for now
    id = id;
}
void widget_int_btn_btn(int id,struct widget_data_btn *wdata)
{
    //Button function, call the user's callback
    printf("WIDGET BUTTON: callback received, button, user int %d\n",wdata->user_int);
    id = id;
}
void widget_int_btn_free(int id,struct widget_data_btn *wdata)
{
    printf("WIDGET BUTTON: callback received, free, user in t %d\n",wdata->user_int);
    //Free callback - free the data struct we allocated
    free(wdata);
    id = id;
}


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
    int center_x = x1 + ((x2-x1)>>1);
    int center_y = y1 + ((y2-y1)>>1);
    
    //Draw text
    //GFXPrintString(center_x,center_y,color_fg,color_bg,text);
    
    //Malloc a new data structire
    struct widget_data_btn *wdata = malloc(sizeof(struct widget_data_btn));
    
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
    touch_stack_alloc(x1, x2, y1, y2, wdata, widget_int_btn_td, widget_int_btn_lo, NULL, widget_int_btn_btn, widget_int_btn_free);
}