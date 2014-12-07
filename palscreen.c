/*
Copyright 2014 Andrew Palardy

PalScreen user interface for PiTFT
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "palscreen.h"
#include "gfx.h"

//Main
int main()
{
    printf("Starting GFX lib\n");
    //Init the GFX lib
    GFXInit();
    
    printf("GFX init complete\n");
    
    //Change the color continuously
    while(1)
    {
        printf("Filling screen\n");
        //To test, draw a color on the screen
        GFXFillScreen(0xFFFF);
        
        printf("Swapping buffers");
        
        //Flip the buffers to draw this screen
        GFXSwapBuffer();
        
        //Wait for at least a second
        sleep(2);
        
        //Fill the screen another color
        GFXFillScreen(0x0000);
        
        //Swap the buffers again
        GFXSwapBuffer();
        
        //Wait at least a second
        sleep(2);
        
        //Fill with some other colors
        //Maybe later
    }

}