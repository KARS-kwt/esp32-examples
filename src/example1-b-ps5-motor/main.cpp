#include <ps5Controller.h>
#include <Arduino.h>

// TODO: check the pins for the motor driver
const int leftMotorPWM = 32;    
const int leftMotorDIR = 33;   
const int leftMotorEN = 34;    

const int rightMotorPWM = 25;   
const int rightMotorDIR = 26;   
const int rightMotorEN = 27;    

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
    // Analog stick values range from -128 to 127
    int leftStickY = ps5.LStickY();
    int rightStickY = ps5.RStickY();

    // Check if L2 is pressed for reverse functionality
    bool reverseEnabled = ps5.L2() > 0;
    bool motionEnabled = ps5.R2() > 0;

    // Map analog stick values to PWM values (0 to 255)
    int leftMotorSpeed = abs(leftStickY) * 2;  // Use absolute value and scale
    int rightMotorSpeed = abs(rightStickY) * 2;  // Use absolute value and scale

    // Set motor directions based on analog stick position and L2 button
    digitalWrite(leftMotorDIR, leftStickY >= 0 || !reverseEnabled ? HIGH : LOW);
    digitalWrite(rightMotorDIR, rightStickY >= 0 || !reverseEnabled ? HIGH : LOW);

    // Set motor speeds
    analogWrite(leftMotorPWM, leftMotorSpeed);
    analogWrite(rightMotorPWM, rightMotorSpeed);

    digitalWrite(leftMotorEN, leftMotorSpeed > 0 & motionEnabled ? HIGH : LOW);
    digitalWrite(rightMotorEN, rightMotorSpeed > 0 & motionEnabled ? HIGH : LOW);

    delay(40); 
  }
}
