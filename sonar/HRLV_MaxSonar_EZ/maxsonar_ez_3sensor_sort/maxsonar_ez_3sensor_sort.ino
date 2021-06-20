/*
 * DESCRIPTION:
 * This sketch reads the ranges from three "chained together" sonar sensors 
 * and prints two versions of the returned values to the Serial Monitor.
 * 
 * HARDWARE:
 * HRLV_MaxSonar_EZ
 * Arduino Mega 2560
 * 
 * FUNCTIONALITY:
 * VCC power is supplied to each sensor. The sensors are linked together so that the 
 * the sensors will be automatically triggered in a sequence: left, center, right. 
 * Each sensor is read 10 times in succession and the values are stored in a separate array for each sensor.
 * The calculated ranges are first printed in the order that they were read.
 * The ranges are then sorted (lowest to highest) and printed again.
 * The sketch then chooses the 8th element in each sensor array as the representative value.
 * 
 * NOTES:
 * The 8th range value is chosen because the analog voltages from the sensor can vary over the different readings.
 * By the time of the 8th reading, the value has generally "settled" and is quite reliable while also excluding errant "maximums".
 * 
 * The range is calculated as follows:
 * The analog voltage output from the sensor is proportional to the input voltage (VCC). 
 * Therefore, the formula for the range increment is: Vi = Vcc/1024 where Vi is measured as Volts / 5mm
 * 
 * The total range is calculated as: 5*Vm/Vi where Vm is the measured voltage from the sensor.
 * For a 5v supply, this formula can be simplified to: Vm * 1024
 * 
 * In practice, when using 5v USB power, the sensor gives accurate readings using the formula: 
 * Range(mm) = Vm * 1000 where distance is measured from the FRONT face of the sensor.
 * 
 * When the analog voltage from the sensor is read as an input value using analogRead, 
 * the return value is equal to: Range(cm) * 2 
 * For example, a range of 60cm would return a value from analogRead of ~120.
 * 
 * The minimum (reliable) detectable range is 30cm.
 * 
 * REFERENCE:
 * https://www.maxbotix.com/Ultrasonic_Sensors/MB1003.htm
 * 
 */

 int range_left = 0;    //calculated range value
 int range_center = 0;
 int range_right = 0;
 int sensor_left = A0;    //raw analog voltage readings from the sensor
 int sensor_center = A1;
 int sensor_right = A2;
 int left_value[10];    //arrays to hold stored sensor readings
 int center_value[10];
 int right_value[10];
 int buttonPin = 4;  //manually activate sensor (pulled high when button pressed)
 int triggerPin = 9;
 int flag = 0;  //control state and prevent multiple triggers
 

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); //each button press will pull this pin LOW through a 10K resistor.
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  Serial.begin(115200);
  delay(500); //wait for serial to start
  Serial.println("MaxSonar_EZ Chained Sensor Range Test!");
}

void loop() {
  if(digitalRead(buttonPin) == HIGH && flag == 0) { //check if button pressed (pin pulled HIGH)
    delay(10);
    if(digitalRead(buttonPin) == HIGH) { //double-check button press (debounce)
      flag = 1;         //set flag to prevent accidental retriggers of the button code
      digitalWrite(triggerPin, HIGH);   //pull HIGH to activate first sensor in the chain
      delay(20);
      digitalWrite(triggerPin, LOW);
      delay(20);

      for (int i = 0; i < 10; i++) {
        left_value[i] = analogRead(sensor_left);
        delay(10);
        center_value[i] = analogRead(sensor_center);
        delay(10);
        right_value[i] = analogRead(sensor_right);
        delay(10);
      }
      /*Print unsorted range values*/
      Serial.println("Unsorted Values");
      Serial.print("Left(cm): \t");
      for(int j = 0; j < 10; j++) {
        Serial.print(left_value[j]/2);
        Serial.print("\t");
      }
      Serial.print("\n");
      Serial.print("Center(cm): \t");
      for(int k = 0; k < 10; k++) {
        Serial.print(center_value[k]/2);
        Serial.print("\t");
      }
      Serial.print("\n");
      Serial.print("Right(cm): \t");
      for(int m = 0; m < 10; m++) {
        Serial.print(right_value[m]/2);
        Serial.print("\t");
      }
      Serial.println("\n");

      /*Print sorted range values*/
//      Serial.println("Sorted Arrays: ");
//      Serial.print("Left(cm): \t");
//      sort(left_value, 10);
//      for(int i=0; i<10; i++) {
//         Serial.print(left_value[i]/2); 
//         Serial.print("\t");
//      }
//      Serial.print("\n");
//      Serial.print("Center(cm): \t");
//      sort(center_value, 10);
//      for(int i=0; i<10; i++) {
//         Serial.print(center_value[i]/2); 
//         Serial.print("\t");
//      }
//      Serial.print("\n");
//      Serial.print("Right(cm): \t");
//      sort(right_value, 10);
//      for(int i=0; i<10; i++) {
//         Serial.print(right_value[i]/2); 
//         Serial.print("\t");
//      }
//      Serial.println("\n");

      /*Print selected range values*/
      Serial.println("Selected Values");
      Serial.print("left_value: ");
      Serial.println(left_value[8]/2);
      Serial.print("center_value: ");
      Serial.println(center_value[8]/2);
      Serial.print("right_value: ");
      Serial.println(right_value[8]/2);
      Serial.println("\n");
    }
  }
  else if(digitalRead(buttonPin) == LOW && flag == 1) {
    delay(10);
    if(digitalRead(buttonPin) == LOW) {
      flag = 0;
      memset(left_value, 0, sizeof(left_value));    //reset arrays
      memset(center_value, 0, sizeof(center_value));
      memset(right_value, 0, sizeof(right_value));
    } 
  }
}


void sort(int a[], int size) {
  for(int i=0; i<(size-1); i++) {
    for(int o=0; o<(size-(i+1)); o++) {
      if(a[o] > a[o+1]) {
          int t = a[o];
          a[o] = a[o+1];
          a[o+1] = t;
      }
    }
  }
}
