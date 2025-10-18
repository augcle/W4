#include <Arduino.h>
#include "i2c.h"
#include "ssd1306.h"

void setup() {

    /* This program is a simple calculator, that takes 3 switches binary value, convert it into a number and displays the result*/ 

    //Used for setting up the display
    i2c_init();
    ssd1306_init();
    reset_diplay();
    ssd1306_clearDisplay();
    // ssd1306_printText(0, 0, "Hello World");
    
    //Setting up the ports
    P6DIR &= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6); // Enables the P6.1-P6.6 to input
    P7DIR &= ~(BIT0); // Enables the P7.0 to input

    P6REN |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6); // Enables the resistor
    P7REN |= (BIT0); //Enables the resistor

    P6OUT &= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6); // Sets the resistor to pull-down, which enables active high
    P7OUT &= ~(BIT0); // Sets the resistor to pull-down, which enables active high

    P1DIR &= ~(BIT1); // Sets P1.1 to input (The contact right)
    P1REN |= (BIT1); // Enables the resistor 
    P1OUT |= (BIT1); // Selects pull-up, which means it is active low

    P2DIR &= ~(BIT1); // Sets P2.1 to input (The contact left)
    P2REN |= (BIT1); // Enables the resistor 
    P2OUT |= (BIT1); // Selects pull-up, which means it is active low

}

int buttonConfirm1(); // Function used to test P1.1 for floating contact
int buttonConfirm2(); // Function used to test P2.1 for floating contact

uint8_t dip_value; // The integer that holds the dip-switch values
uint8_t firstVal = 0; // Place to store the first value of the dip-switch
uint8_t secondVal = 0; // Place to store the second value of the dip-switch

int numState = 0; // The state cycle for our numbering. It goes input 1 -> input 2 -> operation -> result.
int calState = 0; // Used to safe the operation to do. It goes addition -> subtraction -> multiplication -> division.

char data[16]; // Just a buffer array

void loop(){ // The main loop
    
    switch (numState) { // Starts of in the num state
        ssd1306_clearDisplay();
    case 0: // Input 1
        dip_value = P6IN & (BIT0+BIT1+BIT2); // Get the value of the dip-switch
        sprintf(data, "Input1:%03d", dip_value); // Format the value
        ssd1306_printText(0, 0, data);  // Display the value
        if (buttonConfirm1()) { // If P1.1 is pressed, cycle to input 2
            ssd1306_clearDisplay(); // Clears the display
            firstVal = dip_value; // Stores the value of the dip
            numState=1; // The actualt 'go-to-next' command
        }
    break;
    case 1: // Input 2
        dip_value = P6IN & (BIT0+BIT1+BIT2); 
        sprintf(data, "Input2:%03d", dip_value);
        ssd1306_printText(0, 0, data);
        if (buttonConfirm1()) { // If P1.1 is pressed, go to operation.
            ssd1306_clearDisplay();
            secondVal = dip_value;
            numState=2;
        }
    break;
    case 2: // Operation mode. Choose what math to do.
        switch (calState) {
            case 0: // Addition
                sprintf(data, "%03d + %03d", firstVal, secondVal); // Format what is calculated
                ssd1306_printText(0, 0, data); // Display it
                    if (buttonConfirm2()) { // If P2.1 is pressed, go to the next math operation
                    ssd1306_clearDisplay(); // Clears the display
                    calState=1;} // The actualt 'go-to-next' command
                    if (buttonConfirm1()){ // Alternatively, if the P1.1 is pressed, go to the results.
                        ssd1306_clearDisplay();
                        numState = 3;
                    }
                break;
            case 1: // Subtraction
                sprintf(data, "%03d - %03d", firstVal, secondVal);
                ssd1306_printText(0, 0, data);
                    if (buttonConfirm2()) {
                    ssd1306_clearDisplay();
                    calState=2;}
                    if (buttonConfirm1()){
                        ssd1306_clearDisplay();
                        numState = 3;
                    }
                break; 
            case 2: // Multiplication
                sprintf(data, "%03d * %03d", firstVal, secondVal);
                ssd1306_printText(0, 0, data);
                    if (buttonConfirm2()) {
                    ssd1306_clearDisplay();
                    calState=3;}
                    if (buttonConfirm1()){
                        ssd1306_clearDisplay();
                        numState = 3;
                    }
                break;  
            case 3: // Division
                sprintf(data, "%03d / %03d", firstVal, secondVal);
                ssd1306_printText(0, 0, data);
                    if (buttonConfirm2()) {
                    ssd1306_clearDisplay();
                    calState=0;} // Cycle back to addiition.
                    if (buttonConfirm1()){
                        ssd1306_clearDisplay();
                        numState = 3;
                    }
                break;
        }
    break;
    case 3: // Results
        switch (calState) { // Depending on what state that the calculator was in, do one of the following
        case 0: // Addition
            sprintf(data, "Result: %03d", firstVal+secondVal);
            ssd1306_printText(0, 0, data);
            if (buttonConfirm1()) { // If P1.1 is pressed, reset everything, and start over. 
                ssd1306_clearDisplay();
                firstVal = 0;
                secondVal = 0; 
                numState=0;
                calState=0;
            }
            break;
        case 1: // Subtraction
            sprintf(data, "%03d", firstVal-secondVal);
            ssd1306_printText(0, 0, data);
            if (buttonConfirm1()) {
                ssd1306_clearDisplay();
                firstVal = 0;
                secondVal = 0; 
                numState=0;
                calState=0;
            }
            break;
        case 2: // Multiplication
            sprintf(data, "%03d", firstVal*secondVal);
            ssd1306_printText(0, 0, data);
            if (buttonConfirm1()) {
                ssd1306_clearDisplay();
                firstVal = 0;
                secondVal = 0; 
                numState=0;
                calState=0;
            }
            break;
        case 3: // Division
            sprintf(data, "%03d", firstVal/secondVal);
            ssd1306_printText(0, 0, data);
            if (buttonConfirm1()) {
                ssd1306_clearDisplay();
                firstVal = 0;
                secondVal = 0; 
                numState=0;
                calState=0;
            }
            break;
        }
    break;
    }
    delay(100); // Waits 0.1 second 
}

int buttonConfirm1(){ // For P1.1. To avoid floating swtiches. This double-checks in the button is stille pressed after 50ms, before sending a 1 back.
    if ((P1IN & BIT1)==0) {
        delay(50);
        if ((P1IN & BIT1)==0) {
            return 1;
        }
    }
    return 0;
}

int buttonConfirm2(){ // For P2.1
    if ((P2IN & BIT1)==0) {
        delay(50);
        if ((P2IN & BIT1)==0) {
            return 1;
        }
    }
    return 0;
}