#include <Arduino.h>

void setup() {
    P1DIR |= (BIT2, BIT3, BIT4, BIT5); // Sets P1.2-P1.5 as output
    P4DIR |= BIT7; // Sets P4.7 as an output (The right lamp)
    
}

void loop() {

P4OUT |= BIT7; // Turn on the right lamp
int counterUp = 0; // Sets out counter

while (1) {
    switch (counterUp) { // Sets the switch to act based on the value of counterUp
    case 0:
        P1OUT |= BIT2; // Sets P1.2 to high
        P1OUT &= ~(BIT3, BIT4, BIT5); // Sets the rest to low
        delay(1000); //Waits 1 second
        counterUp++; //Counts up by 1
        break;
    case 1:
        P1OUT |= BIT3;
        P1OUT &= ~(BIT2, BIT4, BIT5);
        delay(1000);
        counterUp=0; //Resets counter
        break;
    }
}


}