/*
 * DESCRIPTION:
 * This sketch reads the ranges from three "chained together" sonar sensors 
 * and prints the returned values to the Serial Monitor.
 * 
 * HARDWARE:
 * HRLV_MaxSonar_EZ
 * Arduino Mega 2560
 * 
 * FUNCTIONALITY:
 * VCC power is supplied to each sensor. The sensors are linked together so that the 
 * the sensors will be automatically triggered in a sequence: left, center, right. 
 * Each sensor is read 10 times (controlled by value of num_samples) in succession and the values are stored in a separate array for each sensor.
 * The calculated ranges are printed in the order that they were read.
 * (It can be seen from the values that the analog voltages will settle on a value over time.)
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
 int sonar_left = A12;    //raw analog voltage readings from the sensor
 int sonar_center = A13;
 int sonar_right = A14;
 int left_sonar[10];    //arrays to hold stored sensor readings
 int center_sonar[10];
 int right_sonar[10];
 int trigger_pin = A6;
 int left_sort_array[10]; //to hold last 10 values, will be sorted low to high then select median value
 int center_sort_array[10];
 int right_sort_array[10];
 int sort_count = 0; //track the last ten values
 int num_samples = 9;
 int sample_offset = 3;  //number of entries from the max entry of the sorted array

void setup() {
  pinMode(trigger_pin, OUTPUT);
  digitalWrite(trigger_pin, LOW);
  Serial.begin(115200);
  delay(500); //wait for serial to start
  Serial.println("HomeBot Sensor Range Test!");
}

void loop() {
  digitalWrite(trigger_pin, HIGH);   //pull HIGH to activate first sensor in the chain
  delay(20);
  digitalWrite(trigger_pin, LOW);
  delay(20);

  for (int i = 0; i < num_samples; i++) {
    left_sonar[i] = analogRead(sonar_left);
    center_sonar[i] = analogRead(sonar_center);
    right_sonar[i] = analogRead(sonar_right);
  }

      /*Print sorted range values*/
//      Serial.println("Sorted Ranges: ");
//      Serial.print("Left(cm): \t");
      sort(left_sonar, num_samples);
//      for(int i=0; i<num_samples; i++) {
//         Serial.print(left_sonar[i]/2); 
//         Serial.print("\t");
//      }
//      Serial.print("\n");
//      Serial.print("Center(cm): \t");
      sort(center_sonar, num_samples);
//      for(int i=0; i<num_samples; i++) {
//         Serial.print(center_sonar[i]/2); 
//         Serial.print("\t");
//      }
//      Serial.print("\n");
//      Serial.print("Right(cm): \t");
      sort(right_sonar, num_samples);
//      for(int i=0; i<num_samples; i++) {
//         Serial.print(right_sonar[i]/2); 
//         Serial.print("\t");
//      }
//      Serial.println("\n");


  if(sort_count < num_samples) {
    left_sort_array[sort_count] = left_sonar[num_samples - sample_offset]/2;
    center_sort_array[sort_count] = center_sonar[num_samples - sample_offset]/2;
    right_sort_array[sort_count] = right_sonar[num_samples - sample_offset]/2;
    sort_count++;
  }
  else {
    sort_count = 0;
    sort(left_sort_array, num_samples);
    sort(center_sort_array, num_samples);
    sort(right_sort_array, num_samples);
    Serial.print("Sonar Ranges: ");
    Serial.print("\t");
    Serial.print("L: ");
    Serial.print(left_sort_array[num_samples - sample_offset]);
    Serial.print("\t");
    Serial.print("C: ");
    Serial.print(center_sort_array[num_samples - sample_offset]);
    Serial.print("\t");
    Serial.print("R: ");
    Serial.print(right_sort_array[num_samples - sample_offset]);
    Serial.print("\n");
  }

  memset(left_sonar, 0, sizeof(left_sonar));    //reset arrays
  memset(center_sonar, 0, sizeof(center_sonar));
  memset(right_sonar, 0, sizeof(right_sonar));
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
