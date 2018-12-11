//Declare pin functions on Redboard
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6

// button = b
// led = l 
// sizes: small, med, large
const int blarge=13;
const int bmed=11;
const int bsmall=12;
const int llarge=10;
const int lmed=8;
const int lsmall=9;
int bsmall_state = 0;
int bmed_state = 0;
int blarge_state = 0;

char buttonInput[2]; // buffer to read from NANO
char currentState = "S"; // keep track of size of skateboard
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
  
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  digitalWrite(lsmall,HIGH);
}

void loop() {
  Serial.readBytes(buttonInput, 1);
  blarge_state = digitalRead(blarge);
  bmed_state = digitalRead(bmed);
  bsmall_state = digitalRead(bsmall);
  
  nextState = "";
  if (buttonInput[0]!="") { // something from nano 
    nextState = buttonInput[0];
  } 
   if (bsmall_state == HIGH) {
    nextState = 'S';
    Serial.println("SMALL");
  } 

  if (bmed_state == HIGH) {
    nextState = 'M';
    Serial.println("MED");
  } 

  if (blarge_state == HIGH) {
    nextState = 'L';
    Serial.println("LARGE");
  }
  
  digitalWrite(EN, LOW); // Pull enable pin low to allow motor control
  
  if (nextState == 'L') { // Want it at the largest setting
    digitalWrite(lsmall, LOW);
    digitalWrite(lmed, LOW);
    digitalWrite(llarge, HIGH); // Turn on large led
    
    if (currentState == 'S') { // S -> L
      digitalWrite(dir, LOW);
      for(int x= 1; x<FULL-10; x++) {
        digitalWrite(stp,HIGH); // Trigger one step forward
        delay(1);
        digitalWrite(stp,LOW); // Pull step pin low so it can be triggered again
        delay(1);
      }
      
    } else if (currentState == 'M') { // M -> L
      digitalWrite(dir, LOW);
      for(int x= 1; x<HALF-25; x++) {
        digitalWrite(stp,HIGH); 
        delay(1);
        digitalWrite(stp,LOW); 
        delay(1);
      }
    } 
    currentState = 'L';
    
  } else if (nextState == 'M') { // Want med setting
    digitalWrite(lsmall, LOW);
    digitalWrite(lmed, HIGH); // Turn on med led
    digitalWrite(llarge, LOW);
    
    if (currentState == 'S') { // S -> M
        digitalWrite(dir, LOW);
        for(int x= 1; x<HALF-8; x++) {
          digitalWrite(stp,HIGH); 
          delay(1);
          digitalWrite(stp,LOW); 
          delay(1);
        }
        
    } else if (currentState == 'L') {
      digitalWrite(dir, HIGH); // L -> M 
      for(int x= 1; x<HALF-20; x++) {
        digitalWrite(stp,HIGH); 
        delay(1);
        digitalWrite(stp,LOW); 
        delay(1);
      }
    } 
    currentState = 'M';
    
  } else if (nextState =='S') { // Want at smallest setting
    digitalWrite(lsmall, HIGH);
    digitalWrite(lmed, LOW);
    digitalWrite(llarge, LOW);
    
    if (currentState == 'M') { // M -> S
      digitalWrite(dir, HIGH);
      for(int x= 1; x<HALF-8; x++) {
        digitalWrite(stp,HIGH); 
        delay(1);
        digitalWrite(stp,LOW); 
        delay(1);
      }
      
    } else if (currentState == 'L') { // L -> S
      digitalWrite(dir, HIGH);
      for(int x= 1; x<FULL-12; x++) {
        digitalWrite(stp,HIGH); 
        delay(1);
        digitalWrite(stp,LOW); 
        delay(1);
      }
    } 
    currentState = 'S';
  }
}

// Grow board by step steps
void growBoard(int steps) {  
  digitalWrite(dir, LOW); // forwards
  for(int x=1; x<steps; x++) { 
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}

// Shrink board by steps steps
void shrinkBoard(int steps) {  
  digitalWrite(dir, HIGH); // backwards
  for(int x=1; x<steps; x++) { 
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}

//Reset Easy Driver pins to default states
void resetEDPins() {
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}

