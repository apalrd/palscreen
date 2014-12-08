//GPIO interface code
//Copyright 2014 Andrew Palardy

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "backlight.h"

//Backlight time since backlight-on event
time_t backlight_last_time;
uint8_t backlight_stat;
uint8_t backlight_expire;

//Setup the backlight
void backlight_init()
{
    //system call to config the gpio
    system("echo 252 > /sys/class/gpio/export");
    system("echo 'out' > /sys/class/gpio/gpio252/direction");
    
    //Backlight is already off
    backlight_stat = 0;
    
    //Turn on the backlight and poke it
    backlight_poke();
    
    //Set the expire time to 30sec
    backlight_expire = 30;
}
//Turn the backlight on
void backlight_on()
{
    system("echo '1' > /sys/class/gpio/gpio252/value");
    backlight_stat = 1;
}
//Turn the backlight off
void backlight_off()
{
    system("echo '0' > /sys/class/gpio/gpio252/value");
    backlight_stat = 0;
}

//Poke the backlight so it stays on
void backlight_poke()
{
    //Poke the backlight so we keep it on if necessary
    time(&backlight_last_time);
    
    //IS the backlight on? Otherwise turn it on
    if(!backlight)
    {
        //Turn it on immediately
        backlight_on();
    }
}

void backlight_check()
{
    //Check to see if the backlight has been poked recently
    //or if we should turn it on/off
    
    //Is the backlight on?
    if(backlight)
    {
        //Check the delta t
        time_t cur_time;
        time(&cur_time);
        
        //Get diff time
        int delta_t = difftime(&backlight_last_time,&cur_time);
        
        //Is diff time old enough to turn it off?
        if(delta_t >= backlight_expire)
        {
            //Turn the backlight off
            backlight_off();
        }
    }
    //Otherwise the backlight is off
    else
    {
        //Do nothing, poking the backlight will turn it back on
    }
}