/*
Copyright 2014 Andrew Palardy

Kalibration program for pakscreen
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
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
    
    printf("Init touch\n");
    touch_init();
    printf("Touch init complete!\n");
    
    printf("Backlight init\n");
    backlight_init();
    printf("Backlight init complete!\n");
    
    //Turn on the backlight
    backlight_on();


    
    //Calibrate based on 4 points, in a rectangle
    //Storage of read points
    int points_stored[8];
    int has_been_pressed = 0;
    
    //First point
    printf("First point\n");
    GFXFillScreen(GFXPixelColor(0xFF,0x00,0x00));
    //Draw a pair of lines making a crosshairs, each 50px in from the edges
    GFXDrawFastVLine(50,25,50,GFXPixelColor(0,0,0));
    GFXDrawFastHLine(25,50,50,GFXPixelColor(0,0,0));
    //Wait for them to get the touch event and make this the first kal point
    while(1)
    {
        //Get touch events
        touch_get_events();
        
        if(has_been_pressed)
        {
            //They have started to press it. Read the XY position now
            points_stored[0] = touch_get_x();
            points_stored[1] = touch_get_y();
            
            //See if they have released it and we should break
            if(!touch_down())
            {
                break;
            }
            
        }
        else
        {
            //See if they started pressing it
            if(touch_down())
            {
                //They have pressed it, set the flag
                has_been_pressed = 1;
            }
        }
        
    }
    printf("Got cal for point x=50 y=50, touched at x=%d y=%d\n",points_stored[0],points_stored[1]);
    
    //Second point
    printf("Second point\n");
    GFXFillScreen(GFXPixelColor(0xFF,0x30,0x00));
    //Draw a pair of lines making a crosshairs, each 50px in from the edges
    GFXDrawFastVLine(270,25,50,GFXPixelColor(0,0,0));
    GFXDrawFastHLine(245,50,50,GFXPixelColor(0,0,0));
    //Wait for them to get the touch event and make this the first kal point
    has_been_pressed = 0;
    sleep(2);
    while(1)
    {
        //Get touch events
        touch_get_events();
        
        if(has_been_pressed)
        {
            //They have started to press it. Read the XY position now
            points_stored[2] = touch_get_x();
            points_stored[3] = touch_get_y();
            
            //See if they have released it and we should break
            if(!touch_down())
            {
                break;
            }
            
        }
        else
        {
            //See if they started pressing it
            if(touch_down())
            {
                //They have pressed it, set the flag
                has_been_pressed = 1;
            }
        }
        
    }
    printf("Got cal for point x=270 y=50, touched at x=%d y=%d\n",points_stored[2],points_stored[3]);
    
    //Second point
    printf("Third point\n");
    GFXFillScreen(GFXPixelColor(0xFF,0x70,0x00));
    //Draw a pair of lines making a crosshairs, each 50px in from the edges
    GFXDrawFastVLine(50,165,50,GFXPixelColor(0,0,0));
    GFXDrawFastHLine(25,190,50,GFXPixelColor(0,0,0));
    //Wait for them to get the touch event and make this the first kal point
    has_been_pressed = 0;
    sleep(2);
    while(1)
    {
        //Get touch events
        touch_get_events();
        
        if(has_been_pressed)
        {
            //They have started to press it. Read the XY position now
            points_stored[4] = touch_get_x();
            points_stored[5] = touch_get_y();
            
            //See if they have released it and we should break
            if(!touch_down())
            {
                break;
            }
            
        }
        else
        {
            //See if they started pressing it
            if(touch_down())
            {
                //They have pressed it, set the flag
                has_been_pressed = 1;
            }
        }
        
    }
    printf("Got cal for point x=50 y=190, touched at x=%d y=%d\n",points_stored[4],points_stored[5]);
    
    //Second point
    printf("Fourth point\n");
    GFXFillScreen(GFXPixelColor(0xFF,0x90,0x00));
    //Draw a pair of lines making a crosshairs, each 50px in from the edges
    GFXDrawFastVLine(270,165,50,GFXPixelColor(0,0,0));
    GFXDrawFastHLine(245,190,50,GFXPixelColor(0,0,0));
    //Wait for them to get the touch event and make this the first kal point
    has_been_pressed = 0;
    sleep(2);
    while(1)
    {
        //Get touch events
        touch_get_events();
        
        if(has_been_pressed)
        {
            //They have started to press it. Read the XY position now
            points_stored[6] = touch_get_x();
            points_stored[7] = touch_get_y();
            
            //See if they have released it and we should break
            if(!touch_down())
            {
                break;
            }
            
        }
        else
        {
            //See if they started pressing it
            if(touch_down())
            {
                //They have pressed it, set the flag
                has_been_pressed = 1;
            }
        }
        
    }
    printf("Got cal for point x=270 y=190, touched at x=%d y=%d\n",points_stored[6],points_stored[7]);
    
    //Open a file for writing
    FILE *fd = fopen("screen.kal","w");
    //Points file
    /*
    fprintf(fd,"X=%d Y=%d x=%d y=%d\n",50,50,points_stored[0],points_stored[1]);
    printf("X=%d Y=%d x=%d y=%d rtn=%d\n",50,50,points_stored[0],points_stored[1]);
    fprintf(fd,"X=%d Y=%d x=%d y=%d\n",270,50,points_stored[2],points_stored[3]);
    printf("X=%d Y=%d x=%d y=%d\n",270,50,points_stored[2],points_stored[3]);
    fprintf(fd,"X=%d Y=%d x=%d y=%d\n",50,190,points_stored[4],points_stored[5]);
    printf("X=%d Y=%d x=%d y=%d\n",50,190,points_stored[4],points_stored[5]);
    fprintf(fd,"X=%d Y=%d x=%d y=%d\n",270,190,points_stored[6],points_stored[7]);
    printf("X=%d Y=%d x=%d y=%d\n",270,190,points_stored[6],points_stored[7]);
    */
    
    //Figure out the slope and offset now
    //first for X slope
    float slope = (270-50)/((points_stored[0]-points_stored[4])+(points_stored[2]-points_stored[6])*0.5);
    //Now find X offset by extending  the slope down to 0
    float offset = (50 * slope) - (points_stored[0] + points_Stored[2])*0.5;
    
    //Now make them fixed point
    int slope_fxp = (int)(slope * 256.0);
    int offset_fxp = (int)offset;
    
    //Print them both to the console and file
    printf("X gain=%f offset=%f fxpgain=%d fxpoffs=%d\n",slope,offset,slope_fxp,offset_fxp);
    fprintf(fd,"X gain=%f offset=%f fxpgain=%d fxpoffs=%d\n",slope,offset,slope_fxp,offset_fxp);
    
    //now for Y
    float slope = (270-50)/((points_stored[1]-points_stored[3])+(points_stored[5]-points_stored[6])*0.5);
    //Now find X offset by extending  the slope down to 0
    float offset = (50 * slope) - (points_stored[1] + points_Stored[3])*0.5;
    
    //Now make them fixed point
    int slope_fxp = (int)(slope * 256.0);
    int offset_fxp = (int)offset;
    
    //Print them both to the console and file
    printf("Y gain=%f offset=%f fxpgain=%d fxpoffs=%d\n",slope,offset,slope_fxp,offset_fxp);
    fprintf(fd,"Y gain=%f offset=%f fxpgain=%d fxpoffs=%d\n",slope,offset,slope_fxp,offset_fxp);
    //Close the file
    fclose(fd);

}