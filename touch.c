//Touchscreen interface code
//Copyright 2014 Andrew Palardy

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include "touch.h"


//File globals
int ev_fd;

//Current cursor position
unsigned char key_down;
int cur_x;
int cur_y;
int cur_prs;



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
    struct input_event ev_temp;
    
    //Get all of the recent touch events from the kernel
    while(1)
    {
        //Infinite loops are cooler
        len = read(ev_fd, &ev_temp, sizeof(struct input_event));
        if (len < 0)
        {
            return;//End of queue
        }
        else if (len > 0 && len % sizeof(struct input_event) != 0)
        {
            printf("TOUCH: Bad read from touch dev\n");
            return;//Bad stuffs
        }
        else if (len > 0)
        {
            //Number of events
            int nev = len/sizeof(struct input_event);
            //Do stuff with this???
            
            //struct input_event {
            //      struct timeval time;
            //      __u16 type;
            //      __u16 code;
            //      __s32 value;
            //      };
            
            struct timeval time_temp = ev_temp.time;
            
            //For each type of event
            if(ev_temp.type == EV_SYN)
            {
                //Event is a sync event, print it
                printf("TOUCH: Got a Sync event, X=%d, Y=%d, PRS=%d\n",cur_x,cur_y,cur_prs);
            }
            else if(ev_temp.type == EV_KEY)
            {
                //Event is a KEY event, print it
                //Is it a Touch event?
                if(ev_temp.code == BTN_TOUCH)
                {
                    printf("TOUCH: Got a Key event type TOUCH\n");
                }
                else
                {
                    printf("TOUCH: Got a Key event, code %d\n",ev_temp.code);
                }
            }
            else if(ev_temp.type == EV_ABS)
            {
                //Event is an ABS event, print it
                if(ev_temp.code == ABS_X)
                {
                    //printf("TOUCH: X position: %d\n",ev_temp.value);
                    cur_x = ev_temp.value;
                }
                else if(ev_temp.code == ABS_Y)
                {
                    //printf("TOUCH: Y position: %d\n",ev_temp.value);
                    cur_y = ev_temp.value;
                }
                else if(ev_temp.code == ABS_PRESSURE)
                {
                    //printf("TOUCH: PRESSURE %d\n",ev_temp.value);
                    cur_prs = ev_temp.value;
                }
                else
                {
                    printf("TOUCH: Got an unknown ABS event, code %d, value %d\n")
                }
            }
        }
        
    }
}
