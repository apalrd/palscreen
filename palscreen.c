/*
Copyright 2014 Andrew Palardy

PalScreen user interface for PiTFT
*/

#include "palscreen.h"
#include "gfx.h"


//Main
int main()
{
    //Init the GFX lib
    GFXInit();
    
    //Change the color continuously
    while(1)
    {
        //To test, draw a color on the screen
        GFXFillScreen(0xFFFF);
        
        //Flip the buffers to draw this screen
        GFXSwapBuffer();
        
        //Wait for at least a second
        sleep(2);
        
        //Fill the screen another color
        GFXFillScreen(0x0000);
        
        //Swap the buffers again
        GFXSwapbuffer();
        
        //Wait at least a second
        sleep(2);
        
        //Fill with some other colors
        //Maybe later
    }

}