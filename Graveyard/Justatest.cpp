#include <Arduino.h>
#include "i2c.h"
#include "ssd1306.h"

void setup() {

    //Used for setting up the display
    i2c_init();
    ssd1306_init();
    reset_diplay();
    ssd1306_clearDisplay();
    
    //Setting up the ports
    P6DIR &= ~(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6); // P6.0-P6.6 inputs
    P7DIR &= ~(BIT0); // P7.0 input

    P6REN |= (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6); // enable resistors on P6
    P7REN |= (BIT0);

    P6OUT &= ~(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6); // pull-down on P6 (active high)
    P7OUT &= ~(BIT0);

    P1DIR &= ~(BIT1); // P1.1 input (button)
    P1REN |= (BIT1); // enable resistor
    P1OUT |= (BIT1); // pull-up (active low)
    
    P4DIR |= BIT7; // P4.7 output (lamp)
}

uint8_t readDip() {
    // Read bits 0..6 from P6 and return as integer (0..127)
    const uint8_t mask = (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6);
    return (uint8_t)(P6IN & mask); // bits already in place, treat as value
}

char buf[32];
int state = 0; // 0 = set first, 1 = set second, 2 = show sum
uint8_t firstVal = 0;
uint8_t secondVal = 0;
uint8_t prevButton = 1; // P1.1 pull-up => idle = 1

void showPromptForState() {
    ssd1306_clearDisplay();
    switch (state) {
        case 0:
            sprintf(buf, "Set #1: %d", readDip());
            ssd1306_printText(0, 0, buf);
            ssd1306_printText(0, 16, "Press btn to store");
            break;
        case 1:
            sprintf(buf, "Set #2: %d", readDip());
            ssd1306_printText(0, 0, buf);
            ssd1306_printText(0, 16, "Press btn to store");
            break;
        case 2:
            {
                uint16_t sum = (uint16_t)firstVal + (uint16_t)secondVal;
                sprintf(buf, "Sum: %u", sum);
                ssd1306_printText(0, 0, buf);
                ssd1306_printText(0, 16, "Press btn to reset");
            }
            break;
    }
}

void loop(){
    // Read current button state (1 = not pressed, 0 = pressed)
    uint8_t curButton = (P1IN & BIT1) ? 1 : 0;

    // Update live display of dip while waiting
    showPromptForState();

    // Detect falling edge (not-pressed -> pressed)
    if (prevButton == 1 && curButton == 0) {
        // debounce
        delay(50);
        uint8_t confirm = (P1IN & BIT1) ? 1 : 0;
        if (confirm == 0) {
            // button confirmed pressed
            switch (state) {
                case 0:
                    firstVal = readDip();
                    state = 1;
                    break;
                case 1:
                    secondVal = readDip();
                    state = 2;
                    break;
                case 2:
                    // reset to allow new sequence
                    firstVal = 0;
                    secondVal = 0;
                    state = 0;
                    break;
            }
            // short visual feedback: toggle lamp
            P4OUT |= BIT7;
            delay(80);
            P4OUT &= ~BIT7;
        }
        // wait until release to avoid multiple counts
        while ((P1IN & BIT1) == 0) {
            delay(10);
        }
    }

    prevButton = curButton;
    delay(100); // main loop pacing
}