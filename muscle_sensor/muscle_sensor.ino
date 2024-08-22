/*
 * This program demonstrates basic functionality of hte Myoware Muscle Sensor
 */

const int LEDPIN = LED_BUILTIN;
int sensorPin = A0;
int sensorValue = 0;      //Analog value from sensor in range of 0-1032.
int sensorOffset = 20;    //Analog value required to trigger the LED Matrix
int sensorMedian = 0;     //The value "closest to the middle" of the sorted array (actually one element past the exact middle)
int sensorCount = 0;      //Track the number of program cycles and store a new element in the sensorArray each cycle. Sort once the array is full.
int sensorArray[10];      //Holds the last ten values of the sensor.
int arrayLength = 0;
int sensorTemp = 0;       //Temporarilty holds the holds the lower value during an array element swap.
bool sensorState = false;    //Current state of the LED Matrix (ON = 1, OFF = 0)
bool sortState = false;   //Defaults to FALSE. The array is considered unsorted until proven otherwise. 
                          //During each sorting pass, the state is initialized to TRUE and toggles to FALSE if any element of the array is reordered.

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  Serial.begin(115200);
  arrayLength = sizeof(sensorArray) / sizeof(sensorArray[0]);
}

void readSensor() {
  //Read Sensor
  sensorValue = analogRead(sensorPin);
  if(sensorCount < sizeof(sensorArray) / sizeof(sensorArray[0])) {
    sensorArray[sensorCount] = sensorValue;
    sensorCount++;
//    Serial.print("Sensor READING: ");
//    Serial.println(sensorValue);
  }
  else {
//    Serial.print("SensorArray UNSORTED: ");
//    for (int i = 0; i < arrayLength; i++) {
//      Serial.print(sensorArray[i]);
//      Serial.print(", ");
//    }
//    Serial.println();
    arraySort();
//    Serial.print("SensorArray SORTED: ");
//    for (int i = 0; i < arrayLength; i++) {
//      Serial.print(sensorArray[i]);
//      Serial.print(", ");
//    }
//    Serial.println();
    sensorMedian = sensorArray[(int)(sizeof(sensorArray) / sizeof(sensorArray[0])) / 2];
    //Set LED Matrix
    if(sensorValue >= (sensorMedian + sensorOffset) && sensorState == false) {
      Serial.println("Sensor ON!");
      sensorState = true;
      digitalWrite(LEDPIN, HIGH);
    }
    else if(sensorValue < sensorMedian && sensorState == true) {
      Serial.println("Sensor OFF!");
      sensorState = false;
      digitalWrite(LEDPIN, LOW);
    }
    sensorCount = 0;
//    Serial.print("Sensor VALUE: ");
//    Serial.println(sensorValue);
//    Serial.print("Sensor MEDIAN: ");
//    Serial.println(sensorMedian);
//    Serial.println();
  } 
}

void arraySort() {
  while (sortState == false) {  //Repeat until array is sorted (loop returns with sortState set to TRUE)
    sortState = true;           //Toggle state at beginning of each sorting pass
    for(int i = 0; i < (sizeof(sensorArray) / sizeof(sensorArray[0])) - 1; i++) {
      if (sensorArray[i] > sensorArray[i+1]) {
        sensorTemp = sensorArray[i];
        sensorArray[i] = sensorArray[i+1];
        sensorArray[i+1] = sensorTemp;
        sortState = false;  //Because an element was swapped, set sortState to FALSE.
        sensorTemp = 0;
      }
    }
//    Serial.println(sortState);
  }
  sortState = false;    //Reset sortState to default for the next sort
}

void loop(){
  readSensor();
  delay (10);
}
