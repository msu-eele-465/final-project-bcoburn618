#ifndef KEYPAD_SCAN_H
#define KEYPAD_SCAN_H

#include <msp430.h>

// Functions
char scan_keypad(void);
char set_dial(char dial_in[3]);
int set_mode(void);
int set_color(void);
#endif
