#ifndef _TOUCHSTK_H
#define _TOUCHSTK_H


//Calling convention for callbacks:
//For events TD (touch down), LO (lift off), BTN (button):
//void event_function(int touch_id,void *user_ptr)
//For event DN (Down):
//void event_function(int touch_id,void *user_ptr,int xpos,int ypos,int pressure)

//Touch stack element struct
struct touch_stack_elem {
    //int bounds
    int x1;
    int x2;
    int y1;
    int y2;
    //Callback pointers
    void* evt_td;
    void* evt_lo;
    void* evt_dn;
    void* evt_btn;
    //User's pointer
    void* user_ptr;
    //Is Allocated
    uint8_t is_alloc;
    
};

//Touch stack functions
//Process buttons in the loop
void touch_stack_proc();
//Reset the touch mapping stack to re-draw
void touch_stack_reset();
//Delete a specific element off the stack
void touch_stack_free(int id);
//Allocate an element on the touch stack
int touch_stack_alloc(int x1, int x2, int y1, int y2, int usr_int, void* evt_td, void* evt_lo, void* evt_dn, void* evt_btn);
//Find the first element that is covered by the range
int touch_stack_find(int x, int y);
//Get x1, x2, y1, y2 for a given element id
int touch_stack_get_pos(int id, int* x1, int* x2, int* y1, int* y2, int* usr_int);

#endif