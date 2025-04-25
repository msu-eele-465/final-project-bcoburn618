#ifndef MATRIX_CONTROLLER_SETUP_H
#define MATRIX_CONTROLLER_SETUP_H

#include <msp430.h>

#define SLAVE_ADDR 0x69
#define MAX_PACKET_SIZE 3

void rgb_controller_init(void);


#endif;