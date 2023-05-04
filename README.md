# Simoné
3.007 Design Thinking and Innovation Project

![-6273701404518494801_121](https://user-images.githubusercontent.com/81726240/236242987-be57f8b7-8c14-4e7c-9357-81b1b73a56ae.jpg)

To find out more about the problem statement and our product
http://asd.courses.sutd.edu.sg/dti-teams/51242-2/

Below will be the explanation for the code which involves the use of an arudino mega microcontroller in C++.

The arduino mega controls 
1. speaker volume 
2. sd card module
3. WS2812B led strips of light

While doing the C++ code for this project, I learnt several concepts that were crucial to the success of the project.
1. multithreading 
2. memory management

Multithreading in Arduino
Multithreading was required as at the start of the game, in order for players to log in, they will be required to press all 4 buttons to simulate a player 'lock in'. During this process, all the led lights will start blinking. When 1 button is pressed, for example the red button, all the red lights which is one section of the lights on the led strip will stop blinking and remain as a red light. While this red light remains on, the other 3 sections of the led lights are simulatenously blinking. In order to do this, line 111 to line 225 of the code will implement this concept. 

Due to arudino being single threaded, we had to figure a work around this. We implemented multithreading by using the function millis(). This gives the illusion of the arudino doing multiple things at once when in reality it is still doing things within a single thread. Below is a snippet of the code.
unsigned long previousTimeREDLEDBlink = 0;
int BlinkDelay = 400;
if ( (timeNow - previousTimeYELLOWLEDBlink > BlinkDelay) && (yellowledblink == 1 || yellowledblink == 0) ) {
        previousTimeYELLOWLEDBlink += BlinkDelay;
        if (yellowledblink == 1) {
          yellowledblink = 0;
          fill_solid( ledarray[2], 60, CRGB::Yellow);
          FastLED.show();         
        } 
        else {
          yellowledblink = 1;
          fill_solid( ledarray[2], 60, CRGB::Black);
          FastLED.show();
          }
      }
So here, what we are doing is we create a timeline for the 4 different lights to keep blinking every 400ms, then this code here will check the current timing and check if the button has been pressed. If it has not been pressed the lights will keep blinking. Here is a link for more in depth explanation.
https://forum.arduino.cc/t/how-to-make-2-leds-blink-at-the-same-time/366237/11


Installation
1. simone.cpp

download the file and copy the code to a .ino file. 
This is the arduino official IDE. https://www.arduino.cc/en/software
run the file in a arudino mega.
