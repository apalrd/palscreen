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
    while(1)
    {
        //Process touch events
        touch_get_events();
        
        //If we are touched, draw the pixel
        if(touch_down())
        {
            //Draw a pixel in green at the current location
            GFXDrawPixel(touch_get_x(),touch_get_y(),GFXPixelColor(0x00,0xFF,0x00));
        }
    }

}