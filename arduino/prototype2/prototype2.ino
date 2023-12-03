const int maxSpeed = 225; //corresponds to maximum analog output
const int minSpeed = 30;
const int speedPotPin = A0;     //white wire
const int durPotPin = A1;       //black wire
float durationPar = 0.0;  //seconds
float speedPar = 0.0;
int buttonPins[] = {2,4,7,10}; //{green,yellow,blue,red}
int ledPins[] = {3,6,9,11}; //{green,yellow,blue,red}
byte buttonStates[] = {0, 0, 0, 0};
byte otherButtonStates[] = {0, 0, 0, 0};
unsigned long startTime = millis ();
unsigned long curTime = millis();
void setup() {
  Serial.begin(9300);
  // put your setup code here, to run once:
  for (int i=0; i<4; i++){
    pinMode(ledPins[i], OUTPUT);
  }
  for (int i=0; i<4; i++){
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(durPotPin, INPUT);
  pinMode(speedPotPin, INPUT);
}

void loop() {
  readStates();
  //janky way to read buttonStates for 1 and remember location of the 1
  int currentDirection = 5;
  for (int i=0; i<4 ; i++){
    if (1 == buttonStates[i]){
      currentDirection=i;
      break;
    }
  }
  Serial.print("currentDirection: "); Serial.println(currentDirection);
  //if we found that a button is pressed
  // if (currentDirection != 5){
  //   Serial.print("Button pressed: "); Serial.println(currentDirection);
  //   analogWrite(ledPins[currentDirection],speedPar);
  //   while(buttonStates[currentDirection] == 1){
  //     readStates();
  //     analogWrite(ledPins[currentDirection],speedPar);
  //   }
  //   Serial.println("Button released");
  //   //start timer
  //   startTime = millis();
  //   curTime = millis()- startTime;
  //   while(curTime<durationPar){
  //     Serial.println("Button timer running!");
  //     readStates();
  //     analogWrite(ledPins[currentDirection],speedPar);
  //     //check if any of the other buttons have been turned on!
  //     int newDirection = 5;
  //     //copy buttonStates while changing the current direction to zero
  //     for (int i=0; i<4 ; i++){
  //       otherButtonStates[i] = buttonStates[i];
  //     }
  //     otherButtonStates[currentDirection] = 0;
  //     for (int i=0; i<4 ; i++){
  //       if (1 == buttonStates[i]){
  //         newDirection=i;
  //         break;
  //       }
  //     }
  //     if (newDirection =! 5){
  //       //break the timer while loop
  //       Serial.println("Button timer broken due to new button being pressed"); 
  //       break;
  //     } 
  //     curTime = millis() - startTime;
  //   }
  //   digitalWrite(ledPins[currentDirection],LOW);
  // } 
}

void readStates() {
  durationPar = map(analogRead(durPotPin), 0, 1023, 0, 5000);
  speedPar = map(analogRead(speedPotPin), 0, 1023, minSpeed, maxSpeed);
  for (int i = 0; i<4; i++){
    buttonStates[i] = digitalRead(buttonPins[i]);
  }
  Serial.print("Reading States; durationPar:"); Serial.print(durationPar);
  Serial.print(", speedPar: "); Serial.print(speedPar); 
  Serial.print(", buttonStates: "); for (int i = 0; i<4; i++){Serial.println(buttonStates[i]);};
}