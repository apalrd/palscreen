/*
Copyright 2014 Andrew Palardy

PalScreen user interface for PiTFT
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "palscreen.h"
#include "gfx.h"
//#include "touch.h"
#include "backlight.h"

//Main
int main()
{
    //Init GFX
    printf("GFX init\n");
    GFXInit();
    printf("GFX init complete!\n");
    
    //printf("Init touch\n");
    //touch_init();
    //printf("Touch init complete!\n");
    
    printf("GPIO init\n");
    gpio_init();
    printf("GPIO init complete!\n");
    
    //Turn on the backlight
    gpio_backlight_on();


    
    //Change the color continuously
    while(1)
    {
        printf("Filling screen\n");
        //To test, draw a color on the screen
        GFXFillScreen(0xFFFF);
        
        printf("Swapping buffers\n");
        
        //Flip the buffers to draw this screen
        GFXSwapBuffer();
        
        //Wait for at least a second
        sleep(1);
        
        //Blue
        GFXFillScreen(GFXPixelColor(0x00,0x00,0xFF));
        GFXSwapBuffer();
        sleep(1);
        
        //Red
        GFXFillScreen(GFXPixelColor(0xFF,0x00,0x00));
        GFXSwapBuffer();
        sleep(1);
        
        //Green
        GFXFillScreen(GFXPixelColor(0x00,0xFF,0x00));
        GFXSwapBuffer();
        sleep(1);
        
        //Orange
        GFXFillScreen(GFXPixelColor(0xFF,0x80,0x00));
        GFXSwapBuffer();
        sleep(1);
        
        //Magenta
        GFXFillScreen(GFXPixelColor(0xFF,0x00,0xFF));
        GFXSwapBuffer();
        sleep(1);
        
        //Yellow
        GFXFillScreen(GFXPixelColor(0xFF,0xFF,0x00));
        GFXSwapBuffer();
        sleep(1);
        
        //Get touch events after all of that
        //touch_get_events();
    }

}