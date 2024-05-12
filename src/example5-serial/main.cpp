#include <Arduino.h>
#include <ArduinoJson.h>

#define JSON_CAPACITY 200
#define period 1000


void setup() {
    Serial.begin(921600);
}

void loop() {

    StaticJsonDocument<JSON_CAPACITY> doc;

    auto now = millis();

    auto velocity = 100.0 * sin(2 * PI * now / period);
    auto position = 50.0 * sin(2 * PI * now / period);

    doc["time"] = now;
    doc["velocity"] = velocity;
    doc["position"] = position;

    serializeJson(doc, Serial);
    Serial.println();
    delay(100);

}

