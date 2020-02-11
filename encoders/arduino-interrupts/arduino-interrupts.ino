/*************************
 * This program implements a simple method for reading an encoder using the analong input pins.
 * This method is useful for situations where all of the avaible external interrupt pins are not available.
 * 
 * HARDWARE:
 * MakeBlock MegaPi (based on the Arduino Mego 2560)
 * Due to the pinout configuration of the MegaPi, the external interrupt pins are occupied by other functions.
 * Pins 2, 3, 18, 19 are used for motor control. 
 * Pins 20, 21 are used by the SPI interface.
 * 
 ************************/

const byte MOTOR1 = A6;   //assign analog pins to each motor
const byte MOTOR2 = A7;
int encoder1 = 0;         //variables to count encoder disc "ticks" each time the encoder circuitry sends an interrupt pulse. 
int encoder2 = 0;
bool lastState1 = false;
bool lastState2 = false;

float slots = 20.00;      //Number of slots in encoder disc. Used to track rotation and calculate speed.


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MOTOR1, INPUT);
  pinMode(MOTOR2, INPUT);
}

void loop() {
  checkEncoders();

}

void checkEncoders() {
  if (analogRead(MOTOR1) > 900 && lastState1 == false) {
    lastState1 = HIGH;
    encoder1++;
    Serial.print("MOTOR1 H: ");
    Serial.println(encoder1);
  }
  else if (analogRead(MOTOR1) < 100 && lastState1 == true) {
    lastState1 = LOW;
    encoder1++;
    Serial.print("MOTOR1 L: ");
    Serial.println(encoder1);
  }
  
}
