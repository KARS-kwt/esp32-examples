#include <Arduino.h>
#include <ps5Controller.h>

// TODO: check the pins for the motor driver
const int leftMotorPWM = 12;    
const int leftMotorDIR = 14;   
const int leftMotorEN = 27;    

const int rightMotorPWM = 26;   
const int rightMotorDIR = 25;   
const int rightMotorEN = 33;    

const int deadzone = 15;
bool isidle = true;
void setup() {
  Serial.begin(921600);
  ps5.begin("48:18:8d:3d:f3:93");  // Replace with the MAC address of your controller
  Serial.println("Ready.");

  pinMode(leftMotorPWM, OUTPUT);
  pinMode(leftMotorDIR, OUTPUT);
  pinMode(leftMotorEN, OUTPUT);

  pinMode(rightMotorPWM, OUTPUT);
  pinMode(rightMotorDIR, OUTPUT);
  pinMode(rightMotorEN, OUTPUT);
  
}

void loop() {
  while (ps5.isConnected()) {
    isidle = false;
    // Analog stick values range from -128 to 127
    int leftStickY = ps5.LStickY();
    int rightStickY = ps5.RStickY();

    // Check if L2 is pressed for reverse functionality
    // bool reverseEnabled = ps5.L2() > 0;
    bool reverseEnabled = false;

    bool motionEnabled = ps5.R1() > 0;

    // Map analog stick values to PWM values (0 to 255)
    int leftMotorSpeed = leftStickY > deadzone ? abs(leftStickY) * 2 : 0;
    int rightMotorSpeed = rightStickY > deadzone ? abs(rightStickY) * 2 : 0;

   
    // digitalWrite(leftMotorDIR, leftStickY >= 0 || !reverseEnabled ? HIGH : LOW);
    // digitalWrite(rightMotorDIR, rightStickY >= 0 || !reverseEnabled ? HIGH : LOW);

    // Set motor speeds
    analogWrite(leftMotorPWM, leftMotorSpeed);
    analogWrite(rightMotorPWM, rightMotorSpeed);

    digitalWrite(leftMotorEN, leftMotorSpeed > 0 & motionEnabled ? HIGH : LOW);
    digitalWrite(rightMotorEN, rightMotorSpeed > 0 & motionEnabled ? HIGH : LOW);
    Serial.printf("left motor speed %d\n", leftMotorSpeed);
    Serial.printf("right motor speed %d\n", rightMotorSpeed);
    delay(20); 
  }
  
  if(!isidle){
    analogWrite(leftMotorPWM, 0);
    analogWrite(rightMotorPWM, 0);
    digitalWrite(leftMotorEN, LOW);
    digitalWrite(rightMotorEN, LOW);
    isidle = true;
  }
}
