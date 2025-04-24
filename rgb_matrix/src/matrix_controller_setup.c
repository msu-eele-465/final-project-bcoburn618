#include "src/matrix_controller_setup.h"
#include "intrinsics.h"
#include "sys/cdefs.h"

void rgb_controller_init(void) {
  
    UCB1CTLW0 = UCSWRST;                         // Hold eUSCI_B1 in reset
    UCB1CTLW0 |= UCMODE_3 | UCSYNC;              // I2C mode, synchronous
    UCB1CTLW0 &= ~UCMST;                         // Slave mode
    UCB1I2COA0 = SLAVE_ADDRESS | UCOAEN;         // Own address, enable
    P4SEL0 |= BIT6 | BIT7;                       // SDA/SCL functionality
    P4SEL1 &= ~(BIT6 | BIT7);
    UCB1IE |= UCSTPIE | UCRXIE;                  // Enable STOP and RX interrupts
    UCB1CTLW0 &= ~UCSWRST;                       // Release from reset
    __enable_interrupt();                        // Enable maskable IRQs
}