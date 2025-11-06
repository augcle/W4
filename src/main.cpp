#include <Arduino.h>
#include "ssd1306.h"
#include "i2c.h"
#include <msp430.h>
#include <msp430f5529.h>

volatile unsigned int adc_value = 0;

void init_timerA1(void) {
    TA1CTL = TASSEL_2 | MC_1 | TACLR; // Tassel 2 er clock-kilden "Sub main clock" som kører på 1 MHz. MC1 står for timer mode, og 1 betyder up. Som timeren skal tælle op fra 0. TACLR betyder timer clear.
    TA1CCR0 = 999; // Den skal tælle op til 999 clock-cyklusser før den resetter.
    TA1CCR1 = 500; // Dette er vores initial 'duty cycle', på 50%. Det betyder at vi tæller op til 500, som er halvdelen af vores frekvens på 1000. 
    TA1CCTL1 = OUTMOD_7; // Den her bruger vi for at fortæller, at vi bruger output mode 7. Det betyder vi resetter selve outputtet ved CCR1, og resetter timeren ved CCR0. 
}

void init_timerA0(void) {
    TA0CTL = TASSEL_2 | MC_1 | TACLR; // Vi enabler vores nummer 2 timer i systemet, TA0
    TA0CCR0 = 999; // Sætter også dens goal til at være 999
    TA0CCTL0 = CCIE; // Det her enabler vores interrupt på timeren A0.  
}

void init_adc12(void) {
    ADC12CTL0 &= ~ADC12ENC;                       // tillad konfig
    ADC12CTL0  = ADC12SHT0_3 | ADC12ON;           // 32 ADC clk sample-tid
    ADC12CTL1  = ADC12SHP;                        // brug sample-timer
    ADC12MCTL0 = ADC12INCH_0;                     // kanal A0 (P6.0)
    ADC12IE    = ADC12IE0;                        // enable interrupt for MEM0. Det er den her linje det giver os, at når der ligger et resultat klar i mem, så kommer der et interrupt. 
    P6SEL     |= BIT0;                            // analog funktion
    P6REN     &= ~BIT0;                           // ingen pull-resistor
    ADC12CTL0 |= ADC12ENC;                        // enable conversion
}

// TimerA0 Compare ISR. Det er er det der faktisk sker hver eneste gang A0 er interruptet. Så hver 999 svingninger. 
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void) {
    ADC12CTL0 |= ADC12SC;  // Start ADC conversion, og implicit gemmer vi den i MEM0. 
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void) {
    unsigned int iv = ADC12IV;
    if (iv == 6) {  // ADC12IFG0
        adc_value = ADC12MEM0;
        // ← her opdaterer vi PWM'en direkte, så der er minimal forsinkelse:
        TA1CCR1 = ((unsigned long)adc_value * TA1CCR0) / 4095;
    }
}

unsigned int duty = 0;

int main() {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer. Det skal vi bare. 

    // Skærmopsætning
    i2c_init();
    ssd1306_init();
    ssd1306_clearDisplay();

    init_timerA1();
    init_timerA0();
    init_adc12();

    // Portopsætning
    P2DIR |= BIT0;      // Sæt P2.0 som output
    P2SEL |= BIT0;      // Sæt P2.0 til dens PWM funktion

    
    __enable_interrupt();

    char buffer[20];

    while(1){
        
    duty = ((unsigned long)TA1CCR1 * 100) / TA1CCR0;    

        sprintf(buffer, "TA1R:%u", TA1R);   // Konverter tal til tekst
        ssd1306_printText(0, 0, buffer);       // Skriv på linje 0 (fx øverste linje)

        sprintf(buffer, "CCR1:%03u", TA1CCR1);
        ssd1306_printText(0, 1, buffer);       // Linje 1

        sprintf(buffer, "Duty:%03u%%", duty);
        ssd1306_printText(0, 2, buffer);       // Linje 2

        sprintf(buffer, "ADC:%04u", adc_value);
        ssd1306_printText(0, 3, buffer);       // Linje 3

        __delay_cycles(200000);
    }

}