//Touchscreen interface code
//Copyright 2014 Andrew Palardy

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include "libevdev/libevdev.h"


//File globals
struct input_event *ev_temp;
int ev_fd;

void touch_init()
{
    
    ev_fd = open("/dev/input/touchscreen", O_RDONLY|O_NONBLOCK);
    
    //Do the ioctls to get the device info
    
    //Read all of the stuff from the kernel buffer??
    touch_get_events();
  
}

void touch_get_events()
{
    int len = 0;
    
    //Get all of the recent touch events from the kernel
    while(1)
    {
        //Infinite loops are cooler
        len = read(dev->fd, next, free_elem * sizeof(struct input_event));
        if (len < 0) {
            return;//End of queue
        } else if (len > 0 && len % sizeof(struct input_event) != 0)
            printf("TOUCH: Bad read from touch dev\n");
            return;//Bad stuffs
        else if (len > 0) {
            int nev = len/sizeof(struct input_event);
            //Do stuff with this???
        }
        
    }
}
