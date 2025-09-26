// ESP32 Passive Buzzer Example using tone() for Wokwi compatibility

#include <Arduino.h>

const int BUZZER_PIN = 13; // Pin where passive buzzer + is connected

// Play a tone at given frequency (Hz) for a duration (ms)
void playTone(int frequency, int duration) {
  if (frequency > 0) {
    tone(BUZZER_PIN, frequency, duration); // built-in tone() function
    delay(duration);
    noTone(BUZZER_PIN); // stop tone
  } else {
    noTone(BUZZER_PIN);
    delay(duration);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Example: play C1 (32.70 Hz) for 500 ms, then silent 500 ms
  Serial.println("Nốt Đô quãng 1");
  playTone(32.70, 500);
  delay(500);

  // Play C3 (130.81 Hz) for 300 ms, then silent 1000 ms
  Serial.println("Nốt Đô quãng 3");
  playTone(130.81, 1000);
  delay(300);

  // Play C5 (523 Hz) for 300 ms, then silent 300 ms
  Serial.println("Nốt Đô quãng 5");
  playTone(523, 300);
  delay(300);

  // Rest for 1 second
  delay(1000);
}
