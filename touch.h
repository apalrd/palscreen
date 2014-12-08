#ifndef _TOUCH_H
#define _TOUCH_H

//Get touch data
void touch_init();
void touch_get_events();
unsigned char touch_down();
int touch_get_x();
int touch_get_y();
int touch_get_prs();

#endif