/* This program makes the Arduino listen to Serial Monitor data sent by the Raspberry Pi. 
   The LEDs inside three arcade buttons can be controlled through the RPI-Arduino 
   connection. It has a number of states of (in)activeness.

   Original millis()-based code by James Lewis, as pwm-fade-with-millis.ino 
   see baldengineer.com/fading-led-analogwrite-millis-example.html for the source code
   
   Original controlling Arduino with RPI code by Fabio Nelli
   see meccanismocomplesso.org/en/controlling-arduino-raspberry-pi/ for the source code
   
   Adapted by Arthur Geel, 03-06-2018
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


// Pins for LED buttons, Neopixel ring
#define PIN 6
const byte LED_BUTTON_1 = 9;
const byte LED_BUTTON_2 = 10;
const byte LED_BUTTON_3 = 11;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

// piInput is input sent from the Raspberry Pi
int piInput;
 
 
// define directions for LED fade
#define UP 0
#define DOWN 1
 
// constants for min and max PWM
const int minPWM = 0;
const int maxPWM = 255;
 
// State Variable for Fade Direction
byte fadeDirection = UP;
 
// Global Fade Value
// but be bigger than byte and signed, for rollover
int fadeValue = 0;
byte fadeIncrement = 5;
 
// millis() timing Variable, just for fading
unsigned long previousFadeMillis;
unsigned long npMillis = 0;

// Neopixel variables
#define NUM_PIXELS 12
uint32_t currentColor;
uint16_t currentPixel = 0;
 
// How fast to increment?
int fadeInterval = 50;
int fastFadeInterval = 20;
 
void setup() {
  // On startup: ensure buttons and ring are 'off'.
  analogWrite(LED_BUTTON_1, 0); 
  analogWrite(LED_BUTTON_2, 0); 
  analogWrite(LED_BUTTON_3, 0); 
  
  currentColor = strip.Color(50,50,50);
  currentPixel = 0;
  strip.begin();
  strip.show();
  
  Serial.begin(9600);
  piInput = 0;
}
 
void regularFade(unsigned long thisMillis) {
  // is it time to update yet?
  // if not, nothing happens
  if (thisMillis - previousFadeMillis >= fadeInterval) {
    // yup, it's time!
    if (fadeDirection == UP) {
      fadeValue = fadeValue + fadeIncrement;  
      if (fadeValue >= maxPWM) {
        // At max, limit and change direction
        fadeValue = maxPWM;
        fadeDirection = DOWN;
      }
    } else {
      //if we aren't going up, we're going down
      fadeValue = fadeValue - fadeIncrement;
      if (fadeValue <= minPWM) {
        // At min, limit and change direction
        fadeValue = minPWM;
        fadeDirection = UP;
      }
    }
    // Only need to update when it changes
    analogWrite(LED_BUTTON_1, fadeValue);
    analogWrite(LED_BUTTON_2, fadeValue); 
    analogWrite(LED_BUTTON_3, fadeValue);   
 
    // reset millis for the next iteration (fade timer only)
    previousFadeMillis = thisMillis;
  }
}

void fastFade(unsigned long thisMillis) {
  // is it time to update yet?
  // if not, nothing happens
  if (thisMillis - previousFadeMillis >= fastFadeInterval) {
    // yup, it's time!
    if (fadeDirection == UP) {
      fadeValue = fadeValue + fadeIncrement;  
      if (fadeValue >= maxPWM) {
        // At max, limit and change direction
        fadeValue = maxPWM;
        fadeDirection = DOWN;
      }
    } else {
      //if we aren't going up, we're going down
      fadeValue = fadeValue - fadeIncrement;
      if (fadeValue <= minPWM) {
        // At min, limit and change direction
        fadeValue = minPWM;
        fadeDirection = UP;
      }
    }
    // Only need to update when it changes
    analogWrite(LED_BUTTON_1, fadeValue);
    analogWrite(LED_BUTTON_2, fadeValue); 
    analogWrite(LED_BUTTON_3, fadeValue);   
 
    // reset millis for the next iteration (fade timer only)
    previousFadeMillis = thisMillis;
  }
}

void fadePlayButton(unsigned long thisMillis) {
  // is it time to update yet?
  // if not, nothing happens
  if (thisMillis - previousFadeMillis >= fastFadeInterval) {
    // yup, it's time!
    if (fadeDirection == UP) {
      fadeValue = fadeValue + fadeIncrement;  
      if (fadeValue >= maxPWM) {
        // At max, limit and change direction
        fadeValue = maxPWM;
        fadeDirection = DOWN;
      }
    } else {
      //if we aren't going up, we're going down
      fadeValue = fadeValue - fadeIncrement;
      if (fadeValue <= minPWM) {
        // At min, limit and change direction
        fadeValue = minPWM;
        fadeDirection = UP;
      }
    }
    // Only need to update when it changes
    analogWrite(LED_BUTTON_1, fadeValue);
    analogWrite(LED_BUTTON_2, 0); 
    analogWrite(LED_BUTTON_3, 0);   
 
    // reset millis for the next iteration (fade timer only)
    previousFadeMillis = thisMillis;
  }
}

void buttonsOn(){
  analogWrite(LED_BUTTON_1, 255);
  analogWrite(LED_BUTTON_2, 255); 
  analogWrite(LED_BUTTON_3, 255);  
}

void buttonsOff(){
  analogWrite(LED_BUTTON_1, 0);
  analogWrite(LED_BUTTON_2, 0); 
  analogWrite(LED_BUTTON_3, 0);  
}

void colorWipe(){
  strip.setPixelColor(currentPixel, strip.Color(50,50,50));
  strip.show();
  currentPixel++;
  if(currentPixel == NUM_PIXELS) {
    currentPixel = 0;
  }
}

void colorWipe_2(){
  strip.setPixelColor(currentPixel, strip.Color(50,0,0));
  strip.show();
  currentPixel++;
  if(currentPixel == NUM_PIXELS) {
    currentPixel = 0;
  }
}

void colorWipe_3(){
  strip.setPixelColor(currentPixel, strip.Color(50,50,80));
  strip.show();
  currentPixel++;
  if(currentPixel == NUM_PIXELS) {
    currentPixel = 0;
  }
}

void pixelOff(){
  strip.setPixelColor(currentPixel, strip.Color(0,0,0));
  strip.show();
  currentPixel++;
  if (currentPixel == NUM_PIXELS) {
    currentPixel = 0;
  }
}

 
void loop() {
  // get the current time, for this time around loop
  // all millis() timer checks will use this time stamp
  unsigned long currentMillis = millis();
  
  if (Serial.available()){
    piInput = Serial.read() - '0';
  }

  if (piInput == 1) {
    buttonsOff();
    pixelOff();
  }
  
  else if (piInput == 2) {
    regularFade(currentMillis);
    // Slow pulsing buttons. Subtle ring animation.
    
    if ((unsigned long)(millis() - npMillis) >= fadeInterval) {
      npMillis = millis();
      colorWipe();
     }
     
  }
  
  else if (piInput == 3) {
    fastFade(currentMillis);
    
    if ((unsigned long)(millis() - npMillis) >= fadeInterval) {
      npMillis = millis();
      colorWipe_2();
     }
    // Fast pulsing button. Prominent ring animations.
  }
  
  else if (piInput == 4) {
    regularFade(currentMillis);
    
    if ((unsigned long)(millis() - npMillis) >= fadeInterval) {
      npMillis = millis();
      colorWipe();
     }
    // buttons gently pulse, LED ring should indicate current time and overall time.
  }
  
  else if (piInput == 5) {
    fadePlayButton(currentMillis);
    
    if ((unsigned long)(millis() - npMillis) >= fadeInterval) {
      npMillis = millis();
      pixelOff();
     }
    
    // buttons gently pulse, LED ring should indicate current time and overall time.
  }
  
  else {
    buttonsOff();
    pixelOff();
    // catch incorrect data: this will turn the buttons off.
  }
}
