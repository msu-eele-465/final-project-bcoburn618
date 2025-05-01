#include <msp430.h>      
#include <stdbool.h>     
#include <stdint.h>      

#include "src/display_control.h"
#include "src/matrix_controller_setup.h"
#include "intrinsics.h"
#include "sys/cdefs.h"

/*
The RGB screen is divided in half and controlled via top and bottom. This function
controls the color of the top half of the screen by checking the value of inputs r, g, and b
corresponding to red, green, and blue.  The combination of these colors are what is seen on the screen
*/
void setRGBTop(bool r, bool g, bool b) {
    P3OUT &= ~(R1 | G1 | B1);
    if (r) P3OUT |= R1;
    if (g) P3OUT |= G1;
    if (b) P3OUT |= B1;
}
/*
This fucntion is the same as setRGBTop but control the bottom half of the RGB screen.
*/
void setRGBBottom(bool r, bool g, bool b) {
    P3OUT &= ~(R2 | G2 | B2);
    if (r) P3OUT |= R2;
    if (g) P3OUT |= G2;
    if (b) P3OUT |= B2;
}
/*
The pulsing of the clock allows for the pit loaded into the register to be shifted onto the
screen preperation to be displayed
*/
void pulseCLK(void) {
    P2OUT |= CLK;
    P2OUT &= ~CLK;
}
/*
The latch function will latch the data that has been passed in for an entire row and it will then
lock it in place for that row before the screen is set to display
*/
void latch(void) {
    P2OUT |= LAT;
    P2OUT &= ~LAT;
}

/*
The enable display turns the display on when the output is asserted and off when it is low.
This allows for the data pushed into the shift registers to be displayed for that row then it is
turned off in preperation for the next row
*/
void enableDisplay(bool on) {
    if(on){
        P2OUT &= ~OE;
    }else{
        P2OUT |= OE;
    }
}
/*
The selectRow function will select a row address for the row that is currently being set.
This function is updated each every scan cycle meaning each time a new row is ready to receive
data.
*/
void selectRow(uint8_t row) {
    P1OUT &= ~(A | B | C | D);
    if(row & 0x01){
    P1OUT |= A;
    }
    if(row & 0x02){
    P1OUT |= B;
    }
    if(row & 0x04){
    P1OUT |= C;
    }
    if(row & 0x08){
    P1OUT |= D;
    }
}
/*
This function simplay passes no data into the shift registers meaning that
all the pixels within the row and display as a whole will be blank.  This 
function is utilized when the user desires to have the dial board turned off
*/
void clearScreen(void)
{
    uint8_t row, col;

    for (row = 0; row < 16; row++) {
        selectRow(row);
        enableDisplay(false);
        for (col = 0; col < 64; col++) {
            setRGBTop(false, false, false);
            setRGBBottom(false, false, false);
            pulseCLK();
        }
        latch();
    }
}


/*
The displayET fuction takes in a three digit dial along with the 
three color values R, G, and B.  Based on the input of the dial in
the fucntion will pull information from the font map and push it into
the rows for the dial into be displayed on the RGB screen.  The colors
will then be passed into the setRGBTop function within this function
that dictates the color of the screen.
*/
void displayET(int dial_in[3], bool r, bool g, bool b) {
    uint8_t row, col;
    uint8_t pixel;
    uint8_t currentCol;

    // 5x7 font data for digits 0–9
    const uint8_t font5x7[][5] = {
        {0x7E, 0x81, 0x81, 0x81, 0x7E}, // 0
        {0x00, 0x82, 0xFF, 0x80, 0x00}, // 1
        {0xE2, 0x91, 0x91, 0x91, 0x8E}, // 2
        {0x42, 0x81, 0x89, 0x89, 0x76}, // 3
        {0x1C, 0x12, 0x11, 0xFF, 0x10}, // 4
        {0x4F, 0x89, 0x89, 0x89, 0x71}, // 5
        {0x7E, 0x89, 0x89, 0x89, 0x72}, // 6
        {0x01, 0xE1, 0x11, 0x09, 0x07}, // 7
        {0x76, 0x89, 0x89, 0x89, 0x76}, // 8
        {0x4E, 0x91, 0x91, 0x91, 0x7E}  // 9
    };

    // Dot character used as a decimal point between digits
    const uint8_t fontDot[5] = {
        0x00, 0x80, 0x00, 0x00, 0x00
    };

    // Disable display output while shifting in new data
    enableDisplay(false);

    // Loop through each of the 8 visible rows (top half of display)
    for (row = 0; row < 8; row++) {
        selectRow(row);  // Set row address lines (A-D)

        // Loop through all 64 columns of the display
        for (col = 0; col < 64; col++) {
            pixel = 0;

            // First digit (columns 0–4)
            if (col < 5) {
                currentCol = font5x7[dial_in[0]][col];
                pixel = (currentCol >> row) & 0x01;

            // Dot character (columns 6–7)
            } else if (col >= 6 && col < 8) {
                currentCol = fontDot[col - 6];
                pixel = (currentCol >> row) & 0x01;

            // Second digit (columns 9–13)
            } else if (col >= 9 && col < 14) {
                currentCol = font5x7[dial_in[1]][col - 9];
                pixel = (currentCol >> row) & 0x01;

            // Third digit (columns 15–19)
            } else if (col >= 15 && col < 20) {
                currentCol = font5x7[dial_in[2]][col - 15];
                pixel = (currentCol >> row) & 0x01;

            // Empty space in all other columns
            } else {
                pixel = 0;
            }

            // Set color for current pixel (top half only)
            if (pixel) {
                setRGBTop(r, g, b);  // Turn on RGB color
            } else {
                setRGBTop(false, false, false);  // Turn off pixel
            }

            pulseCLK();  // Shift current pixel into the column buffer
        }

        latch();               // Latch column data into output register
        enableDisplay(true);   // Briefly enable the display to show this row
        __delay_cycles(1000);  // Short delay for visibility (adjust as needed)
        enableDisplay(false);  // Turn display off before moving to next row
    }
}

/*
This function simply makes the whole screen red. (was used for testing)
*/
void fillScreenRed(void) {
    uint8_t row;
    uint8_t col;
    for (row = 0; row < 16; row++) {  // rows 0-15
        enableDisplay(false); // Disable while shifting data

        selectRow(row); // Select the row

        for (col = 0; col < 64; col++) {
            // Set top pixel (R1 ON, G1 OFF, B1 OFF)
            setRGBTop(true, false, false);

            // Set bottom pixel (R2 ON, G2 OFF, B2 OFF)
            setRGBBottom(true, false, false);

            // Shift data for current pixel
            pulseCLK();
        }

        // Latch all 64 pixels into place
        latch();

        // Turn on display for this row
        enableDisplay(true);

        // Hold it briefly so it’s visible
        __delay_cycles(1000);  

        // Turn display off before next row
        enableDisplay(false);
    }
}







