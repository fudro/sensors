/*
 * DESCRIPTION:
 * This sketch reads the range from the sensor and prints the value to the Serial Monitor.
 * 
 * HARDWARE:
 * HRLV_MaxSonar_EZ
 * Arduino Mega 2560
 * 
 * FUNCTIONALITY:
 * VCC power is supplied to the sensor. The analog voltage output from the sensor
 * is proportional to the input voltage. The formula for the range increment is:
 * Vi = Vcc/1024
 * where Vi is measured as Volts / 5mm
 * 
 * The range is calulated as:
 * 5*Vm/Vi
 * where Vm is the measured voltage from the sensor.
 * For a 5v supply, this formula can be simplified to: Vm * 1024
 * 
 * In practice, when using 5v USB power, the sensor gives accurate readings using the formula: 
 * Range(mm) = Vm * 1000
 * where distance is measured from the FRONT face of the sensor.
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

 int vcc = 5;
 int rangeFactor = 1000;
 int range = 0;
 int sensorPin = A0;
 int triggerPin = 4;
 int flag = 0;
 

void setup() {
  pinMode(A0, INPUT);
  pinMode(triggerPin, INPUT);
  Serial.begin(115200);
  delay(500); //wait for serial to start
  Serial.println("MaxSonar_EZ Range Test!");
}

void loop() {
  if(digitalRead(triggerPin) == HIGH) {
    delay(10);
    if(digitalRead(triggerPin) == HIGH) {
      flag = 1;
      range = analogRead(sensorPin);
    }
  }
  else if(digitalRead(triggerPin) == LOW && flag == 1) {
    delay(10);
    if(digitalRead(triggerPin) == LOW) {
      Serial.print("Range(cm): ");
      Serial.println(range/2);
      flag = 0;
      range = 0;
    } 
  }
}
