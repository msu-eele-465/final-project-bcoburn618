#include "src/matrix_controller_setup.h"
#include <msp430.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

volatile int Data_Cnt = 0;
volatile int RXDATA[MAX_PACKET_SIZE];



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

   WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
    
    

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    rgb_controller_init();

    while(1)
    {
        fillScreenRed();
        P6OUT ^= BIT6;                      // Toggle P1.0 using exclusive-OR
        __delay_cycles(100000);             // Delay for 100000*(1/MCLK)=0.1s
    }
}
//--------------Interupt Service Routines--------------------------------------------
//--------------------------I2C Receive ISR------------------------------------------
#pragma vector = EUSCI_B1_VECTOR
__interrupt void EUSCI_B1_ISR(void)
{
    switch (__even_in_range(UCB1IV, 0x1E)) {
        case 0x16:  
            RXDATA[Data_Cnt++] = UCB1RXBUF;  // Read received byte

            break;

        case 0x12:  // UCSTPIFG: Stop condition detected
            UCB1IFG &= ~UCSTPIFG;  // Clear STOP flag
            break;

        default:
            break;
    
    
    }
}