
#include "intrinsics.h"
#include "src/keypad_scan.h"
#include "src/controller_control.h"
#include "src/lcd_control.h"
#include <msp430.h>

char dial_in[3];
int mode;
int board_state = 0;

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
        switch(mode){
            case 0xA: LCD_Clear();
                      LCD_print("Enter Dial In", 13);
                      set_dial(dial_in);
                      LCD_command(0x80);
                      LCD_print("Set Dial ", 9);
                      char dial_in_string[4];
                      dial_in_string[0] = dial_in[0] + '0';
                      dial_in_string[1] = '.';
                      dial_in_string[2] = dial_in[1] + '0';
                      dial_in_string[3] = dial_in[2] + '0';
                      LCD_print(dial_in_string, 4);
                      LCD_command(0xC0);
                      LCD_print("B=Send A=Reset", 14);
                      mode = 0;
                      __delay_cycles(200);
                      break;
            case 0xB: //I2C sending to slave
                     if(board_state == 0){
                        LCD_clear_second_line(16);
                        LCD_print("Board Off", 9);
                        break;
                      }else if(board_state == 1){
                        LCD_clear_second_line(16);
                        LCD_print("Board On", 8);
                      }break;

            case 0xD: if(board_state == 1){
                        LCD_clear_second_line(16);
                        LCD_print("Board Off", 9);
                        board_state = 0;
                        break;
                      }else if(board_state == 0){
                        LCD_clear_second_line(16);
                        LCD_print("Board On", 8);
                        board_state = 1;
                      }break;
            default:  break;
        }
    

    
        P6OUT ^= BIT6;                      // Toggle P1.0 using exclusive-OR
        __delay_cycles(100000);             // Delay for 100000*(1/MCLK)=0.1s
    }
}
