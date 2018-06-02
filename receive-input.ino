/* This program makes the Arduino listen to Serial Monitor data sent by the Raspberry Pi. 
   The built-in LED on the Arduino board blinks with a certain interval. Adjust the piInput
   variable from the Raspberry Pi to adjust the blinking speed.
   
   Original code by Fabio Nelli, https://www.meccanismocomplesso.org/en/controlling-arduino-raspberry-pi/
   Adapted by Arthur Geel, 02-06-2018
*/

const int ledPin = 13; // built-in LED on Arduino board
int piInput

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  piInput = 7;
}

void loop(){;
  if(Serial.available()){
    // listens to serial monitor for input sent by Raspberry Pi
    piInput = Serial.read() - '0';
  }
  digitalWrite(ledPin, HIGH);
  delay(piInput * 100);
  digitalWrite(ledPin, LOW);
  delay(piInput * 100);
}
