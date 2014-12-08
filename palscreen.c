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
            int xpos = touch_get_x();
            int ypos = touch_Get_y();
            printf("Printing to the screen at x=%d y=%d\n",xpos,ypos);
            //Draw a pixel in green at the current location
            GFXDrawPixel(xpos,ypos,GFXPixelColor(0x00,0xFF,0x00));
        }
    }

}