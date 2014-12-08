//GPIO interface code
//Copyright 2014 Andrew Palardy

//Setup the GPIO system
void gpio_init();
//Turn the backlight on
void gpio_backlight_on();
//Turn the backlight off
void gpio_backlight_off();
//Read the status of a GPIO button
void gpio_read_switch(char pin);
//Close the GPIO system
void gpio_close();