#include "src/display_control.h"
#include "src/matrix_controller_setup.h"
#include <msp430.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

volatile int Data_Cnt = 0;
volatile int RXDATA[MAX_PACKET_SIZE];
volatile int dial_in[3];  
volatile bool new_dial_received = false;
volatile bool display_status = true;
bool was_display_on;
bool r;
bool g;
bool b;


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    
                                            // to activate previously configured port settings
    rgb_controller_init();

    while(1)
    {
       
       while(display_status) {
            displayET(dial_in, true, true, true);
           
        }
      while(display_status == false) {
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
            RXDATA[Data_Cnt++] = UCB1RXBUF;  // Read received byte
            if(Data_Cnt == 3){
                Data_Cnt = 0;
                dial_in[0] = RXDATA[0];
                dial_in[1] = RXDATA[1];
                dial_in[2] = RXDATA[2];
                
            }else if(RXDATA[0] == 0xD){
                display_status = !display_status;  // Toggle boolean
                Data_Cnt = 0;
            }else if(RXDATA[0] == 0xC){
                Data_Cnt = 0;
                //color processing
            }

            break;

        case 0x12:  // UCSTPIFG: Stop condition detected
            UCB1IFG &= ~UCSTPIFG;  // Clear STOP flag
            break;

        default:
            break;
    
    
    }
}

