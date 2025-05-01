#include "src/controller_control.h"
#include "intrinsics.h"
#include "sys/cdefs.h"

void controller_init(){
//----------------------------Pin Initialization--------------------------------
//--RGB LED (1=red, 2=green, 3=blue, 4=yellow)
    P4DIR |= BIT0;                              // Configure red part
    P4OUT &= ~BIT0;

    P2DIR |= BIT2;                              // Configure green part
    P2OUT &= ~BIT2;

    P2DIR |= BIT0;                              // Configure blue part
    P2OUT &= ~BIT0;

//--Columns
    P3DIR |= BIT4 | BIT5 | BIT6 | BIT7;        // Set columns as outputs
    P3OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);     // Initially set all low

//--Rows
    P3DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3);
    P3REN |= BIT0 | BIT1 | BIT2 | BIT3;
    P3OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);

    P6DIR |= BIT6;                             // Set P16 to output direction
    P6OUT |= BIT6;      // Clear P16 output latch for a defined power-on state
//---------------------Button ISR Initialization--------------------------------
    // Configure P1.3 as input with pull-up resistor
    P1DIR &= ~BUTTON;        // Set P1.3 as input
    P1REN |= BUTTON;         // Enable  pull-up/down resistor
    P1OUT |= BUTTON;         // Select pull-up (keeps pin high when button unpressed)

    // Configure interrupt
    P1IES |= BUTTON;         // high to low sensitivity
    P1IFG &= ~BUTTON;        // Clear interrupt flag
    P1IE  |= BUTTON;         // Enable interrupt for P1.3

   
//------------------------------- I2C Initialization -----------------------------    
    UCB1CTLW0 |= UCSWRST;                      // UCSWRST =1 for eUSCI_B1 in SW reset
	
//--Configure eUSCI_B1
    UCB1CTLW0 |= UCSSEL_3;                     // Choose BRCLK=SMCLK=1MHz
    UCB1BRW = 10;                              // Divide BRCLK by 10 for SCL=100kHz

    UCB1CTLW0 |= UCMODE_3;                     // Put into I2C mode
    UCB1CTLW0 |= UCMST;                        // Put into master mode
	UCB1CTLW0 |= UCTR;                         // Put into Tx mode

    UCB1CTLW1 |= UCASTP_2;                     // Auto STOP when UCB1TBCNT reached
	UCB1TBCNT = 3;                             // # of Bytes in Packet

//--Configure Ports SDA (P4.6) and SCL (P4.7)
    P4SEL1 &= ~(BIT6 | BIT7);
    P4SEL0 |= (BIT6 | BIT7);

//--Take eUSCI_B1 out of SW reset
    UCB1CTLW0 &= ~UCSWRST;

//--Enable Interrupts AFTER exiting reset
	UCB1IE |= UCTXIE0;                         // Enable I2C Tx0 IRQ
    __enable_interrupt();                      // Enable Maskable IRQs
}




