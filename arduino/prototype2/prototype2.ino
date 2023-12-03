
const int maxSpeed = 225;
const int minSpeed = 30;
const int speedPotPin = A0;     //white wire
const int durPotPin = A1;       //black wire
float durationPar = 0.0;  //seconds
float speedPar = 0.0;
int buttonPins[] = {1,4,7,10}; //{green,yellow,blue,red}
int ledPins[] = {3,6,9,11}; //{green,yellow,blue,red}
byte buttonStates[] = {0, 0, 0, 0};
byte otherButtonStates[] = {0, 0, 0, 0};
unsigned long startTime = millis ();
unsigned long curTime = millis();
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  readStates();
  int currentDirection = 5;
  for (int i=0; i<4 ; i++){
    if (1 == buttonStates[i]){
      currentDirection=i;
      break;
    }
  }
  //if we found that a button is pressed
  if (currentDirection != 5){
    analogWrite(ledPins[currentDirection],speedPar);
    while(buttonStates[currentDirection] == 1){
      readStates();
      analogWrite(ledPins[currentDirection],speedPar);
    }
    //start timer
    startTime = millis();
    curTime = millis()- startTime;
    while(curTime<durationPar){
      readStates();
      analogWrite(ledPins[currentDirection],speedPar);
      //check if any of the other buttons have been turned on!
      int newDirection = 5;
      //copy buttonStates while changing the current direction to zero
      for (int i=0; i<4 ; i++){
        otherButtonStates[i] = buttonStates[i];
      }
      otherButtonStates[currentDirection] = 0;
      for (int i=0; i<4 ; i++){
        if (1 == buttonStates[i]){
          newDirection=i;
          break;
        }
      }
      if (newDirection =! 5){
        //break the timer while loop
        break;
      } 
      curTime = millis() - startTime;
    }
    digitalWrite(ledPins[currentDirection],LOW);
  } 
}

void readStates() {
  durationPar = map(analogRead(durPotPin), 0, 1023, 0, 5000);
  speedPar = map(analogRead(speedPotPin), 0, 1023, minSpeed, maxSpeed);
  for (int i = 0; i<=3; i++){
    buttonStates[i] = digitalRead(buttonPins[i]);
  }
}