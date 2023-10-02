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
// https://wokwi.com/projects/365067824797777921


#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 1
#define BUTTON_PIN_3 2
#define BUTTON_PIN_4 3

char currentText[5] = {0};

void HandleButtonClick(char *txt){
    _delay_ms(200);
    strcat(currentText, txt);
    lcd_set_cursor(0,1);
     lcd_puts(currentText);
     return;

    if(strlen(currentText)==4){
        if(!strcmp("1442",currentText)){
            lcd_printf("Correct welcome!");
        }else{
            lcd_printf("INCORRECT CODE");
            _delay_ms(3000);
            lcd_set_cursor(0,1);
            lcd_puts("               ");
            lcd_set_cursor(0,1);
        }
        currentText[0] = 0;
    }else{
          for(int i = 0;i <strlen(currentText);i++)          
            lcd_puts(currentText[i]);
    }
}

int main(void)
{
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_1);
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_2);
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_3);
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_4);


    millis_init();
    sei();

    lcd_init();
    lcd_enable_blinking();
    lcd_enable_cursor();

    
    lcd_puts("Skriv in koden:");

    while(1) {
        if(Button_Is_Clicked(PINB,BUTTON_PIN_1)) HandleButtonClick("1");
        if(Button_Is_Clicked(PINB,BUTTON_PIN_2)) HandleButtonClick("2");
        if(Button_Is_Clicked(PINB,BUTTON_PIN_3)) HandleButtonClick("3");
        if(Button_Is_Clicked(PINB,BUTTON_PIN_4)) HandleButtonClick("4");
    }
    return 0;
}