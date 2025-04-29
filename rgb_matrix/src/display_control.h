#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

 #define PIXEL_WIDTH 3

#include "intrinsics.h"
#include "sys/cdefs.h"
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

void setRGBTop(bool r, bool g, bool b);
void setRGBBottom(bool r, bool g, bool b);
void pulseCLK(void);
void latch(void);
void enableDisplay(bool on);
void selectRow(uint8_t row);
void clearScreen(void);
void fillScreenRed(void);
void displayET(int dial_in[3], bool r, bool g, bool b);




#endif