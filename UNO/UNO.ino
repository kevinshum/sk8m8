// Declare pin functions on Redboard
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6

// Buttons = b
// Leds = l 
// Sizes: small, med, large
const int blarge=13;
const int bmed=11;
const int bsmall=12;
const int llarge=10;
const int lmed=8;
const int lsmall=9;
int bsmall_state = 0;
int bmed_state = 0;
int blarge_state = 0;

char buttonInput[2]; // Buffer to read from NANO
char currentState = "S"; // Keep track of size of skateboard
char nextState = "S";

const int FULL = 550;
const int HALF = FULL/2+10;

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(blarge, INPUT);     
  pinMode(bmed, INPUT);     
  pinMode(bsmall, INPUT);  
  pinMode(llarge, OUTPUT);     
  pinMode(lmed, OUTPUT);     
  pinMode(lsmall, OUTPUT);   
  
  resetEDPins(); // Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); // Open Serial connection for debugging
  digitalWrite(lsmall,HIGH);
}

void loop() {
  Serial.readBytes(buttonInput, 1);
  blarge_state = digitalRead(blarge);
  bmed_state = digitalRead(bmed);
  bsmall_state = digitalRead(bsmall);
  
  nextState = "";
  if (buttonInput[0]!="") { // Input from Nano 
    nextState = buttonInput[0];
  } 
  if (bsmall_state == HIGH) {
    nextState = 'S';
  } 
  else if (bmed_state == HIGH) {
    nextState = 'M';
  } 
  else if (blarge_state == HIGH) {
    nextState = 'L';
  }
  
  digitalWrite(EN, LOW); // Pull enable pin low to allow motor control
  
  if (nextState == 'L') { // Want it at the largest setting
    digitalWrite(lsmall, LOW);
    digitalWrite(lmed, LOW);
    digitalWrite(llarge, HIGH); // Turn on large led
    
    if (currentState == 'S') { // S -> L
      growBoard(FULL-10);
      
    } else if (currentState == 'M') { // M -> L
      growBoard(HALF-25);
    } 
    currentState = 'L';
    
  } else if (nextState == 'M') { // Want med setting
    digitalWrite(lsmall, LOW);
    digitalWrite(lmed, HIGH); // Turn on med led
    digitalWrite(llarge, LOW);
    
    if (currentState == 'S') { // S -> M
        growBoard(HALF-12);
        
    } else if (currentState == 'L') {
      shrinkBoard(HALF-20);
    } 
    currentState = 'M';
    
  } else if (nextState =='S') { // Want at smallest setting
    digitalWrite(lsmall, HIGH);
    digitalWrite(lmed, LOW); // Turn on small led
    digitalWrite(llarge, LOW);
    
    if (currentState == 'M') { // M -> S
      shrinkBoard(HALF-8);
      
    } else if (currentState == 'L') { // L -> S
      shrinkBoard(FULL-12);
    } 
    currentState = 'S';
  }
}

// Grow board by step steps
void growBoard(int steps) {  
  digitalWrite(dir, LOW); // Forwards
  for(int x=1; x<steps; x++) { 
    digitalWrite(stp,HIGH); // Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); // Pull step pin low so it can be triggered again
    delay(1);
  }
}

// Shrink board by steps steps
void shrinkBoard(int steps) {  
  digitalWrite(dir, HIGH); // Backwards
  for(int x=1; x<steps; x++) { 
    digitalWrite(stp,HIGH); // Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); // Pull step pin low so it can be triggered again
    delay(1);
  }
}

// Reset Easy Driver pins to default states
void resetEDPins() {
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}

