#include <Arduino.h>
#include <ArduinoJson.h>

const int leftMotorPWN = 12;
const int leftMotorDir = 14;
const int rightMotorPWN = 27;
const int rightMotorDir = 26;

void handleSerialData(const String &data);
void sendHeadingData();

void setup()
{
    Serial.begin(921600);

    pinMode(leftMotorPWN, OUTPUT);
    pinMode(leftMotorDir, OUTPUT);
    pinMode(rightMotorPWN, OUTPUT);
    pinMode(rightMotorDir, OUTPUT);
}

void loop()
{
    static char inputBuffer[256];
    static int inputPos = 0;

    if (Serial.available() > 0)
    {
        String inputString = Serial.readStringUntil('\n');
        handleSerialData(inputString);
    }

    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 20)
    {
        sendHeadingData();
        lastSendTime = millis();
    }
}

void handleSerialData(const String &data)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    if (error)
    {
        Serial.println("Failed to parse JSON");
        return;
    }

    float leftSpeed = doc["left_speed"];
    float rightSpeed = doc["right_speed"];

    if (leftSpeed > 0) {
        digitalWrite(leftMotorDir, HIGH);
    } else {
        digitalWrite(leftMotorDir, LOW);
        leftSpeed = -leftSpeed;
    }
    if (rightSpeed > 0) {
        digitalWrite(rightMotorDir, HIGH);
    } else {
        digitalWrite(rightMotorDir, LOW);
        rightSpeed = -rightSpeed;
    }

    int leftPWM = map(leftSpeed, 0, 1, 0, 255);
    int rightPWM = map(rightSpeed, 0, 1, 0, 255);
}
void sendHeadingData()
{
    JsonDocument doc;

    int heading = random(0, 360);
    doc["heading"] = heading;
    serializeJson(doc, Serial);
    Serial.println();
}
