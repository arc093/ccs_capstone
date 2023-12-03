/*

  The circuit
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground
  - Potentiometer attached to pin 3, pin 4


*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;        // the number of the pushbutton pin
const int ledPin =  6;          // the number of the LED pin
const int durPotPin = A5;       //orange wire
const int speedPotPin = A0;     //red wire



// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
float durationPar = 0.0;  //seconds
float speedPar = 0.0;    //voltage applied to motor drivers where 5 is max
unsigned long startTime = millis ();
unsigned long curTime = millis();

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(durPotPin, INPUT);
  pinMode(speedPotPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  //print the pot values to get the correct range for the map function
  Serial.print(analogRead(durPotPin));
  //Serial.print(analogRead(speedPotPin));
  durationPar = map(analogRead(durPotPin), 0, 1023, 0.1, 5);
  speedPar = map(analogRead(speedPotPin), 0, 1023, 1, 5);

  // check if the user is holding the button, continuously start a new timer so when they stop holding the button the timer is released and starts counting
  if (buttonState == HIGH) {
      // start a new timer
      startTime = millis();
      analogWrite(ledPin,speedPar);
      curTime = millis() - startTime;
      // if the timer is done, turn off the LED, if its still going, keep the LED on
      while (curTime < durationPar) {
        // turn LED on with brightness according to speed:
        analogWrite(ledPin,speedPar);
      }
  } 
}
