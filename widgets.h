#ifndef _WIDGETS_H
#define _WIDGETS_H

//GUI Widgets

//Struct for button data
struct widget_data_btn {
    int x1;
    int x2;
    int y1;
    int y2;
    char *text;
    void *cbk
    int user_int;
    uint16_t color_fg;
    uint16_t color_bg;
};

//Clear screen to a color
void widget_clear_screen(uint16_t color);
//Draw a button with overlay text and a pressed callback
//Colors fg is the 'drawn' color, bg is the 'pressed' color
//Text is drawn using the opposite colors (bg normally, fg when pressed)
//BG color is always used to draw a box around the element, FG when pressed
void widget_draw_btn(int x1, int x2, int y1, int y2, uint16_t color_fg, uint16_t color_bg, void *cbk, char *text);

#endif