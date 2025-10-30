#include <msp430.h>
#include <stdio.h>
#include "i2c.h"
#include "ssd1306.h"

void init_adc12(void) {
    ADC12CTL0 &= ~ADC12ENC;                  // Disable ADC for config
    ADC12CTL0 = ADC12SHT0_3 | ADC12ON;       // 32 ADC clock cycles sample, ADC on
    ADC12CTL1 = ADC12SHP;                    // Use sampling timer
    ADC12MCTL0 = ADC12INCH_0;                // Channel A0 (P6.0)
    P6SEL |= BIT0;                           // Set P6.0 to analog function
    P6REN &= ~BIT0;                          // Disable pull resistor
    ADC12CTL0 |= ADC12ENC;                   // Enable ADC conversions
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;                // Stop watchdog timer

    // Init OLED
    i2c_init();
    ssd1306_init();
    ssd1306_clearDisplay();

    init_adc12();

    char buffer[20];
    unsigned int adc_value = 0;
    unsigned int adc_percent = 0;

    while (1) {
        ADC12CTL0 |= ADC12SC;                // Start conversion
        while (ADC12CTL1 & ADC12BUSY);       // Vent til ADC er færdig
        adc_value = ADC12MEM0;               // Læs resultat (0–4095)
        adc_percent = (adc_value / 4095)*100;

        sprintf(buffer, "ADC12MEM0: %u", adc_value);
        ssd1306_printText(0, 0, buffer);

        sprintf(buffer, "Procent: %u%%", adc_percent);
        ssd1306_printText(0, 1, buffer);

        __delay_cycles(200000);              // Lille pause for læsbar opdatering
    }
}
