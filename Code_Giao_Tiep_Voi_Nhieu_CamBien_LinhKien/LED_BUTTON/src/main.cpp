#include <Arduino.h>

const int LED_PIN = 12;      
const int BUTTON_PIN = 5;    

// Variables for debounce
int buttonState = HIGH;         
int lastButtonReading = HIGH;     
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // ms

bool ledState = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Setup complete. Starting blink and button control...");
}

void loop() {
  unsigned long currentMillis = millis();

  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading) {
    lastDebounceTime = currentMillis;
  }
  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        Serial.print("LED is now: ");
        Serial.println(ledState ? "ON" : "OFF");
      }
    }
  }
  lastButtonReading = reading;
}
