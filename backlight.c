//GPIO interface code
//Copyright 2014 Andrew Palardy

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <wiringpi.h>

//Setup the GPIO system
void gpio_init()
{
    //system call to config the gpio
    system("echo 252 > /sys/class/gpio/export");
    system("echo 'out' > /sys/class/gpio/gpio252/direction");
    
    //Turn off the backlight
    gpio_backlight_off();
    
    //Start wiring pi module
    WiringPiSetup();
    
    //Set the four button pins to INPUT
    pinMode(4,INPUT);
    pinMode(3,INPUT);
    pinMode(2,INPUT);
    pinMode(1,INPUT);
    
    //Set the pull ups
    pinUpDnControl(4,PUD_UP);
    pinUpDnControl(3,PUD_UP);
    pinUpDnControl(2,PUD_UP);
    pinUpDnControl(1,PUD_UP);
}
//Turn the backlight on
void gpio_backlight_on()
{
    system("echo '1' > /sys/class/gpio/gpio252/value");
}
//Turn the backlight off
void gpio_backlight_off()
{
    system("echo '0' > /sys/class/gpio/gpio252/value");
}


//Read the status of a GPIO button
void gpio_read_switch(char pin)
{
    if(pin < 5 && pin > 0)
    {
        //Pin is valid, read it's inverse value
        return !digitalRead(pin);
    }
    //Pin is not valid, return 0
    return 0;
}

//Close the GPIO system
void gpio_close()
{
    //Do nothing yet
}