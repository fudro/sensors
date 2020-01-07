#include <Arduino_LSM6DS3.h>
float x, y, z;

void setup() {
  Serial.begin(115200);
  Serial.println("IMU Gyroscope Test!");
  IMU.begin();
}

void loop() {
  if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);

        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.println(z);
  }
  delay(150);
}
