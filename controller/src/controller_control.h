/*
 * This function declares initializations for the controller*/

#ifndef CONTROLLER_CONTROL_H
#define CONTROLLER_CONTROL_H

#define MAX_PACKET_SIZE 3
#define SLAVE_ADDRESS 0x69
#define BUTTON  BIT3

#include <msp430.h>

// Function prototype
void controller_init();


#endif
