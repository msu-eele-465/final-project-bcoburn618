#include "src/matrix_controller_setup.h"
#include <msp430.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

volatile int Data_Cnt = 0;
volatile int RXDATA[MAX_PACKET_SIZE];
volatile char dial_in[3];  
volatile bool new_dial_received = false;




int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    
                                            // to activate previously configured port settings
    rgb_controller_init();

    while(1)
    {
       
        if(dial_in[0] != 0xD && dial_in[0] != 0){
           displayET(dial_in);
        }
        else{
            clearScreen();
        }
        
    }
}
//--------------Interupt Service Routines--------------------------------------------
//--------------------------I2C Receive ISR------------------------------------------
#pragma vector = EUSCI_B1_VECTOR
__interrupt void EUSCI_B1_ISR(void)
{
    switch (__even_in_range(UCB1IV, 0x1E)) {
        case 0x16:  
            dial_in[Data_Cnt++] = UCB1RXBUF;  // Read received byte
            if(Data_Cnt == 3){
                Data_Cnt = 0;
            }else if(dial_in[0] == 0xD){
                Data_Cnt = 0;
            }

            break;

        case 0x12:  // UCSTPIFG: Stop condition detected
            UCB1IFG &= ~UCSTPIFG;  // Clear STOP flag
            break;

        default:
            break;
    
    
    }
}