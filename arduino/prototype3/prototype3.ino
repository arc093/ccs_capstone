const int maxSpeed = 225; //corresponds to maximum analog output
const int minSpeed = 30;
const int speedPotPin = A0;     //white wire
const int durPotPin = A1;       //black wire
float durationPar = 0.0;  //seconds
float speedPar = 0.0;

int ledPins[] = {9,11}; //{green, red}

int cMotionButtonPins[] = {7,10}; //{white, white}
byte cMotionButtonStates[] = {0, 0};
byte copiedButtonStates[] = {0, 0}; //used for child motion button states

int ptMotionButtonPins[] = {2,4}; //{orange}
byte ptMotionButtonStates[] = {0,0};

const int numMotionButtons = 2;

const int modeButtonPins[] = {13,12,8}; //stop, roll, go
byte modeButtonStates[] = {0, 0, 0};
int mode = 0; //0: stop, 1: roll (just pt controls), 2: go (both controls)
byte ptOverride = 0; //only relevant in mode 2, when 0: child controls are active, when 1: pt controls are taking precidence

unsigned long startTime = millis ();
unsigned long curTime = millis();


void setup() {
  Serial.begin(9300);
  // put your setup code here, to run once:
  for (int i=0; i<numMotionButtons; i++){
    pinMode(ledPins[i], OUTPUT);
  }
  for (int i=0; i<numMotionButtons; i++){
    pinMode(cMotionButtonPins[i], INPUT);
  }
  for (int i=0; i<numMotionButtons; i++){
    pinMode(ptMotionButtonPins[i], INPUT);
  }

  for (int i=0; i<3; i++){
    pinMode(modeButtonPins[i], INPUT);
  }

  pinMode(durPotPin, INPUT);
  pinMode(speedPotPin, INPUT);
}

void loop() {
  readStates();
  if (mode == 1){
    ptControls();
  }
  if (mode == 2){
    ptControls();
    cControls();
  }

}

void ptControls(){
  //janky way to read cMotionButtonStates for 1 and remember location of the 1
  int currentDirection = -1;
  for (int i=0; i<numMotionButtons ; i++){
    if (1 == ptMotionButtonStates[i]){
      currentDirection=i;
      break;
    }
  }
  
  // if we found that a button is pressed
  if (currentDirection != -1){
    Serial.println("pt button pressed! currentDirection: "); Serial.println(currentDirection);
    analogWrite(ledPins[currentDirection],speedPar);
    while(ptMotionButtonStates[currentDirection] == 1){
      readStates();
      analogWrite(ledPins[currentDirection],speedPar);
      if (mode == 0) {
        break;
      }
    }
    digitalWrite(ledPins[currentDirection],LOW);
  }   
}

void cControls(){
  //janky way to read cMotionButtonStates for 1 and remember location of the 1
  int currentDirection = -1;
  for (int i=0; i<numMotionButtons ; i++){
    if (1 == cMotionButtonStates[i]){
      currentDirection=i;
      break;
    }
  }
  // if we found that a button is pressed
  if (currentDirection != -1){
    Serial.println("child button pressed! currentDirection: "); Serial.println(currentDirection);
    analogWrite(ledPins[currentDirection],speedPar);
    while(cMotionButtonStates[currentDirection] == 1){
      readStates();
      analogWrite(ledPins[currentDirection],speedPar);
      if (mode == 0) {
        break;
      }
      if (mode == 1){
        break;
      }
      if (ptOverride == 1){
        break;
      }
    }
    //Serial.println("Button released");
    //start timer
    startTime = millis();
    curTime = millis()- startTime;
    while(curTime<durationPar){
      //Serial.println("Button timer running!");
      readStates();
      analogWrite(ledPins[currentDirection],speedPar);
      if (mode == 0) {
        break;
      }
      if (mode == 1){
        break;
      }
      if (ptOverride == 1){
        break;
      }
      //check if any of the other buttons have been turned on!
      int newDirection = 5;
      //copy cMotionButtonStates while changing the current direction to zero
      for (int i=0; i<numMotionButtons ; i++){
        copiedButtonStates[i] = cMotionButtonStates[i];
      }
      copiedButtonStates[currentDirection] = 0;
      for (int i=0; i<numMotionButtons ; i++){
        if (1 == copiedButtonStates[i]){
          newDirection=i;
          break;
        }
      }
      //Serial.print("copiedButtonStates: "); for (int i = 0; i<4; i++){Serial.print(copiedButtonStates[i]);}; Serial.println();
      //Serial.print("newDirection: "); Serial.println(newDirection);
      if (newDirection != 5){
        //break the timer while loop
        //Serial.println("Button timer broken due to new button being pressed"); 
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
  for (int i = 0; i<numMotionButtons; i++){
    cMotionButtonStates[i] = digitalRead(cMotionButtonPins[i]);
  }
  for (int i = 0; i<numMotionButtons; i++){
    ptMotionButtonStates[i] = digitalRead(ptMotionButtonPins[i]);
  }
  for (int i = 0; i<3; i++){
    modeButtonStates[i] = digitalRead(modeButtonPins[i]);
  }
  int modePressed = -1;
  for (int i=0; i<3 ; i++){
    if (1 == modeButtonStates[i]){
      modePressed=i;
      break;
    }
  }
  if (modePressed != -1){
    mode = modePressed;
  }
  //check if pt buttons are pressed to engage override
  if (mode == 2){
    //janky way to check if button is pressed
    int currentDirection = -1;
    for (int i=0; i<numMotionButtons ; i++){
      if (1 == ptMotionButtonStates[i]){
        currentDirection=i;
        break;
      }
    }
    // if we found that a button is pressed
    if (currentDirection != -1){
      ptOverride = 0;
    }
    else{
      ptOverride = 1;
    }
  }
  Serial.println("mode, ptOverride: "); Serial.println(mode); Serial.println(ptOverride);
  // Serial.print("Reading States; durationPar:"); Serial.print(durationPar);
  // Serial.print(", speedPar: "); Serial.print(speedPar); 
  // Serial.print(", buttonStates: "); for (int i = 0; i<4; i++){Serial.println(buttonStates[i]);};
}

