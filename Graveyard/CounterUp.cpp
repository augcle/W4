#include <Arduino.h>

void setup() {
    P1DIR |= (BIT2+BIT3+BIT4+BIT5); // Sets P1.2-P1.5 as output
}

void loop() {

        P1OUT |= BIT2; // Sets P1.2 to high, which i 1
        P1OUT &= ~(BIT3+BIT4+BIT5); // Sets the rest to low
        delay(1000);

        P1OUT |= (BIT3);
        P1OUT &= ~(BIT2+BIT4+BIT5);
        delay(1000);

        P1OUT |= (BIT3+BIT2);
        P1OUT &= ~(BIT4+BIT5);
        delay(1000);

        P1OUT |= (BIT4);
        P1OUT &= ~(BIT2+BIT3+BIT5);
        delay(1000);   
             
        P1OUT |= (BIT4+BIT2);
        P1OUT &= ~(BIT3+BIT5);
        delay(1000);

        P1OUT |= (BIT4+BIT3);
        P1OUT &= ~(BIT2+BIT5);
        delay(1000);

        P1OUT |= (BIT4+BIT3+BIT2);
        P1OUT &= ~(BIT5); 
        delay(1000);
        
        P1OUT |= (BIT5); 
        P1OUT &= ~(BIT2+BIT3+BIT4); 
        delay(1000);
        
        P1OUT |= (BIT5+BIT2); 
        P1OUT &= ~(BIT3+BIT4); 
        delay(1000);        

}