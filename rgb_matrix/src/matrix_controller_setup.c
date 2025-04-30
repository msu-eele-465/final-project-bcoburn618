#include "src/matrix_controller_setup.h"
#include "intrinsics.h"
#include "sys/cdefs.h"
#include "msp430.h"

void rgb_controller_init(void) {
//---------------------------Configure Clock-----------------------------------
    CSCTL0 = CSKEY;             // Unlock CS registers
    CSCTL1 = DCOFSEL_5;         // Set DCO to 24 MHz (DCOFSEL_5 = 24 MHz)
    CSCTL2 = SELM__DCOCLK | SELS__DCOCLK | SELA__REFOCLK; // MCLK & SMCLK = DCO, ACLK = REFO
    CSCTL3 = 0;                 // Set all dividers to 1 (no division)
    CSCTL0 = 0;                 // Lock CS registers
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
//----------------------------RGB Matrix Ports-----------------------------------
    P3OUT &= ~(R1 | G1 | B1 | R2 | G2 | B2);    //R1, G1, B1, R2, G2, B2)
    P3DIR |= (R1 | G1 | B1 | R2 | G2 | B2);

    P1OUT &= ~(A | B | C | D);                  //A, B, C, D (shift signals)
    P1DIR |= (A | B | C | D);

    P2OUT &= ~(LAT | CLK | OE);                 //Latch, Clock, and Color Control Signsl
    P2DIR |= (LAT | CLK | OE);

  
//------------------------------- I2C Initialization -----------------------------    
//--Put eUSCI_B0 into software reset to allow configuration
    UCB1CTLW0 |= UCSWRST;       

//--Configure eUSCI_B0 for I2C Slave mode
    UCB1CTLW0 |= UCMODE_3 | UCSYNC;         // Set I2C mode, synchronous operation
    UCB1I2COA0 = SLAVE_ADDR | UCOAEN;       // Set slave address & enable

//--Configure Ports for I2C SDA (P4.6) and SCL (P4.7)
    P4SEL1 &= ~(BIT6 | BIT7);               // Select primary module function for I2C
    P4SEL0 |= (BIT6 | BIT7);

//--Enable I2C Module
    UCB1CTLW0 &= ~UCSWRST;                  // Release eUSCI_B0 from reset

//--Enable I2C Interrupts
    UCB1IE |= UCRXIE0;                      // Enable I2C receive interrupt
    __bis_SR_register(GIE);                 // Enable global interrupts

//---------------------------------------------Status LED-----------------------------
    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction

}