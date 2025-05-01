
/*
Final Project (RGB Digital Dial Board)
EELE 465
Beau Coburn
05/01/2025
Description:

This program allows the user to control a 64x32 matrix RGB display.  Utilizing the 4x4 keypad
the user can select between four diffent functionalites.  Pressing A, the user is prompted by 
a message on the LCD screen to enter a dial in.  A three digit number can be entered by pressing digits 0-9
on the keypad.  The system then prompts the user to either send the dial (via I2C) or choose a different dial.
If the user chooses to send the dial, the data will be set to I2C to the matrix controller to be displayed.
If the user selects C, they will then be prompted by the LCD screen to pick a color for the RGB display.  A
new color can be selected by pressing 0-6.  This data will then be passed to the matrix controller for it to
be processed.  The color of the screen at the current moment will be displayed in the lower right corner of the
LCD screen.  The user can then press D which will toggle the state of the RGB display by sending a flag over
I2C.  The state of the board is also shown on the LCD screen and can be seen in the lower left corner.  
The status of the RGB display can also be controlled by pressing an external button.
*/
#include "intrinsics.h"
#include "msp430fr2355.h"
#include "src/keypad_scan.h"
#include "src/controller_control.h"
#include "src/lcd_control.h"
#include "src/rgb_control.h"
#include <msp430.h>
#include <string.h>

char dial_in[3];
int mode;
int board_state = 1;
int Data_Cnt;
volatile char Packet[MAX_PACKET_SIZE];
int color_index;
const char *colors[7] = {"Red", "Green", "Blue", "Yellow", "Cyan", "Purple", "White"};

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    
    P6OUT &= ~BIT6;                         // Clear P1.0 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P1.0 to output direction

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    controller_init();
    LCD_init();
    LCD_setup();

    while(1)
    {
        mode = set_mode();
        rgb_control(2);                                         //set Green to indicate proper operation
        switch(mode){
            case 0xA: LCD_Clear();
                      LCD_print("Enter Dial In", 13);
                      set_dial(dial_in);                        // prompt user for three digits (dial in)
                      LCD_command(0x80);                        //move cursor to first space on first line
                      LCD_print("Set Dial ", 9);
                      //Convert dial_in to ascii to be printed
                      char dial_in_string[4];
                      dial_in_string[0] = dial_in[0] + '0';
                      dial_in_string[1] = '.';
                      dial_in_string[2] = dial_in[1] + '0';
                      dial_in_string[3] = dial_in[2] + '0';
                      LCD_print(dial_in_string, 4);
                      LCD_command(0xC0);                        // move cursor to first space of second line
                      LCD_print("B=Send A=Reset", 14);
                      mode = 0;                                 // stop re-execution
                      __delay_cycles(200);
                      break;
            case 0xB:  //Send dial_in to slave to be displayed on matrix
                        UCB1I2CSA = 0x0069; 
                        Packet[0] = dial_in[0];
                        Packet[1] = dial_in[1];
                        Packet[2] = dial_in[2]; 
                        Data_Cnt = 0;                            //ensure count is zero
                        UCB1TBCNT = 3;                           // set packet length to 3
                        UCB1CTLW0 |= UCTR;                       // Transmitter mode
                        UCB1IE |= UCTXIE0;                       // Enable TX interrupt
                        UCB1CTLW0 |= UCTXSTT;                    // Start transmission
                        rgb_control(3);                          // set blue for valid transaction
                        __delay_cycles(20000);

                        //Check Current Board State
                     if(board_state == 0){
                        LCD_clear_second_line(16);
                        LCD_print("Board Off", 9);
                        break;
                      }else if(board_state == 1){
                        LCD_clear_second_line(16);
                        LCD_print("Board On", 8);
                      }
                        clear_for_color();                      //clear previous color
                        LCD_print(colors[color_index], strlen(colors[color_index]));
                      break;
            
            case 0xC:   LCD_clear_first_line(16);
                        LCD_print("Select Color", 12);
                        color_index = set_color();
                        UCB1I2CSA = 0x0069; 
                        Packet[0] = 0xC;
                        Packet[1] = color_index;
                        Data_Cnt = 0;                            //ensure count is zero
                        UCB1TBCNT = 2;                           // set packet length to 3
                        UCB1CTLW0 |= UCTR;                       // Transmitter mode
                        UCB1IE |= UCTXIE0;                       // Enable TX interrupt
                        UCB1CTLW0 |= UCTXSTT;                    // Start transmission
                        rgb_control(3);                          // set blue for valid transaction
                        __delay_cycles(20000);
                        clear_for_color();
                        LCD_print(colors[color_index], strlen(colors[color_index]));
                        LCD_clear_first_line(16);
                        LCD_print("Set Dial ", 9);
                        LCD_print(dial_in_string, 4);
                        mode = 0;
                        break;

            case 0xD:   UCB1I2CSA = 0x0069;
                        Packet[0] = 0xD;
                        Data_Cnt = 0;                           //ensure count is zero 
                        UCB1TBCNT = 1;                          //set packet length to 1
                        UCB1CTLW0 |= UCTR;                      // Transmitter mode
                        UCB1IE |= UCTXIE0;                       // Enable TX interrupt
                        UCB1CTLW0 |= UCTXSTT;                   //Start transmission
                         rgb_control(3);                          // set blue for valid transaction
                        __delay_cycles(20000);
                        //Update Board State
                        if(board_state == 1){                   //check current board state
                        LCD_clear_second_line(16);
                        LCD_print("Board Off", 9);
                        clear_for_color();
                        LCD_print(colors[color_index], strlen(colors[color_index]));
                        board_state = 0;                        //next board state
                        break;
                      }else if(board_state == 0){               //check current board state
                        LCD_clear_second_line(16);
                        LCD_print("Board On", 8);
                        clear_for_color();
                        LCD_print(colors[color_index], strlen(colors[color_index]));
                        board_state = 1;                        //next board state
                      }break;   
            default:  break;
        }
    

    
        P6OUT ^= BIT6;                      // Toggle P1.0 using exclusive-OR
        __delay_cycles(100000);             // Delay for 100000*(1/MCLK)=0.1s
    }
}

//------------------Interrupt Service Routines----------------------------------
//-----------------------I2C Send ISR-------------------------------------------
#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void) {
    if(Data_Cnt < sizeof(Packet)) {
        UCB1TXBUF = Packet[Data_Cnt++];  
    }else{
         Data_Cnt = 0;
         UCB1IE &= ~UCTXIE0;        // Disable TX interrupt
         UCB1CTLW0 |= UCTXSTP;      // Send STOP condition
    }
}
//----------------------------Button ISR---------------------------------------
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    
    UCB1I2CSA = 0x0069;                     // slave address
    Packet[0] = 0xD;
    Data_Cnt = 0;                           // ensure count is zero 
    UCB1TBCNT = 1;                          // set packet length to 1
    UCB1CTLW0 |= UCTR;                      // Transmitter mode
    UCB1IE |= UCTXIE0;                      // Enable TX interrupt
    UCB1CTLW0 |= UCTXSTT;                   // Start transmission
    rgb_control(3);                         // set blue for valid transaction
    __delay_cycles(20000);
    //Update Board State
    switch(board_state){
    
        case 0:  LCD_clear_second_line(16);
                 LCD_print("Board On", 8);
                 clear_for_color();
                 LCD_print(colors[color_index], strlen(colors[color_index]));       // reprint current color
                 board_state = 1;                        //next board state
                break;

        case 1:  LCD_clear_second_line(16);
                 LCD_print("Board Off", 9);
                 clear_for_color();
                 LCD_print(colors[color_index], strlen(colors[color_index]));      // reprint current color
                 board_state = 0;                        //next board state
                break;
        
        default: 
                break;
    }
   
    P1IFG  &= ~BUTTON;
}