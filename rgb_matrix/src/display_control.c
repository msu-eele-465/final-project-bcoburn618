#include <msp430.h>      
#include <stdbool.h>     
#include <stdint.h>      

#include "src/display_control.h"
#include "src/matrix_controller_setup.h"
#include "intrinsics.h"
#include "sys/cdefs.h"

void setRGBTop(bool r, bool g, bool b) {
    P3OUT &= ~(R1 | G1 | B1);
    if (r) P3OUT |= R1;
    if (g) P3OUT |= G1;
    if (b) P3OUT |= B1;
}

void setRGBBottom(bool r, bool g, bool b) {
    P3OUT &= ~(R2 | G2 | B2);
    if (r) P3OUT |= R2;
    if (g) P3OUT |= G2;
    if (b) P3OUT |= B2;
}

void pulseCLK(void) {
    P2OUT |= CLK;
    P2OUT &= ~CLK;
}

void latch(void) {
    P2OUT |= LAT;
    P2OUT &= ~LAT;
}

void enableDisplay(bool on) {
    if(on){
        P2OUT &= ~OE;
    }else{
        P2OUT |= OE;
    }
}

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



void displayET(char dial_in[3]) {
    uint8_t row, col;
    uint8_t i, j;
    uint8_t pixel;
    uint8_t currentCol;
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

const uint8_t fontDot[5] = {
    0x00, 0x00, 0x00, 0x01, 0x01
};

    
    // Disable display while writing
    enableDisplay(false);
    
    for (row = 0; row < 8; row++) { // 7 rows per character
        selectRow(row); // Select current row

        // For each column across the whole display
        for (col = 0; col < 64; col++) {

            // Figure out which part of the number we are in
            pixel = 0;

            if (col < 5) {
                // First digit
                currentCol = font5x7[dial_in[0]][col];
                pixel = (currentCol >> row) & 0x01;
            }
            else if (col >= 6 && col < 8) {
                // Decimal point is narrow (at col 6-10)
                currentCol = fontDot[col - 6];
                pixel = (currentCol >> row) & 0x01;
            }
            else if (col >= 9 && col < 14) {
                // Second digit
                currentCol = font5x7[dial_in[1]][col - 9];
                pixel = (currentCol >> row) & 0x01;
            }
            else if (col >= 15 && col < 20) {
                // Third digit
                currentCol = font5x7[dial_in[2]][col - 15];
                pixel = (currentCol >> row) & 0x01;
            }
            else {
                pixel = 0; // Empty space
            }

            // Set pixel color
            if (pixel) {
                setRGBTop(true, false, false);  // Red pixel ON
                setRGBBottom(true, false, false);  // Red pixel ON
            } else {
                setRGBTop(false, false, false);  // Pixel OFF
                setRGBBottom(false, false, false);  // Pixel OFF
            }

            // Shift to next column
            pulseCLK();
        }

        // After filling a row, latch it and display it
        latch();
        enableDisplay(true);
        __delay_cycles(1000); // Brief hold
        enableDisplay(false);
    }
}

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