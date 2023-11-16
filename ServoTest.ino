#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>//https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library



Servo servo1;  
Servo servo2;
Servo servo3;

// servoDriver
Adafruit_PWMServoDriver pca= Adafruit_PWMServoDriver(0x40);

// constant for microsecond conversion
int zeroDegree = 544;
int oneEightyDegree = 2400;
int angleValue = 90; // servo start at 90 when first initilise


float angleSmoothed = 0;

void smoothMoveFromAToB(float initialAngle, float finalAngle) {
  // convert to microsecond 
  initialAngle = map(initialAngle, 0, 180, 544, 2400);
  finalAngle = map(finalAngle, 0, 180, 544, 2400);
  

  if (finalAngle >= initialAngle) {
    while (initialAngle < (finalAngle-5)) { // -5 is a offset for terminating the while loop, or else, it may infinitely appraoch the number and don't exit the loop
      angleSmoothed = (0.97*initialAngle) + (0.03*finalAngle); // in microosecond
      Serial.println(angleSmoothed);
      servo1.writeMicroseconds(angleSmoothed);  
      initialAngle = angleSmoothed;
      delay(5);
    }
  } else {
    while (finalAngle < (initialAngle-5)) {  // -5 is a offset for terminating the while loop, or else, it may infinitely appraoch the number and don't exit the loop
      angleSmoothed = (0.97*initialAngle) + (0.03*finalAngle); // in microosecond
      Serial.println(angleSmoothed);
      servo1.writeMicroseconds(angleSmoothed);  
      initialAngle = angleSmoothed;
      delay(5);
    }
  }
}

long previousPotReading;

void smoothMoveWithPotentiometer() { // the function needs to be run in a loop
  long potReading = analogRead(A0);

  potReading = map(potReading, 0, 1023, 544, 2400);

  bool isTheFirstPass = true;
  if (isTheFirstPass) {
    previousPotReading = potReading;
    isTheFirstPass = false;
  }

  angleSmoothed = (0.97*potReading) + (0.03*previousPotReading);

  servo1.writeMicroseconds(angleSmoothed);  

  previousPotReading = angleSmoothed;
}


void repeatedlyMovingBetweenTwoPoint(float startPoint, float endPoint) {
  smoothMoveFromAToB(startPoint, endPoint);
  delay(5);
  smoothMoveFromAToB(endPoint, startPoint);
  delay(5);
}


void servoMoveToInputValueFromSerialMonitor() {

  while (Serial.available() == 0) {
    
  }
  Serial.print("Enter: ");
  angleValue = Serial.parseInt();  
  Serial.println(angleValue);

  

  int degree = map(angleValue, 0, 180, 544, 2400);
  servo1.writeMicroseconds(degree);  
  // pca.writeMicroseconds(0,degree);

  while (Serial.available() > 0) {
    char c = Serial.read();
  }

  delay(1000);
}





void setup() {
  servo1.attach(5); // attach servo
  // servo2.attach(6);
  // servo3.attach(7);
  Serial.begin(9600); // serial monitor begin
  pca.begin(); // servoDriver initilise
  pca.setPWMFreq(60); // servoDriver set frequency
}

void loop() {
  
  // servo1.write(105);
  // servo2.write(90);
  // servo3.write(40);

  // delay(1000);

  // servo1.write(90);
  // servo2.write(80);
  // servo3.write(90);

  // delay(1000);

  servoMoveToInputValueFromSerialMonitor();


  // smooth servo movement between angle 0 to 100
  // repeatedlyMovingBetweenTwoPoint(0, 100);

  // smoothMoveWithPotentiometer();

}