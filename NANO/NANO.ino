#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>

Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);

const int buttonPin=2;
const int ledPin=12;

char largeStr[1]="L";
char medStr[1]="M";
char smallStr[1]="S";
char stateStr[1]="0";

int buttonClicked=0;
int iter = 0; 
int buttonState = 0;
int button = 0;
int lastLastLastY = -1;
int lastLastY = -1;
int lastY = -1;
int numWobbles = 0;

void setup() {
  Serial.begin(9600);
  
  // ===========================
  // GYROSCOPE SETUP
  gyro.enableAutoRange(true); /* Enable auto-ranging */
  if(!gyro.begin()){ /* Initialise the sensor */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  displaySensorDetails();

  // ===========================
  // BUTTON/LED SETUP
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin, OUTPUT);
}

void loop() {
  // ===========================
  // GYROSCOPE DATA
  sensors_event_t event; 
  gyro.getEvent(&event); /* Display the results (speed is measured in rad/s) */
  
  // ====================================
  // STATE MACHINE

  button = digitalRead(buttonPin);
  
  // STOP
  if (buttonState == 0) {
    stateStr[0] = '0';
    Serial.write(stateStr,1); // SEND 0 TO UNO 

    if (button == HIGH){ // move to next state
      resetPerformance();
      buttonState=1;
      digitalWrite(ledPin, HIGH); // turn on light when recording performance
    }
  }
  else if (buttonState == 1) {
    stateStr[0] = '1';
    Serial.write(stateStr,1); // SEND 1 TO UNO 
    buttonState=2;
  }
  // RECORD
  else if (buttonState == 2) {
    stateStr[0] = '2';
    Serial.write(stateStr,1); // SEND 2 TO UNO 
    recordPerformance(event.gyro.y);

    if (button == HIGH){
      buttonState=3;
    }
  }
  // ACTUATE
  else if (buttonState == 3) {
    stateStr[0] = '3';
    Serial.write(stateStr,1); // SEND 3 TO UNO 
    digitalWrite(ledPin, LOW); 

    if (numWobbles > 6) { // numFalls > 2 &&
      Serial.write(largeStr,1); // SEND L TO UNO 
    } else if (numWobbles > 3) {
      Serial.write(medStr,1); // SEND M TO UNO 
    } else {
      Serial.write(smallStr,1); // SEND S TO UNO 
    }

    // Blink to show the number of wobbles
    for (int i =0; i < numWobbles; i++) {
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      delay(200);
    }
    buttonState=0;
  }
  delay(100);
}

// PERFORMANCE TESTING
void recordPerformance(float newY) {
  switch (iter) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      lastLastLastY = lastLastY;
      lastLastY = lastY;
      lastY = newY;
      Serial.print("gyroY ");
      Serial.println(newY);
        if (lastLastLastY != -1 && abs(lastLastLastY - newY) > 0.3) {
        numWobbles += 1;
//        Serial.println("WOBBLE");
      }
      break;
    default:
      break;
  }
  
  iter= (iter+1) % 4;
}

// RESET PERFORMANCE TESTING
void resetPerformance() {
  lastLastLastY = -1;
  lastLastY = -1;
  lastY = -1;
  numWobbles = 0;
}

// GYROSCOPE HELPER FUNCTION
void displaySensorDetails(void)
{
  sensor_t sensor;
  gyro.getSensor(&sensor);
//  Serial.println("------------------------------------");
//  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
//  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
//  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
//  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
//  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
//  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");  
//  Serial.println("------------------------------------");
//  Serial.println("");
  delay(500);
}
