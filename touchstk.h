#ifndef _TOUCHSTK_H
#define _TOUCHSTK_H

//Touch stack element struct
struct touch_stack_elem {
    //int bounds
    int x1;
    int x2;
    int y1;
    int y2;
    //Callback pointers
    void *evt_td;
    void *evt_lo;
    void *evt_dn;
    void *evt_btn;
    //Is Allocated
    uint8_t is_alloc;
};

//Touch stack functions
//Process buttons in the loop
void touch_stack_proc();
//Reset the touch mapping stack to re-draw
void touch_stack_reset();
//Delete a specific element off the stack
void touch_stack_free(char id);
//Allocate an element on the touch stack
char touch_stack_alloc(int x1, int x2, int y1, int y2, void *evt_td, void *evt_lo, void *evt_dn, void *evt_btn);
//Find the first element that is covered by the range
char touch_stack_find(int x, int y)

#endif