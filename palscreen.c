/*
Copyright 2014 Andrew Palardy

PalScreen user interface for PiTFT
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "palscreen.h"
#include "gfx.h"
#include "touch.h"
#include "backlight.h"
#include "touchstk.h"
#include "widgets.h"


//Main
int main()
{
    //Init GFX
    printf("GFX init\n");
    GFXInit();
    printf("GFX init complete!\n");
    
    printf("Touch init\n");
    touch_init();
    printf("Touch reading kal\n");
    touch_read_kal();
    printf("Touch init complete!\n");
    
    printf("Backlight init\n");
    backlight_init();
    printf("Backlight init complete!\n");
    
    //Turn on the backlight
    backlight_on();


    
    //Draw in a color
    GFXFillScreen(0xFFFF);
    
    //Setup 2 buttons with callbacks
    //void widget_draw_btn(int x1, int x2, int y1, int y2, uint16_t color_fg, uint16_t color_bg, void *cbk, int usr_int, char *text);

    widget_draw_button(220,340,180,295,GFXPixelColor(0,0xFF,0),GFXPixelColor(0xFF,0,0),NULL,0,NULL);
    widget_draw_button(5,105,5,105,GFXPixelColor(0,0xFF,0),GFXPixelColor(0,0,0xFF),NULL,1,NULL);
    
    while(1)
    {
        //Process the touch events
        touch_get_events();
        touch_stack_proc();
        //Check the backlight
        backlight_check();
        //Sleep for a bit (50ms)
        usleep(50000);
    }

}