# Class survey project
by: Joakim Englund, Daniel Örhill, Mohib Ali Akbari

# Table of Content
- Description
- Requirements
- File structure

## Description
Basic survey tool built with an ATmega328P MCU, three buttons (green, yellow, and red), an LCD screen and a secondary chip for wifi.\
The screen displays a question that can be answered by pressing any of the buttons.\
The data then gets uploaded to thingspeak where it can be analysed.

This is a school project where the biggest part of the goal was to learn about tools and hardware.±
Some of the things we learned:
* We learned more about bare-metal coding for Arduino
* It was the first time we used a secondary chip to handle WiFi
* It was the first time we used Wokwi, which was a nice simulator to work with, albeit a bit slow, and as far as we understand (and experienced); the free version has a limit on the WiFi, which meant adding more (and longer) delays than we would have used if we were working directly on the hardware
* It was the first time we used Thingspeak as a database, and it seems very useful for projects like this
* We also learned more about Makefiles as we dug into that in order to clean up the one that came with the avrwifi-repo and make it more easily understandable for us (it could still be better, but we're happy with the changes we managed to implement)

Based on [native-blink](https://github.com/aspcodenet/native-blink/) and [avrwifi (testing branch)](https://github.com/aspcodenet/avrwifi/tree/testing).

The project is simulated on [Wokwi](https://wokwi.com/projects/378185869128599553).

 ### Example:
Screen: How was class today?
```
Button1 = Good   Button2 = Okay   Button3 = Bad
```
The students would press one of the three buttons.

## Requirements
The requirements given to us when we got the assignment.
### Software
- C code preferably, but C++ is fine
- No GPL-licensed code/libraries

### Hardware
- ATmega328P preferably, but a full Arduino dev board or ESP32 is fine
- LCD or OLED Screen
- 3 buttons
- ESP8266 for Wifi

## File Structure
This project's codebase consists of 7 files plus a Makefile. Six of the files (ESP8266.c/.h, lcd.c/.h, and uart.c/.h) are basically used as libraries. They were included from existing projects, slightly modified, and we mostly sought to understand how they worked and what they were doing for the project. A lot of it could have been cleaned out as it was functionality that we didn't use, but we chose to keep them mostly intact in case some of that functionality would be useful down the road.

* ESP8266.c/.h contains functionality to initialize the chip, connect to WiFi and push data to thingspeak
* lcd.c/.h contains functionality to initialize the lcd screen and a bunch of commands that can be used to control how the screen funtions, including displaying text
* uart.c/.h contains funtionality to initialize the uart and serial communication, and sending and recieving characters from a stream. Used to get a link between the MCU and the standard C I/O, useful for debugging
* main.c contains the main program which sets up the GPIO-pins, LCD screen, and WiFi-chip and then goes to the main loop where it waits for button-presses, which it handles with a couple of helper-functions
