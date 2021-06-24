#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("IMU Test!"); 
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Display the floating point data */
  Serial.print("YAW/COMPASS(X): ");  //Based on "top-down" view: Clockwise subtracts from the current angle, Counte-Clockwise increases the current angle. 
                                  //Angle is based on 360 degrees with the ZERO based on the position/angle of the sensor at the time of sensor initialization.
  Serial.print(event.orientation.x, 2); //second parameter sets the number of decimal places
  Serial.print("\t");
  Serial.print("ROLL(Y): "); //Based on the Sonar Unit being the "front" of the robot: 
                          //Roll to Left (CCW) = Negative Value, Roll to Right (CW) = Positive Value
  Serial.print(event.orientation.y, 2);
  Serial.print("\t");
  Serial.print("PITCH(Z): ");  //Based on the Sonar Unit being the "front" of the robot: 
                            //Pitch Forward = Negative Value (going lower, "downhill"), Pitch Backward = Positive Value (going higher, "uphill")
  Serial.print(event.orientation.z, 2);
  Serial.print("\n");
  
  delay(100);
}
