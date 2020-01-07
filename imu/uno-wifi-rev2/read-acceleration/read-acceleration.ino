#include <Arduino_LSM6DS3.h>
float x, y, z;

void setup() {
  Serial.begin(115200);
  Serial.println("IMU Acceleration Test!");
  IMU.begin();
}

void loop() {
  if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);

        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.println(z);
  }
  delay(150);
}
