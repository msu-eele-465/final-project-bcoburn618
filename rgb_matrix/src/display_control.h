#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

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
void fillScreenRed(void);



#endif