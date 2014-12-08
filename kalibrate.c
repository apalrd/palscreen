/*
Copyright 2014 Andrew Palardy

Kalibration program for pakscreen
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
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
            points_stored[0] = touch_read_x();
            points_stored[1] = touch_read_y();
            
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
    printf("Got cal for point x=50 y=50, touched at x=%d y=%d\n",points_stored[0],points_Stored[1]);
    
    //Second point
    printf("Second point\n");
    GFXFillScreen(GFXPixelColor(0xFF,0x30,0x00));
    //Draw a pair of lines making a crosshairs, each 50px in from the edges
    GFXDrawFastVLine(270,25,50,GFXPixelColor(0,0,0));
    GFXDrawFastHLine(245,50,50,GFXPixelColor(0,0,0));
    //Wait for them to get the touch event and make this the first kal point
    while(1)
    {
        //Get touch events
        touch_get_events();
        
        if(has_been_pressed)
        {
            //They have started to press it. Read the XY position now
            points_stored[2] = touch_read_x();
            points_stored[3] = touch_read_y();
            
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
    printf("Got cal for point x=270 y=50, touched at x=%d y=%d\n",points_stored[2],points_Stored[3]);
    
    //Second point
    printf("Third point\n");
    GFXFillScreen(GFXPixelColor(0xFF,0x70,0x00));
    //Draw a pair of lines making a crosshairs, each 50px in from the edges
    GFXDrawFastVLine(50,165,50,GFXPixelColor(0,0,0));
    GFXDrawFastHLine(25,190,50,GFXPixelColor(0,0,0));
    //Wait for them to get the touch event and make this the first kal point
    while(1)
    {
        //Get touch events
        touch_get_events();
        
        if(has_been_pressed)
        {
            //They have started to press it. Read the XY position now
            points_stored[4] = touch_read_x();
            points_stored[5] = touch_read_y();
            
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
    printf("Got cal for point x=50 y=190, touched at x=%d y=%d\n",points_stored[4],points_Stored[5]);
    
    //Second point
    printf("Fourth point\n");
    GFXFillScreen(GFXPixelColor(0xFF,0x90,0x00));
    //Draw a pair of lines making a crosshairs, each 50px in from the edges
    GFXDrawFastVLine(270,165,50,GFXPixelColor(0,0,0));
    GFXDrawFastHLine(245,190,50,GFXPixelColor(0,0,0));
    //Wait for them to get the touch event and make this the first kal point
    while(1)
    {
        //Get touch events
        touch_get_events();
        
        if(has_been_pressed)
        {
            //They have started to press it. Read the XY position now
            points_stored[6] = touch_read_x();
            points_stored[7] = touch_read_y();
            
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
    printf("Got cal for point x=270 y=190, touched at x=%d y=%d\n",points_stored[6],points_Stored[7]);
    
    //Open a file for writing
    int fd = open("screen.kal",O_CREAT | O_WRONLY,S_IRUSR);
    
    //Points
    int point_x[] = {50, 270, 50, 270};
    int point_y[] = {50, 50, 190, 190};
    //Print the kal line
    for(int i = 0;i < 4;i++)
    {
        fprintf(fd,"X=%d Y=%d x=%d y=%d\n",point_x[i],point_y[i],points_stored[i*2]points_stored[i*2+1]);
        printf("X=%d Y=%d x=%d y=%d\n",point_x[i],point_y[i],points_stored[i*2]points_stored[i*2+1]);
    }
    
    //Close the file
    close(fd);

}