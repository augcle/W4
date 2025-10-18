#include <Arduino.h>

void setup() {
    
    P6DIR &= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6); // Enables the P6.1-P6.6 to input
    P7DIR &= ~(BIT0); // Enables the P7.0 to input

    P6REN |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6); // Enables the resistor
    P7REN |= (BIT0); //Enables the resistor

    P6OUT &= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6); // Sets the resistor to pull-down, which enables active high
    P7OUT &= ~(BIT0); // Sets the resistor to pull-down, which enables active high
    
    P4DIR |= BIT7; // Sets P4.7 as an output (The right lamp)
    

}

void loop() {
    if ((P6IN&(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6)) || (P7IN&BIT0))
    {
        P4OUT |= BIT7; // Turns on the lamp
    }
    else
    {
        P4OUT &= ~(BIT7); // Turns off the lamp
    }
    
}