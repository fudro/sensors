/*
 * DESCRIPTION:
 * This sketch reads the ranges from three "chained together" sonar sensors. 
 * The sensor are arranged at different angles to created a pattern of 5 overlapping detection zones.
 * The sketch illuminates a set of four LEDs to show which zone has been activated.
 * 
 * HARDWARE:
 * HRLV_MaxSonar_EZ
 * Arduino Mega 2560
 * Four LEDs: Blue, Green, Green, Red
 * 
 * FUNCTIONALITY:
 * Two Green LEDs represent the center zone.
 * Blue and Red LEDs represent the left and right zones respectively.
 * When the center zone is activated, both Green LEDs will illuminate.
 * When an overlapping zone between the center and one of the side zones is activated,
 * the Green LED nearest that side PLUS the colored LED representing that side are illuminated.
 * When either side zone is activated, only the colored LED representing that side will illuminate.
 * 
 * NOTES:
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

 int last_left = 0;    //last range value
 int last_center = 0;
 int last_right = 0;
 int new_left = 0;    //current range value
 int new_center = 0;
 int new_right = 0;
 int sensor_left = A0;    //raw analog voltage readings from the sensor
 int sensor_center = A1;
 int sensor_right = A2;
 int left_value[10];    //arrays to hold stored sensor readings
 int center_value[10];
 int right_value[10];
 int threshold = 10;
 int blue_LED = 5;
 int green_left_LED = 6;
 int green_right_LED = 7;
 int red_LED = 8;
 int blue_state = 0;
 int green_state = 0;
 int red_state = 0;
 int buttonPin = 4;  //manually activate sensor (pulled LOW when button pressed)
 int triggerPin = 9;
 int flag = 0;  //control state and prevent multiple triggers
 

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); //each button press will pull this pin LOW through a 10K resistor.
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);  //Initialize pin LOW. Pull HIGH to activate sensors.
  Serial.begin(115200);
  delay(500); //wait for serial to start
  Serial.println("MaxSonar_EZ Chained Sensor Range Test!");
}

void loop() {
  getRanges();
}

int getRanges() {
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
  new_left = left_value[8]/2;
  new_center = center_value[8]/2;
  new_right = right_value[8]/2;

  //if no values have been stored, store them
  if(last_left == 0) {
    last_left = new_left;
  }
  if(last_center == 0) {
    last_center = new_center;
  }
  if(last_right == 0) {
    last_right = new_right;
  }

  //check if the new ranges pass the threshold of change
  if(last_left - new_left > threshold) {   //if new reading is less than old reading and passes the threshold
    last_left = new_left;   //update stored reading
    blue_state = 1;   //set zone state
  }
  else if(last_left - new_left < -threshold) { //if new reading is greater than last reading and passes the threshold 
    last_left = new_left;   //update stored reading
    blue_state = 0;   //set zone state
  }
  
  if(last_center - new_center > threshold) {
    last_center = new_center;
    green_state = 1;
  }
  else if(last_center - new_center < 0) {
    last_center = new_center;
    green_state = 0;
  }
  
  if(last_right - new_right > threshold) {
    last_right = new_right;
    red_state = 1;
  }
  else if(last_right - new_right< -threshold) {
    last_right = new_right;
    red_state = 0;
  }

  if(blue_state == 1 && green_state == 1) {
    digitalWrite(blue_LED, HIGH);
    digitalWrite(green_left_LED, HIGH);
    digitalWrite(green_right_LED, LOW);
    digitalWrite(red_LED, LOW);
  }
  else if(red_state == 1 && green_state == 1) {
    digitalWrite(green_right_LED, HIGH);
    digitalWrite(red_LED, HIGH);
    digitalWrite(blue_LED, LOW);
    digitalWrite(green_left_LED, LOW);
  }
  else if(blue_state == 1 && green_state + red_state == 0) {
    digitalWrite(blue_LED, HIGH);
    digitalWrite(green_left_LED, LOW);
    digitalWrite(green_right_LED, LOW);
    digitalWrite(red_LED, LOW);
  }
  else if(red_state == 1 && green_state + blue_state == 0) {
    digitalWrite(red_LED, HIGH);
    digitalWrite(blue_LED, LOW);
    digitalWrite(green_left_LED, LOW);
    digitalWrite(green_right_LED, LOW);
  }
  else if(green_state == 1 && blue_state + red_state == 0) {
    digitalWrite(green_left_LED, HIGH);
    digitalWrite(green_right_LED, HIGH);
    digitalWrite(red_LED, LOW);
    digitalWrite(blue_LED, LOW);
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

  /*Print selected range values*/
  Serial.println("Max Ranges");
  Serial.print("left_value: ");
  Serial.println(last_left);
  Serial.print("center_value: ");
  Serial.println(last_center);
  Serial.print("right_value: ");
  Serial.println(last_right);
  Serial.println("\n");
  
  memset(left_value, 0, sizeof(left_value));    //reset arrays
  memset(center_value, 0, sizeof(center_value));
  memset(right_value, 0, sizeof(right_value));
  delay(250);
}
