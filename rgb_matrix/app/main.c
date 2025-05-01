/*
Final Project (RGB Digital Dial Board)
EELE 465
Beau Coburn
05/01/2025
Description:

This program allows for the RGB matrix display to be driven and controlled.
The programs functionality is dictated by data received over I2C sent from
the main controller.  If the the program receives a packet that is of length 
three, it will take the data received and pass it into an array known as dial in.
This dial is then passed into a function that will display the ET and will remain doing
so until a new dial is received or the board is turned off.  If the program receives
a packet that is two bytes long with the first element being 0xC, the second element
will contain a digit that corresponds to a color.  This digit will then be decoded corresponding
to a color.  If the first element of the I2C transaction is 0xD, the the display state will be toggled.
*/

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
volatile int color;
volatile bool r = true;
volatile bool g = false;
volatile bool b = false;


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    
                                            // to activate previously configured port settings
    rgb_controller_init();

    while(1)
    {
       
       while(display_status) {
            //set color based on user input
            switch(color){
                case 0x0:   r = true; g = false; b = false;         // red
                            break;
                case 0x1:   r = false; g = true; b = false;         // green
                            break;  
                case 0x2:   r = false; g = false; b = true;         // blue
                            break;
                case 0x3:   r = true; g = true; b = false;          // yellow
                            break;
                case 0x4:   r = false; g = true; b = true;          // cyan
                            break;
                case 0x5:   r = true; g = false; b = true;          // magenta/purple
                            break;
                case 0x6:   r = true; g = true; b = true;           // white
                            break;
                default:    r = true; g = false; b = false;         // default color is red
                            break;
            }
            displayET(dial_in, r, g, b);
           
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
            RXDATA[Data_Cnt++] = UCB1RXBUF;                 // Read received byte
            if(Data_Cnt == 3){                              // check a dial in has been sent
                Data_Cnt = 0;                               // reset count for next transmission
                //move RXDATA for dial in           
                dial_in[0] = RXDATA[0];
                dial_in[1] = RXDATA[1];
                dial_in[2] = RXDATA[2];
                
            }else if(RXDATA[0] == 0xD){                     // Check if board state needs to change
                display_status = !display_status;           // Toggle state of board
                Data_Cnt = 0;                               // reest count for next transmission
            }else if(RXDATA[0] == 0xC && Data_Cnt == 2){    // check if new color has been sent
                color = RXDATA[1];                          // move color index to color for switch case
                Data_Cnt = 0;                               // reset count ffor next transmission
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

