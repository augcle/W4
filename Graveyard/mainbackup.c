#include <Arduino.h>
#include "i2c.h"
#include "ssd1306.h"

void setup() {

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
    
    P4DIR |= BIT7; // Sets P4.7 as an output (The right lamp)


}

uint8_t dip_value;
char data[8];
int counter;

void loop(){
    P4OUT &= ~BIT7;
    dip_value = P6IN & (BIT0+BIT1+BIT2); //
    ssd1306_clearDisplay(); // Clears the display
    sprintf(data, "%08d", dip_value); // Formats the array data to contain 8 zeroes by default and the data from dip-values
    ssd1306_printText(0, 0, data);
    delay(2000); // Waits two second 
    
    if ((P1IN & BIT1)==0)
    {
        P4OUT |= BIT7;
        ssd1306_clearDisplay();
        delay(2000);
    }
    


}



/*

*/

#include <Arduino.h>
#include "i2c.h"
#include "ssd1306.h"

void setup() {

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
    
    P4DIR |= BIT7; // Sets P4.7 as an output (The right lamp)


}

uint8_t dip_value;
char data[16];
int counter = 0;

void loop(){
    /*
    P4OUT &= ~BIT7;
    dip_value = P6IN & (BIT0+BIT1+BIT2); //
    ssd1306_clearDisplay(); // Clears the display
    sprintf(data, "%08d", dip_value); // Formats the array data to contain 8 zeroes by default and the data from dip-values
    ssd1306_printText(0, 0, data);
    delay(2000); // Waits two second 
    
    if ((P1IN & BIT1)==0)
    {
        P4OUT |= BIT7;
        ssd1306_clearDisplay();
        delay(2000);
    }
    */
    
    switch (counter) {
    case 0:
        ssd1306_clearDisplay(); // Clears the display  
        dip_value = P6IN & (BIT0+BIT1+BIT2); 
        sprintf(data, "Input:%08d", dip_value);
        ssd1306_printText(0, 0, data);
        if ((P1IN & BIT1)==0) {
            counter=1;
        }
        
        delay(100); // Waits 0.1 second 
        break;
    case 1:
        ssd1306_clearDisplay(); // Clears the display
        ssd1306_printText(0, 0, "Jello World!");
        if ((P1IN & BIT1)==0) {
            counter=0;
        }
        delay(100); // Waits 0.1 second 
        break;
    }

}



/*

*/

#include <Arduino.h>
#include "i2c.h"
#include "ssd1306.h"

void setup() {

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
    
    P4DIR |= BIT7; // Sets P4.7 as an output (The right lamp)


}

int buttonConfirm();

uint8_t dip_value;
char data[16];
int state = 0;
uint8_t firstVal = 0;
uint8_t secondVal = 0;

void loop(){
    /*
    P4OUT &= ~BIT7;
    dip_value = P6IN & (BIT0+BIT1+BIT2); //
    ssd1306_clearDisplay(); // Clears the display
    sprintf(data, "%08d", dip_value); // Formats the array data to contain 8 zeroes by default and the data from dip-values
    ssd1306_printText(0, 0, data);
    delay(2000); // Waits two second 
    
    if ((P1IN & BIT1)==0)
    {
        P4OUT |= BIT7;
        ssd1306_clearDisplay();
        delay(2000);
    }
    */
    
    switch (state) {
        ssd1306_clearDisplay();
    case 0:
        dip_value = P6IN & (BIT0+BIT1+BIT2); 
        sprintf(data, "Input1:%03d", dip_value);
        ssd1306_printText(0, 0, data);
        if (buttonConfirm()) {
            ssd1306_clearDisplay();
            firstVal = dip_value;
            state=1;
        }
        break;
    case 1:
        dip_value = P6IN & (BIT0+BIT1+BIT2); 
        sprintf(data, "Input2:%03d", dip_value);
        ssd1306_printText(0, 0, data);
        if (buttonConfirm()) {
            ssd1306_clearDisplay();
            secondVal = dip_value;
            state=2;
        }
        break;
    case 2:
        sprintf(data, "Diff:%03d", abs(secondVal - firstVal));
        ssd1306_printText(0, 0, data);
        if (buttonConfirm()) {
            ssd1306_clearDisplay();
            firstVal = 0;
            secondVal = 0; 
            state=0;
        }
        break;
    }
    delay(100); // Waits 0.1 second 
}

int buttonConfirm(){
    if ((P1IN & BIT1)==0) {
        delay(50);
        if ((P1IN & BIT1)==0) {
            return 1;
        }
    }
    return 0;
}