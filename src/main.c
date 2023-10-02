#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b)))) 

#define Set_Button_As_Input_Pullup(ddr,pdr,pin) BIT_CLEAR(ddr,pin);BIT_SET(pdr,pin);
#define Button_Is_Clicked(pinr,pin) !BIT_CHECK(pinr,pin)


// B (digital pin 8 to 13)
// C (analog input pins)
// D (digital pins 0 to 7)
// https://wokwi.com/projects/377477854928800769


#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 1
#define BUTTON_PIN_3 2

char displayText[17] = "Vad tycker du?"; // TODO: Change to 33 chars and use both rows, or use another display with more room for text.

void updateDisplayText(void){} // TODO: Enable setting displayText via mqtt or something. Check for bounds.

void HandleButtonClick(char *txt){ // TODO: Send the result to the cloud, thingspeak or something.
    _delay_ms(200);

    if (!strncmp(txt, "g", 1)){
        lcd_puts("Bra. Tack!");
    }else if (!strncmp(txt, "o", 1)){
        lcd_puts("Okej. Tack!");
    }else{ // !strncmp(txt, "b", 1)
        lcd_puts("Daligt. Tack!"); // TODO: Find a solution to handling Swedish characters.
    }
    _delay_ms(1500);
    lcd_set_cursor(0,1);
    lcd_puts("               ");
    lcd_set_cursor(0,1);
}

int main(void)
{
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_1);
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_2);
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_3);


    sei();

    lcd_init();
    lcd_enable_blinking();
    lcd_enable_cursor();

    updateDisplayText();
    lcd_puts(displayText);
    lcd_set_cursor(0,1);

    while(1) {
        if(Button_Is_Clicked(PINB,BUTTON_PIN_1)) HandleButtonClick("g"); // good
        if(Button_Is_Clicked(PINB,BUTTON_PIN_2)) HandleButtonClick("o"); // okay
        if(Button_Is_Clicked(PINB,BUTTON_PIN_3)) HandleButtonClick("b"); // bad
    }
    return 0;
}