//Touchscreen interface code
//Copyright 2014 Andrew Palardy

#include <sys/types.h>
#include "typedefs.h"
#include <stdio.h>
#include "touchstk.h"
#include "touch.h"

//Touch stack (32 elements)
struct touch_stack_elem touch_stack_glob[32];

//Last touch down
int touched_last_id;

//Touch stack functions
//Process buttons in the loop
void touch_stack_proc()
{
    //Process the touch stack
    
    //New touched id defaults to none
    int new_id = -1;
    
    //Cache x and y since they have to do math internally
    int xpos = touch_get_x();
    int ypos = touch_get_y();
    
    //If we are touched down:
    if(touch_down())
    {
        //Get the first number that matches the current position
        new_id = touch_stack_find(xpos,ypos);
    }
    
    //If our new id is different from our old id
    if(new_id != touched_last_id)
    {
        //Was old ID a real button?
        if(touched_last_id >= 0)
        {
            //Yes it was, process lift off event every time the button is inactive
            //See if the lo event is mapped (not null)
            if(touch_stack_glob[touched_last_id].evt_lo != NULL)
            {
                touch_stack_glob[touched_last_id].evt_lo(touched_last_id,touch_stack_glob[new_id].user_ptr);
            }
            
            //If the new button is -1 (touch released), also process the button event
            //But only if the event is mapped
            if(new_id == -1 && touch_stack_glob[touched_last_id].evt_btn != NULL)
            {
                touch_stack_glob[touched_last_id].evt_btn(touched_last_id,touch_stack_glob[new_id].user_ptr);
            }
        }
        
        //Is the new button real?
        //Is the touch down event mapped?
        if(new_id >= 0 && touch_stack_glob[new_id].evt_td != NULL)
        {
            //Process the touch down event for that button
            touch_stack_glob[new_id].evt_td(new_id,touch_stack_glob[new_id].user_ptr,xpos,ypos,touch_get_prs());
        }
    }
    
    //If any button is pressed, process thd down event for it
    //But only if the event is mapped
    if(new_id >= 0 && touch_stack_glob[new_id].evt_dn != NULL)
    {
        touch_stack_glob[new_id].evt_dn(new_id,touch_stack_glob[new_id].user_ptr);
    }
    
    //Store the last id
    touched_last_id = new_id;
}

//Reset the touch mapping stack to re-draw
void touch_stack_reset()
{
    //Go through the whole touch stack and un-allocate it all
    for(int i = 0;i<32;i++)
    {
        //Unallocate this element of the glob stack
        touch_stack_glob[i].is_alloc=0;
    }
    
    //Reset touched last
    touched_last = 0;
}

//Delete a specific element off the stack
void touch_stack_free(int id)
{
    //Check the ID is in bounds
    if(id > 31 || id < 0)
    {
        printf("TOUCHSTK: Attempted to free bad id %d\n",id);
        return;
    }
    
    //Free it from the stack
    touch_stack_glob[id].is_alloc = 0;
}

//Allocate an element on the touch stack
int touch_stack_alloc(int x1, int x2, int y1, int y2, void* usr_ptr, void* evt_td, void* evt_lo, void* evt_dn, void* evt_btn)
{
    int free_id = -1;
    //Find the next free element on the stack
    for(int i = 0;i<32;i++)
    {
        //Check if this one is good
        if(!touch_stack_glob[i].is_alloc)
        {
            //Free! Try to use it
            free_id = i;
            break;
        }
    }
    //If we got this far and free_id is still 32, we didn't find a free one
    //Return 32 id (invalid)
    if(free_id == -1)
    {
        printf("TOUCHSTK: Could not find free element in stack\n");
        return -1;
    }
    
    //Setup the struct in the glob arr
    //Position
    touch_stack_glob[free_id].x1 = x1;
    touch_stack_glob[free_id].x2 = x2;
    touch_stack_glob[free_id].y1 = y1;
    touch_stack_glob[free_id].y2 = y2;
    //Callbacks
    touch_stack_glob[free_id].evt_td = evt_td;
    touch_stack_glob[free_id].evt_lo = evt_lo;
    touch_stack_glob[free_id].evt_dn = evt_dn;
    touch_stack_glob[free_id].evt_btn = evt_btn;
    //User Int
    touch_stack_glob[free_id].user_ptr = usr_ptr;
    //Is Allocated
    touch_stack_glob[free_id].is_alloc = 1;
    
    return free_id;
}
//Find the first element in range
int touch_stack_find(int x, int y)
{
    for(int i = 0;i < 32; i++)
    {
        //See if we are in the range
        if(touch_stack_glob[i].x1 <= x && touch_stack_glob[i].x2 >= x && touch_stack_glob[i].y1 <= y && touch_stack_glob[i].y2 >= y)
        {
            //Found it! return the number
            return i;
        }
    }
    //Return -1 if none found
    return -1;
}