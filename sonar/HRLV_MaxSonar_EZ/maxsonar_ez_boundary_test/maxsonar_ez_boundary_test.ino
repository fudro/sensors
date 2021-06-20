/*
 * DESCRIPTION:
 * This allows you to test the detection boundaries of the MaxSonar_EZ sensor.
 * 
 * HARDWARE:
 * HRLV_MaxSonar_EZ (MB1003)
 * Arduino Mega 2560
 * 
 * FUNCTIONALITY:
 * This sketch reads the range from the sensor to set a max value.
 * Then, whenever the sensor reads a value that is less than established the max value,
 * an LED will be illuminated. By using a backstop (to establish the max value)and a
 * probe object (to be detected by the sensor) the effective boundaries of the sensor
 * can be determined.
 * 
 * REFERENCE:
 * https://www.maxbotix.com/Ultrasonic_Sensors/MB1003.htm
 * 
 */

 int range = 0;
 int maxRange = 0;
 int sensorPin = A0;
 int LEDPin = 4;
 int flag = 0;
 

void setup() {
  pinMode(A0, INPUT);
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, HIGH); //Activate LED to indicate that program is starting.
  Serial.begin(115200);
  delay(500); //wait for serial to start
  Serial.println("MaxSonar_EZ Boundary Test!");
}

void loop() {
  maxRange = getRange() + 5;
  Serial.print("MaxRange: ");
  Serial.println(maxRange);
  delay(3000);
  digitalWrite(LEDPin, LOW);   //Turn off LED to show that the program is ready to for a test probe
  while(true) {
    if(maxRange - getRange() > 10 && flag == 0) {
      flag = 1;
      digitalWrite(LEDPin, HIGH);
      Serial.print("Range(cm): ");
      Serial.println(range);
      delay(100);   //Short delay to allow LED to be seen
    }
    else if(maxRange - getRange() <= 10 && flag == 1) {
      digitalWrite(LEDPin, LOW);
      range = 0;
      flag = 0;
    }
  }
}

int getRange() {
  for(int i=0; i<2; i++) {   //sensor seems to require multiple readings to return a measurement
    range = analogRead(sensorPin);
    delay(250);
  }
  range = (range/2) -5;   //Calibrate the returned value based on actual measurements
  return range;
}
