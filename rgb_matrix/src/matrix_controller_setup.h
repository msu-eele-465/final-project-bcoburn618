#ifndef MATRIX_CONTROLLER_SETUP_H
#define MATRIX_CONTROLLER_SETUP_H

#include <msp430.h>
//----------------RGB Matrix Ports------------------
//Port 3
#define R1  BIT0    
#define G1  BIT1    
#define B1  BIT2
#define R2  BIT3
#define G2  BIT4
#define B2  BIT5
//Port 1
#define A   BIT4
#define B   BIT5
#define C   BIT6
#define D   BIT7
//Port 2
#define LAT  BIT0
#define CLK  BIT1
#define OE   BIT2
//--------------------IR Receiver----------------------
#define IR   BIT3

#define SLAVE_ADDR 0x69
#define MAX_PACKET_SIZE 3
//------------------------Clock Definitions for 24 MHz-------------------------------------
#define CSKEY   0xA500              // Key to unlock CS registers (whole CSCTL0 register)
#define DCOFSEL_5   0x05            // 24 MHz (nominal)
#define SELM__DCOCLK 0x0100         // MCLK source select DCOCLK (bits 8-6)
#define SELS__DCOCLK 0x0010         // SMCLK source select DCOCLK (bits 4-2)
#define SELA__REFOCLK 0x0000        // ACLK source select REFOCLK (default)

//---------------------Functions-------------------------------------------------
void rgb_controller_init(void);


#endif;