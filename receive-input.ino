/* This program makes the Arduino listen to Serial Monitor data sent by the Raspberry Pi. 
   The LEDs inside three arcade buttons can be controlled through the RPI-Arduino 
   connection. It has four states: inactive, little news, lotsa news and music playback.

   Original code by Fabio Nelli, https://www.meccanismocomplesso.org/en/controlling-arduino-raspberry-pi/
   Adapted by Arthur Geel, 02-06-2018
*/

const int led_button_1 = 9;
const int led_button_2 = 10;
const int led_button_3 = 11;

int brightness = 0;
int fadeAmount = 5;
int piInput;

void setup() {
  pinMode(led_button_1, OUTPUT);
  pinMode(led_button_2, OUTPUT);
  pinMode(led_button_3, OUTPUT);
  Serial.begin(9600);
  piInput = 0;
}

void loop(){;
  if (Serial.available()){
    piInput = Serial.read() - '0';
  }
  
  if (piInput == 1) {
    analogWrite(led_button_1, 0);
    analogWrite(led_button_2, 0);
    analogWrite(led_button_3, 0);
  }
  
  else if (piInput == 2) {
    analogWrite(led_button_1, brightness);
    analogWrite(led_button_2, brightness);
    analogWrite(led_button_3, brightness);
    
    brightness = brightness + fadeAmount;
    
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    
    delay(50);
  }
  
  else if (piInput == 3) {
    analogWrite(led_button_1, brightness);
    analogWrite(led_button_2, brightness);
    analogWrite(led_button_3, brightness);
    
    brightness = brightness + fadeAmount;
    
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    
    delay(10);
    
  }
  
  else if (piInput == 4) {
    analogWrite(led_button_1, 255);
    analogWrite(led_button_2, 255);
    analogWrite(led_button_3, 255);
    
  }
  
  else {
    analogWrite(led_button_1, 0);
    analogWrite(led_button_2, 0);
    analogWrite(led_button_3, 0);
  }
}
