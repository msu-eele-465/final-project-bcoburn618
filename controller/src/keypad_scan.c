/*----------------------------------------------------------------------------------
Beau Coburn & Aaron Foster
EELE 465
2/25/25
Project 3: Keypad LED Pattersn
Description:  




led_pattern
    The scan_keypad logic is used to scan in a single digit input from the keypad.  When a key is pressed
    it will be returned to the main code which will use it to set the desired LED pattern on the off board
    LED bar display.  This functionality is also used to return the input (A or B) which changes the speed
    at which the LED pattern is displayed.  Additionaly, this is used to detect when D is pressed to reset the sytem.
    Logic was used to decode the ascii character to a decimal value so that the desired value was valid within the LED
    pattern module

Pin Out
--------------
Rows : P3.0, P3.1, P3.2, P3.3 
Cols : P3.4, P3.5, P3.6, P3.7

//This line was added to fix git pushing
-------------------------------------------------------------------------------------*/
#include <msp430.h> 
#include "rgb_control.h"
 
// Keypad mapping
char keymap[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
/*
    This function scans the input of the key pad by setting the rows as inputs and the columns as outputs.
    The first column is set high and all the rows are checked to see if both the row and column are high corresponding
    to a key press.  If no key is detected on the first column, the logic will continue and the next row set high
    till a key press is detected.  A value will only be returned if a key is pressed and will only be sent when the
    key is released.  
*/
char scan_keypad(void){
    unsigned int row, col;
    const char row_pin[4] = {BIT0, BIT1, BIT2, BIT3};
    const char col_pin[4] = {BIT4, BIT5, BIT6, BIT7};
         
    // Loop through all the columns
    for(col = 0; col < 4; col++) {
        // Set all columns low
        P3OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);  // Set all columns low
        
        // Set the current column to HIGH
        P3OUT |= col_pin[col];                             // Set the current column high

        // Check each row to see if any row is low (indicating a key press)
        for(row = 0; row < 4; row++) {
            if((P3IN & (row_pin[row])) != 0) {               // If the row is pulled low
            //P1OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);  // Set all columns low
                // Wait for key release to avoid multiple detections
                while((P3IN & (row_pin[row])) != 0);
                return keymap[row][col];                    // Return the key from the keymap
            }
        }
    }

    return 0; // No key pressed

}
/*
This function allows the user to enter three digits from the keypad that
will be used as the dial sent to the RGB screen and displayed on the
LCD screen.  It loops through scan_keypad function three times, then 
decodes the ascii characters returned to hex.
*/
    void set_dial(char dial_in[3]) {
    int i = 0;
    char key = 0;
    int digit;

    while (i < 3) {
        key = scan_keypad();  
        if (key != 0) {             // Only decode if a key was pressed
            rgb_control(4);         // show key has been pressed
            //Decode ascii
            if (key >= '0' && key <= '9') {
                digit = key - '0';
            } else {
                switch (key) {
                    case 'A': digit = 0xA; break;
                    case 'B': digit = 0xB; break;
                    case 'C': digit = 0xC; break;
                    case 'D': digit = 0xD; break;
                    case '*': digit = 14; break;
                    case '#': digit = 15; break;
                    default: break;  // Skip invalid keys
                }
            }

            dial_in[i] = digit;
            i++;
        }
    }
}
/*
The set_mode function allows the user to choose a mode for the dial_board system.
The user is able to choose one value which should be A, B, C, or D.  This is done by 
using the scan_keypad function and the returned value is decoded from its ascii value 
to a hex value.
*/
int set_mode(void) {
    char key = scan_keypad();  // Check keypad once
    int digit = -1;

    if (key != 0) {
        // Decode ASCII to digit
        if (key >= '0' && key <= '9') {
            digit = key - '0';
        } else {
            switch (key) {
                case 'A': digit = 0xA; break;
                case 'B': digit = 0xB; break;
                case 'C': digit = 0xC; break;
                case 'D': digit = 0xD; break;
                case '*': digit = 14; break;
                case '#': digit = 15; break;
                default: digit = -1; break;  // Invalid key
            }
        }
    }

    return digit;  // -1 if no key or invalid key
}
/*
This function utilizes the scan keypad function to allow
the user to select a number 0-6 corresponding to a color
that the RGB display can produce.  The value is returned 
from the scan_keypad function, decoded from ascii to hex
and returned to be sent to the RGB display and the color
shown on the LCD screen
*/
int set_color(void) {
    char key;
    int digit = -1;
while(digit == -1){
    key = scan_keypad();
    if (key != 0) {
        // Decode ASCII to digit
        if (key >= '0' && key <= '9') {
            digit = key - '0';
        } else {
            switch (key) {
                case 'A': digit = 0xA; break;
                case 'B': digit = 0xB; break;
                case 'C': digit = 0xC; break;
                case 'D': digit = 0xD; break;
                case '*': digit = 14; break;
                case '#': digit = 15; break;
                default: digit = -1; break;  // Invalid key
            }
        }
    }
}
    return digit;  // -1 if no key or invalid key
}