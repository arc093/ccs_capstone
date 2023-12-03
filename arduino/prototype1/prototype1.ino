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
const int durPotPin = A5;       //black wire
const int speedPotPin = A0;     //white wire
const int maxSpeed = 225;
const int minSpeed = 30;



// variables will change:
byte buttonState = LOW;         // variable for reading the pushbutton status
byte prevButtonState = LOW;
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
  Serial.begin(9300);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  durationPar = map(analogRead(durPotPin), 0, 1023, 0, 5000);
  speedPar = map(analogRead(speedPotPin), 0, 1023, minSpeed, maxSpeed);

  if (buttonState != prevButtonState){
    prevButtonState = buttonState;
    if (buttonState == HIGH){
      analogWrite(ledPin,speedPar);
    }
    if (buttonState == LOW){
      startTime = millis();
      curTime = millis()- startTime;
      while(curTime < durationPar){
        durationPar = map(analogRead(durPotPin), 0, 1023, 0, 5000);
        speedPar = map(analogRead(speedPotPin), 0, 1023, minSpeed, maxSpeed);
        analogWrite(ledPin,speedPar);
        curTime = millis() - startTime;
      }
      digitalWrite(ledPin,LOW);
    }
  }
}