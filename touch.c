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
int cur_gain_x;
int cur_offs_x;
int cur_gain_y;
int cur_offs_y;
unsigned char kal_done;



void touch_init()
{
    
    ev_fd = open("/dev/input/touchscreen", O_RDONLY|O_NONBLOCK);
    
    //One gain
    cur_gain_x = 0x00FF;
    cur_gain_y = 0x00FF;
    
    //Zero offset
    cur_offs_x = 0;
    cur_offs_y = 0;
    
    //Kal not done
    kal_done = 0;
    
    //Read all of the stuff from the kernel buffer??
    touch_get_events();
  
}

//Read calibration from kal file
//Note: We can't do this in init since we also use this code for kalibrate
void touch_read_kal()
{
    //Fopen the kal file
    FILE *kal = fopen("screen.kal","r");
    
    int rtn = 0;
    
    int gain_temp = 0;
    int offs_temp = 0;
    
    char buf[64];
    
    printf("TOUCH: Opened kal file screen.kal\n");
    
    //Fscanf in a while loop to get meaningful data
    while(1)
    {
        //Read the next line in the file
        rtn = fgets(&buf,64,kal);
        //Did we reach the end of file?
        if(feof(kal))
        {
            printf("TOUCH: Kal reached end of file\n");
            break;
        }
        //Try scanning the X line
        rtn = sscanf(buf,"LINE X gain=%d offs=%d\n",&gain_temp,&offs_temp);
        //Did we get it?
        if(rtn != 0)
        {
            //Got it! Continue
            cur_gain_x = gain_temp;
            cur_offs_x = offs_temp;
            printf("TOUCH: Got X gain=%d offs=%d\n",cur_gain_x,cur_offs_x);
            kal_done = 1;
            continue;
        }
        
        //Nope, maybe try Y line
        rtn = sscanf(buf,"LINE Y gain=%d offs=%d\n",&gain_temp,&offs_temp);
        //Did we get it?
        if(rtn != 0)
        {
            //Got it! Continue
            cur_gain_y = gain_temp;
            cur_offs_y = offs_temp;
            printf("TOUCH: Got Y gain=%d offs=%d\n",cur_gain_y,cur_offs_y);
            kal_done = 1;
            continue;
        }
        
        //Neither but the line was long enough
        printf("TOUCH: Kal found a bad line\n");
    }
    
    fclose(kal);
}

unsigned char touch_down()
{
    return key_down;
}

int touch_get_x()
{
    //Return scaled x - note that touchscreen is inverted
    if(!kal_done)
    {
        return cur_y;
    }
    //Use cals
    int temp = cur_y - cur_offs_x;
    temp = (temp * cur_gain_x) >> 8;
    return temp;
}
int touch_get_y()
{
    //Return scaled y - note that touchscreen is inverted
    if(!kal_done)
    {
        return cur_x;
    }
    //Use cals
    int temp = cur_x - cur_offs_y;
    temp = (temp * cur_gain_y) >> 8;
    return temp;
}
int touch_get_prs()
{
    //Return raw pressure
    return cur_prs;
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
            //int nev = len/sizeof(struct input_event);
            //Do stuff with this???
            
            //struct input_event {
            //      struct timeval time;
            //      __u16 type;
            //      __u16 code;
            //      __s32 value;
            //      };
            
            //For each type of event
            if(ev_temp.type == EV_SYN)
            {
                //Event is a sync event, print it
                printf("TOUCH: Got a Sync event, X=%d, Y=%d, PRS=%d\n",cur_x,cur_y,cur_prs);
                //At SYNC events, check pressure=0 for key up/down
                if(cur_prs)
                {
                    //Key is down
                    key_down = 1;
                }
                else
                {
                    //Key must be up
                    key_down = 0;
                }
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
                    printf("TOUCH: Got an unknown ABS event, code %d, value %d\n",ev_temp.code,ev_temp.value);
                }
            }
        }
        
    }
}
