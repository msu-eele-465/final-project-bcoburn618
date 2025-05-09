#ifndef LCD_CONTROL_H
#define LCD_CONTROL_H

#include <msp430.h>

#define RS  BIT5
#define EN  BIT4
#define D7  BIT7
#define D6  BIT6
#define D5  BIT5
#define D4  BIT4





// Functions
void LCD_setup();                       //initializes ports
void LCD_init();                        // LCD initialization 
void toggle_enable();                   //toggling enable line
void LCD_command(unsigned char cmd);    //sends commands to screen
void LCD_Clear();                       // clears screen
void LCD_write(unsigned char message);  //sends charcaters to screen
void LCD_clear_first_line(int spaces);
void LCD_print(const char *word, int length);
void LCD_clear_second_line(int spaces);
void clear_for_color();


#endif