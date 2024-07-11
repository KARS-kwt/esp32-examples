#include <ArduinoJson.h>
// #include "GamePad.hpp"

#define JSON_CAPACITY 200

// GamePad gamepad;

const int left_pwm_pin = 32;   // 32
const int left_dir_pin = 25;   // 25
const int right_pwm_pin = 33;  // 33
const int right_dir_pin = 26;  // 26

const int left_forward_dir = 1;   // 1
const int right_forward_dir = 0;  // 0

const int max_pwm = 30;  // 20

void setup() {
    Serial.begin(115200);
    // gamepad.reset_allow_list();
    // gamepad.setup();
    // gamepad.reset_allow_list();
    pinMode(right_pwm_pin, OUTPUT);
    pinMode(left_pwm_pin, OUTPUT);
    pinMode(right_dir_pin, OUTPUT);
    pinMode(left_dir_pin, OUTPUT);

    digitalWrite(left_dir_pin, left_forward_dir);
    digitalWrite(right_dir_pin, right_forward_dir);
}

void stop_all() {
    analogWrite(left_pwm_pin, 0);
    analogWrite(right_pwm_pin, 0);
}

uint last_heartbeat_time = 0;
const uint heartbeat_interval = 2000;

void loop() {
    static int time_since_update = millis();

    StaticJsonDocument<JSON_CAPACITY> doc;

    if (Serial.available() > 0) {
        String json = Serial.readStringUntil('\n');
        DeserializationError error = deserializeJson(doc, json);

        if (!error) {
            time_since_update = millis();
            int left_motor = doc["left_motor"];
            int right_motor = doc["right_motor"];
            int left_pwm = map(abs(left_motor), 0, 100, 0, max_pwm);
            int right_pwm = map(abs(right_motor), 0, 100, 0, max_pwm);

            int left_dir = left_motor >= 0 ? left_forward_dir : !left_forward_dir;
            int right_dir = right_motor >= 0 ? right_forward_dir : !right_forward_dir;

            digitalWrite(left_dir_pin, left_dir);
            analogWrite(left_pwm_pin, left_pwm);

            digitalWrite(right_dir_pin, right_dir);
            analogWrite(right_pwm_pin, right_pwm);
        }
    }

    if (millis() - time_since_update > 500) {
        stop_all();
        // Console.printf("Stopping\n");
    }

    if (millis() - last_heartbeat_time > heartbeat_interval) {
        StaticJsonDocument<64> heartbeatDoc;
        heartbeatDoc["heartbeat"] = "alive";
        serializeJson(heartbeatDoc, Serial);
        Serial.println();
        last_heartbeat_time = millis();
    }

    delay(10);
}