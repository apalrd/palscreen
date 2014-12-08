/*
Copyright 2014 Andrew Palardy

PalScreen user interface for PiTFT
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "palscreen.h"
#include "gfx.h"
#include "touch.h"
#include "backlight.h"
#include "touchstk.h"

//Calling convention for callbacks:
//For events TD (touch down), LO (lift off), BTN (button):
//void event_function(int touch_id,void *user_ptr)
//For event DN (Down):
//void event_function(int touch_id,void *user_ptr,int xpos,int ypos,int pressure)

//Callbacks
void evt0_td(int touch_id,void *user_ptr)
{
    printf("TOUCH DOWN PRIMARY id=%d\n",touch_id);
}
void evt1_td(int touch_id,void *user_ptr)
{
    printf("TOUCH DOWN SECONDARY id=%d\n",touch_id);
}
void evt0_lo(int touch_id,void *user_ptr)
{
    printf("LIFT OFF PRIMARY id=%d\n",touch_id);
}
void evt1_lo(int touch_id,void *user_ptr)
{
    printf("LIFT OFF SECONDARY id=%d\n",touch_id);
}
void evt0_btn(int touch_id,void *user_ptr)
{
    printf("BUTTON PRIMARY id=%d\n",touch_id);
}
void evt1_btn(int touch_id,void *user_ptr)
{
    printf("BUTTON SECONDARY id=%d\n",touch_id);
}
void evt0_dn(int touch_id,void *user_ptr,int xpos,int ypos,int pressure)
{
    printf("DOWN PRIMARY id=%d x=%d y=%d prs=%d\n",touch_id,xpos,ypos,pressure);
}
void evt1_dn(int touch_id,void *user_ptr,int xpos,int ypos,int pressure)
{
    printf("DOWN SECONDARY id=%d x=%d y=%d prs=%d\n",touch_id,xpos,ypos,pressure);
}

//Main
int main()
{
    //Init GFX
    printf("GFX init\n");
    GFXInit();
    printf("GFX init complete!\n");
    
    printf("Touch init\n");
    touch_init();
    printf("Touch reading kal\n");
    touch_read_kal();
    printf("Touch init complete!\n");
    
    printf("Backlight init\n");
    backlight_init();
    printf("Backlight init complete!\n");
    
    //Turn on the backlight
    backlight_on();


    
    //Draw in a color
    GFXFillScreen(0xFFFF);
    
    //Setup at leat 2 touch events
    touch_stack_alloc(0,100,0,100,NULL,evt0_td,evt0_lo,evt0_dn,evt0_btn);
    touch_stack_alloc(190,290,140,240,NULL,evt1_td,evt1_lo,evt1_dn,evt1_btn);
    
    while(1)
    {
        //Process the touch events
        touch_get_events();
        touch_stack_proc();
        //Sleep for a bit (50ms)
        usleep(50000);
    }

}