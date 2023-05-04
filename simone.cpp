#include "FastLED.h"
#define Data_Pin 7 //lights switch
#define chipset WS2813
#define BRIGHTNESS 70
#define redledinbutton 2
#define greenledinbutton 8
#define blueledinbutton 4
#define yellowinbutton 6
#define redbutton A0 //14 //A0 
#define bluebutton A1 //15 //A1
#define greenbutton A3 //17 //A3
#define yellowbutton A2 //16 //A2

#include <pcmConfig.h>
#include <pcmRF.h>
#include <TMRpcm.h>
#include <SPI.h>    //include speaker control library
#include <SD.h>                           //include SD module library
                       
#define SD_ChipSelectPin 53  //define CS pin
TMRpcm tmrpcm;  //crete an object for speaker library
bool running_speaker = false;

const byte MAX_LEVEL = 10;
byte sequence[MAX_LEVEL]; //c++ array number 10 sets 10 spaces, not set 10 as a variable inside. 
byte your_sequence[MAX_LEVEL];
byte level = 1;
byte velocity = 1000;
byte generateseq = 0;

CRGB rawleds[239];
CRGBSet leds(rawleds, 239);
CRGBSet leds1(leds(0, 59));
CRGBSet leds2(leds(60, 119));
CRGBSet leds3(leds(120, 179));
CRGBSet leds4(leds(180, 239));

struct CRGB * ledarray[] = {
  leds1,
  leds2,
  leds3,
  leds4
};

void setup() {
  delay(2000); // power-up safety delay 
  FastLED.addLeds < chipset, Data_Pin > (leds, 239);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
  set_max_power_indicator_LED(13);
  Serial.begin(9600);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);

  Serial.begin(9600);
  tmrpcm.speakerPin = 46;       //define speaker pin. //you must use pin 9 of the Arduino Uno and Na//the library is using this pin
  if (!SD.begin(SD_ChipSelectPin)) { 
    //see if the card is present and can be initialized
    return;                  //don't do anything more if not
  }
  
 tmrpcm.setVolume(4);        //0 to 7. Set volume level
 //tmrpcm.play("ben1.wav"); 
}

void loop() {
  
  if (generateseq == 0)
  {
  fill_solid(ledarray[0], 60, CRGB::Black);
  FastLED.show();
  fill_solid(ledarray[1], 60, CRGB::Black);
  FastLED.show();
  fill_solid(ledarray[2], 60, CRGB::Black);
  FastLED.show();
  fill_solid(ledarray[3], 60, CRGB::Black);
  FastLED.show();
  }  
  
    if ( (digitalRead( A0 ) == HIGH || digitalRead( A1 ) == HIGH || digitalRead(A2) == HIGH || digitalRead(A3) == HIGH) && (generateseq==0) ) //If start button is pressed or you're winning
  // digitalRead() Reads the value from a specified digital pin, either HIGH or LOW.
    {
      delay(400);
      next_stage();
    }

    if (generateseq == 1) {
        while ( generateseq == 1 ) {
          generate_sequence();
          if (level == 10) {
            generateseq = 0;
            level = 1;
            velocity = 1000;
            }
          show_sequence();
          get_sequence();
          continue;
        }   
    }
}


void next_stage() {

    byte redledblink = 1;
    byte blueledblink = 1;
    byte greenledblink = 1;
    byte yellowledblink = 1;

    //led red
    unsigned long previousTimeREDLEDBlink = 0;
    int BlinkDelay = 400;
    //led blue
    unsigned long previousTimeBLUELEDBlink = 0;
    //led green
    unsigned long previousTimeGREENLEDBlink = 0;
    //led yellow
    unsigned long previousTimeYELLOWLEDBlink = 0;
    byte act = 0;
    
    while (act == 0) {
       
      arduino2(7);
      
      unsigned long timeNow = millis();  
      if ( (timeNow - previousTimeREDLEDBlink > BlinkDelay) && (redledblink == 1 || redledblink == 0) ) {
        previousTimeREDLEDBlink += BlinkDelay;
        if (redledblink == 1) {
          redledblink = 0; 
          fill_solid( ledarray[0], 60, CRGB::Red);
          FastLED.show();         
        } 
        
        else{
          redledblink = 1;
          fill_solid( ledarray[0], 60, CRGB::Black);
          FastLED.show();
        }

      } 

      if ( (timeNow - previousTimeBLUELEDBlink > BlinkDelay) && (blueledblink == 1 || blueledblink == 0) ) {
        previousTimeBLUELEDBlink += BlinkDelay;
        if (blueledblink == 1) {
          blueledblink = 0;
          fill_solid( ledarray[1], 60, CRGB::Blue);
          FastLED.show();
        } 
        else {
            blueledblink = 1;
            fill_solid( ledarray[1], 60, CRGB::Black);
            FastLED.show();
          }
      } 

    //toggle yellowled blink
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

    
    //toggle greenled blink
    if ( (timeNow - previousTimeGREENLEDBlink > BlinkDelay) && (greenledblink == 1 || greenledblink == 0) ) {
      previousTimeGREENLEDBlink += BlinkDelay;
      if (greenledblink == 1) {
        greenledblink = 0;
        fill_solid( ledarray[3], 60, CRGB::Green);
        FastLED.show();         
      } 
      else {
          greenledblink = 1;
          fill_solid( ledarray[3], 60, CRGB::Black);
          FastLED.show();
        };
      } 


    //toggle redbutton  
    if ((digitalRead(redbutton) == HIGH) ) {
        redledblink = 2;
        fill_solid( ledarray[0], 60, CRGB::Red);
        FastLED.show();
    }

    //toggle bluebutton
    if ((digitalRead(bluebutton) == HIGH) ) {
        blueledblink = 2;
        fill_solid( ledarray[1], 60, CRGB::Blue);
        FastLED.show();
    }

    //toggle yellowbutton
    if ((digitalRead(yellowbutton) == HIGH) ) {
        yellowledblink = 2;
        fill_solid( ledarray[2], 60, CRGB::Yellow);
        FastLED.show();
    }

    //toggle greenbutton
    if ((digitalRead(greenbutton) == HIGH) ) {
        greenledblink = 2;
        fill_solid( ledarray[3], 60, CRGB::Green);
        FastLED.show();
    }

    if ( ( redledblink == 2 ) && ( blueledblink == 2 ) && ( greenledblink == 2 ) && ( yellowledblink == 2 ) ) {
          timeNow = 0;
          generateseq++;
          arduino2(0);
          return;
    }

  }
}


void show_sequence() {   //sequence= [1,1,1,2,3,1,2,3,2,3], //lights , red, blue, green, yellow //seq is blue blue blue green yellow blue
  fill_solid(ledarray[0], 60, CRGB::Black);
  FastLED.show();
  fill_solid(ledarray[1], 60, CRGB::Black);
  FastLED.show();
  fill_solid(ledarray[2], 60, CRGB::Black);
  FastLED.show();
  fill_solid(ledarray[3], 60, CRGB::Black);
  FastLED.show();
  
  for (byte i = 0; i < level; i++) { //sequence [1,1,1,2,3,1...] blue blue blue green yellow blue //ledarray[0] = red, [1] = blue
    if (i == 0) {
      delay(400);
    }
    if (sequence[i] == 0) {
      fill_solid(ledarray[sequence[i]], 60, CRGB::Red);
      FastLED.show();
      //arduino2(1);
      delay(velocity);
    } else if (sequence[i] == 1) {
      fill_solid(ledarray[sequence[i]], 60, CRGB::Blue);
      FastLED.show();
      //arduino2(2);
      delay(velocity);
    } else if (sequence[i] == 2) {
      fill_solid(ledarray[sequence[i]], 60, CRGB::Yellow);
      FastLED.show();
      //arduino2(3);
      delay(velocity);
    } else if (sequence[i] == 3) {
      fill_solid(ledarray[sequence[i]], 60, CRGB::Green);
      FastLED.show();
      //arduino2(4);
      delay(velocity);
    }
    fill_solid(ledarray[sequence[i]], 60, CRGB::Black);
    FastLED.show();
    //arduino2(0);
    delay(400); //in milliseconds

  }
}


void generate_sequence() {
  randomSeed(millis()); //in this way is really random!!!

  for (byte i = 0; i < MAX_LEVEL; i++) {
    sequence[i] = random(0, 4); //random 0-3 is sub-divided led array, maybe 3 is red, [ 2, 5, 6,..... , 4 ]
  }
}

void get_sequence() {
  byte redbuttonpressed = 0;
  byte bluebuttonpressed = 0;
  byte greenbuttonpressed = 0;
  byte yellowbuttonpressed = 0;
  byte flag = 0; //this flag indicates if the sequence is correct

  for (byte i = 0; i < level; i++) { //sequence [1,1,1,2,3,1...] blue blue blue green yellow blue //ledarray[0] = red, [1] = blue
    flag = 0;
    while ( (flag == 0)  ) { 

      if( digitalRead(A0) == LOW){
        fill_solid(ledarray[0], 60, CRGB::Black); 
        FastLED.show();
        redbuttonpressed = 0;
        } else {
        fill_solid(ledarray[0], 60, CRGB::Red);
        FastLED.show();
        redbuttonpressed = 1;
        your_sequence[i] = 0;
        if (your_sequence[i] != sequence[i]) {
          arduino2(1);
          while (redbuttonpressed == 1) {
            if( digitalRead(A0) == LOW) {
              redbuttonpressed = 0;
              flag = 1;
              }
            }
            arduino2(0);
          wrong_sequence();
          return;
        } else {
          arduino2(1);
          
          while (redbuttonpressed == 1) {
            if( digitalRead(A0) == LOW) {
              redbuttonpressed = 0;
              flag = 1;
              }
          }
          arduino2(0);
        }
      }
      

      if( digitalRead(A1) == LOW) {
        fill_solid(ledarray[1], 60, CRGB::Black); 
        FastLED.show();
        bluebuttonpressed = 0;
        } else {
        fill_solid(ledarray[1], 60, CRGB::Blue);
        FastLED.show();
        bluebuttonpressed = 1;
        your_sequence[i] = 1;
        if (your_sequence[i] != sequence[i]) {
          arduino2(2);
          while (bluebuttonpressed == 1) {
            if( digitalRead(A1) == LOW) {
              bluebuttonpressed = 0;
              flag = 1;
              }
            }
            arduino2(0);
          wrong_sequence();
          return;
        } else {
          arduino2(2);
          while (bluebuttonpressed == 1) {
            if( digitalRead(A1) == LOW) {
              bluebuttonpressed = 0;
              flag = 1;
              }
            }
            arduino2(0);
          }
        }

      
      if( digitalRead(A2) == LOW) {
        fill_solid(ledarray[2], 60, CRGB::Black); 
        FastLED.show();
        yellowbuttonpressed = 0;
        } else {
        fill_solid(ledarray[2], 60, CRGB::Yellow);
        FastLED.show();
        yellowbuttonpressed = 1;
        your_sequence[i] = 2;
        if (your_sequence[i] != sequence[i]) {
          arduino2(3);
          while (yellowbuttonpressed == 1) {
            if( digitalRead(A2) == LOW) {
              yellowbuttonpressed = 0;
              flag = 1;
              }
            }
          arduino2(0);
          wrong_sequence();
          return;
        } else {
          arduino2(3);
          while (yellowbuttonpressed == 1) {
            if( digitalRead(A2) == LOW) {
              yellowbuttonpressed = 0;
              flag = 1;
              }
            }
            arduino2(0);
          }
        }

      
      if( digitalRead(A3) == LOW) {
        fill_solid(ledarray[3], 60, CRGB::Black); 
        FastLED.show();
        greenbuttonpressed = 0;
        } else {
        fill_solid(ledarray[3], 60, CRGB::Green);
        FastLED.show();
        greenbuttonpressed = 1;
        your_sequence[i] = 3;
        if (your_sequence[i] != sequence[i]) {
          arduino2(4);
          while (greenbuttonpressed == 1) {
            if( digitalRead(A3) == LOW) {
              greenbuttonpressed = 0;
              flag = 1;
              }
            }
            arduino2(0);
          wrong_sequence();
          return;
        } else {
          arduino2(4);
          while (greenbuttonpressed == 1) {
            if( digitalRead(A3) == LOW) {
              greenbuttonpressed = 0;
              flag = 1;
              }
            }
            arduino2(0);
          }
        }

    }
  }
  right_sequence(); // blink 1 time if right seq
}

void wrong_sequence() { //all led blinks 
  delay(500);
  bool soundonce = false;
  for (byte i = 0; i < 3; i++) {
    
    delay(500);
    fill_solid(leds, 239, CRGB::Red);
    FastLED.show();
    if (soundonce == false) {
      arduino2(5);
      soundonce = true;
    }

    delay(500);
    fill_solid(leds, 239, CRGB::Black);
    FastLED.show();
    
  }
  arduino2(0);
  generateseq = 0; //need to change as ours will end game
  velocity = 1000;
  level = 1;
}


void right_sequence() {
  
  delay(500);
  fill_solid(leds, 239, CRGB::Black);
  FastLED.show();
  delay(500);

  fill_solid(leds, 239, CRGB::Green);
  FastLED.show();
  arduino2(6); 
  delay(500);
   
  fill_solid(leds, 239, CRGB::Black);
  FastLED.show();
  
  if (level < MAX_LEVEL){ 
    level++;
  }
  //velocity -= 30;
  arduino2(0);
} 

void arduino2(int states) {
  
  if ( states ==0 ) {
    // digitalWrite(13,LOW);
    // digitalWrite(12,LOW);
    // digitalWrite(11,LOW);
    running_speaker = false;
  } 
  else if ( states == 1 && running_speaker ==false ) { //red
  // digitalWrite(13,HIGH);
  // digitalWrite(12,LOW);
  // digitalWrite(11,LOW);
  tmrpcm.play("1.wav");
  running_speaker = true;
  }
  
  else if ( states == 2 && running_speaker ==false) { //blue
  // digitalWrite(13,LOW);
  // digitalWrite(12,HIGH);
  // digitalWrite(11,LOW);
  tmrpcm.play("2.wav");
  running_speaker = true;

  }

  else if ( states == 3 && running_speaker ==false) { //yellow
  // digitalWrite(13,HIGH);
  // digitalWrite(12,HIGH);
  // digitalWrite(11,LOW);
  tmrpcm.play("3.wav");
  running_speaker = true;
  }

  else if (states == 4 && running_speaker ==false) { //green
  // digitalWrite(13,LOW);
  // digitalWrite(12,LOW);
  // digitalWrite(11,HIGH);
  tmrpcm.play("4.wav");
  running_speaker = true;
  }

  else if (states == 5 && running_speaker ==false) { //wrong seq
  // digitalWrite(13,HIGH);
  // digitalWrite(12,LOW);
  // digitalWrite(11,HIGH);
  tmrpcm.play("w.wav");
  running_speaker = true;
  }

  else if (states == 6 && running_speaker ==false) { //correct seq
  // digitalWrite(13,LOW );
  // digitalWrite(12,HIGH);
  // digitalWrite(11,HIGH);
  tmrpcm.play("c.wav");
  running_speaker = true;
  }

  else if (states == 7 && running_speaker ==false) { //backgrd music 
  // digitalWrite(13,HIGH);
  // digitalWrite(12,HIGH);
  // digitalWrite(11,HIGH);
  tmrpcm.play("bgm.wav");
  running_speaker = true;
  }
  
}