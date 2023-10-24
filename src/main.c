#define F_CPU 16000000UL

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "ESP8266.h"
#include "lcd.h"

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL << (b))))

#define Set_Button_As_Input_Pullup(ddr, pdr, pin) BIT_CLEAR(ddr, pin); BIT_SET(pdr, pin);
#define Button_Is_Clicked(pinr, pin) !BIT_CHECK(pinr, pin)

// B (digital pin 8 to 13)
// C (analog input pins)
// D (digital pins 0 to 7)
// https://wokwi.com/projects/378185869128599553

#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 1
#define BUTTON_PIN_3 2

// TODO: Change to 33 chars and use both rows, *or* use another display with more room for text
char displayText[17] = "Vad tycker du";
char displayText2[17] = "om lektionen?";

// TODO: Enable setting displayText and lesson id via mqtt or something (check for bounds)
void updateDisplayText(void) {}

void puts_displayText(void)
{
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_puts(displayText);
    lcd_set_cursor(0, 1);
    lcd_puts(displayText2);
}

void fetchAndSend(int field)
{
    int data = 1000;        // Placeholder, field1 is supposed to contain data about what lesson it is (a lesson-id if you will)
    addData("field1", data);
    data = (field == 2) ? 1 : 0;
    addData("field2", data);
    data = (field == 3) ? 1 : 0;
    addData("field3", data);
    data = (field == 4) ? 1 : 0;
    addData("field4", data);

    pushData();             // Push data to Thingspeak
}

void HandleButtonClick(int field)
{
    _delay_ms(200);

    lcd_clear();
    lcd_set_cursor(0, 0);

    if (field == 2)         // Green button pressed
    {
        lcd_puts("Bra.");
    }
    else if (field == 3)    // Yellow button pressed
    {
        lcd_puts("Okej.");
    }
    else                    // Red button pressed (field == 3)
    {
        lcd_puts("Dalig."); // TODO: Find a solution to handle Swedish characters
    }
    lcd_set_cursor(0, 1);
    lcd_puts("Skickar data.");
    fetchAndSend(field);
    puts_displayText();
}

void initializeLcd()
{
    lcd_init();
    lcd_enable_blinking();
    lcd_enable_cursor();
    lcd_puts("Starting:");
}

int main(void)
{
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_1);
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_2);
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_3);

    initializeLcd();
    _delay_ms(1000);        // Delay on startup

    init_serial();
    lcd_set_cursor(0, 1);
    lcd_puts("RESET          ");
    ESPinit();              // Setup ESP8266

    sei();

    puts_displayText();     // Print the question

    while (1)
    {
        if (Button_Is_Clicked(PINB, BUTTON_PIN_1))
            HandleButtonClick(2);                   // Green
        if (Button_Is_Clicked(PINB, BUTTON_PIN_2))
            HandleButtonClick(3);                   // Yellow
        if (Button_Is_Clicked(PINB, BUTTON_PIN_3))
            HandleButtonClick(4);                   // Red
    }
}