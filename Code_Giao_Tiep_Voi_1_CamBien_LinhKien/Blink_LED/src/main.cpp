#include <Arduino.h>

#define ledPin 2

void blinkLed(uint8_t pin, int delayMs = 1000);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  blinkLed(ledPin);
}

// put function definitions here:
void blinkLed(uint8_t pin, int delayMs) {
  pinMode(pin, OUTPUT);

  digitalWrite(pin, HIGH);
  delay(delayMs);
  digitalWrite(pin, LOW);
  delay(delayMs);
}
