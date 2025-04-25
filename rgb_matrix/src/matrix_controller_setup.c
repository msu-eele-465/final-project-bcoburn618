#include "src/matrix_controller_setup.h"
#include "intrinsics.h"
#include "sys/cdefs.h"

void rgb_controller_init(void) {
  
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
}