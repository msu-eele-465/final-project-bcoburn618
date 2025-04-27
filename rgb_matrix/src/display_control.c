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
    if (on) {
        P2OUT &= ~OE;
    } else {
        P2OUT |= OE;
    }
}

void selectRow(uint8_t row) {
    P1OUT &= ~(A | B | C | D);
    if (row & 0x01) P1OUT |= A;
    if (row & 0x02) P1OUT |= B;
    if (row & 0x04) P1OUT |= C;
    if (row & 0x08) P1OUT |= D;
}

void fillScreenRed(void) {
    uint8_t row;
    uint8_t col;
    for (row = 0; row < 16; row++) {  // rows 0-15
        enableDisplay(false); // Disable while shifting data

        selectRow(row); // Select the row

        for ( col = 0; col < 64; col++) {
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