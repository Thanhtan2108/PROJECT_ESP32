#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "HCSR04.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/adc.h"
#include "Melody.h"

// ==== Định nghĩa chân và thông số ====
#define TRIG_PIN 23
#define ECHO_PIN 5

#define MQ2_SENSOR_PIN ADC1_CHANNEL_4  // GPIO32

#define BUZZER_PIN 15
#define BUZZER_CHANNEL 0

#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

#define TIMER_PERIOD pdMS_TO_TICKS(1000)  // 5 giây

// button_led
#define BTN_ON 2
#define BTN_OFF 18
#define LED_PIN 14

// Semaphore
#define BUTTON_SEM_PIN  27
#define LED_BLINK_PIN   12
#define DEBOUNCE_DELAY_MS 50
#define BLINK_COUNT 3
#define LED_ON_TIME  500
#define LED_OFF_TIME 500

// ==== LCD và cảm biến siêu âm ====
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);
UltraSonicDistanceSensor distanceSensor(TRIG_PIN, ECHO_PIN);

SemaphoreHandle_t btnSemaphore = NULL;
int mq2SensorValue = 0;

// ==== Task đọc cảm biến MQ2 (ADC) ====
void readMQ2SensorTask(void *pvParameters)
{
    while (1) {
        mq2SensorValue = adc1_get_raw((adc1_channel_t)MQ2_SENSOR_PIN);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ==== Hàm callback timer ====
void timerCallback(TimerHandle_t xTimer)
{
    double distance = distanceSensor.measureDistanceCm();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MQ2:");
    lcd.print(mq2SensorValue);

    lcd.setCursor(0, 1);
    if (isnan(distance)) {
        lcd.print("Distance err");
    } else {
        lcd.print("D:");
        lcd.print(distance, 2);
        lcd.print(" cm");
    }
}

// ==== Task điều khiển buzzer ====

// Tempo and timing
int tempo = 200;
int wholenote;

// Task function to play melody once
void TaskPlayMelody(void* pvParameters) {
  // Compute wholenote duration
  wholenote = (60000 * 4) / tempo;

  // Lấy trực tiếp biến numNotes_Lib đã được định nghĩa trong Melody.h do dùng extern
  for (int i = 0; i < numNotes_Lib * 2; i += 2) {
    int note = melody[i];
    int durationCode = melody[i + 1];
    int noteDuration;

    if (durationCode > 0) {
      noteDuration = wholenote / durationCode;
    } else {
      noteDuration = (wholenote / abs(durationCode)) * 1.5;
    }

    // Play tone for 90% of duration
    tone(BUZZER_PIN, note, noteDuration * 0.9);
    vTaskDelay((noteDuration) / portTICK_PERIOD_MS);
    noTone(BUZZER_PIN);
  }

  // Suspend task when done
  vTaskSuspend(NULL);
}

// ==== Task điều khiển LED bằng nút ON/OFF ====

void ledControlTask(void *pvParameters) {
    bool lastBtnOnState  = LOW;
    bool lastBtnOffState = LOW;
    bool stableBtnOnState  = LOW;
    bool stableBtnOffState = LOW;

    TickType_t lastDebounceOnTime  = 0;
    TickType_t lastDebounceOffTime = 0;

    while (1) {
        TickType_t currentTime = xTaskGetTickCount();
        bool currentOnReading  = digitalRead(BTN_ON);
        bool currentOffReading = digitalRead(BTN_OFF);

        // Xử lý nút BTN_ON
        if (currentOnReading != lastBtnOnState) {
            lastDebounceOnTime = currentTime;
        }
        if ((currentTime - lastDebounceOnTime) >= pdMS_TO_TICKS(DEBOUNCE_DELAY_MS)) {
            if (currentOnReading != stableBtnOnState) {
                stableBtnOnState = currentOnReading;
                if (stableBtnOnState == HIGH) {
                    digitalWrite(LED_PIN, HIGH);
                    Serial.println("LED ON");
                }
            }
        }
        lastBtnOnState = currentOnReading;

        // Xử lý nút BTN_OFF
        if (currentOffReading != lastBtnOffState) {
            lastDebounceOffTime = currentTime;
        }
        if ((currentTime - lastDebounceOffTime) >= pdMS_TO_TICKS(DEBOUNCE_DELAY_MS)) {
            if (currentOffReading != stableBtnOffState) {
                stableBtnOffState = currentOffReading;
                if (stableBtnOffState == HIGH) {
                    digitalWrite(LED_PIN, LOW);
                    Serial.println("LED OFF");
                }
            }
        }
        lastBtnOffState = currentOffReading;

        vTaskDelay(pdMS_TO_TICKS(10));  // Delay nhỏ để phản hồi nhanh
    }
}


// ==== Task đọc nút và cấp semaphore ====
void btnSemaphoreTask(void *pvParameters) {
    bool buttonState = LOW;
    bool lastReading = LOW;
    TickType_t lastDebounceTime = 0;

    while(1) {
        TickType_t currentTime = xTaskGetTickCount();
        bool currentReading = digitalRead(BUTTON_SEM_PIN);

        if (currentReading != lastReading) {
            lastDebounceTime = currentTime;
        }

        if ((currentTime - lastDebounceTime) >= pdMS_TO_TICKS(DEBOUNCE_DELAY_MS)) {
            if (currentReading != buttonState) {
                buttonState = currentReading;
                if (buttonState == HIGH) {
                    if (xSemaphoreGive(btnSemaphore) == pdTRUE) {
                        Serial.println("Semaphore given!");
                    }
                }
            }
        }

        lastReading = currentReading;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// ==== Task blink LED khi nhận được semaphore ====
void ledBlinkTask(void *pvParameters) {
    while(1) {
        if (xSemaphoreTake(btnSemaphore, portMAX_DELAY) == pdTRUE) {
            Serial.println("Semaphore taken, start LED blink!");
            for (int i = 0; i < BLINK_COUNT; i++) {
                digitalWrite(LED_BLINK_PIN, HIGH);
                vTaskDelay(pdMS_TO_TICKS(LED_ON_TIME));
                digitalWrite(LED_BLINK_PIN, LOW);
                vTaskDelay(pdMS_TO_TICKS(LED_OFF_TIME));
            }
            Serial.println("LED blink complete.");
        }
    }
}

// ==== Setup ====
void setup()
{
    Serial.begin(115200);

    pinMode(BTN_ON, INPUT_PULLDOWN);
    pinMode(BTN_OFF, INPUT_PULLDOWN);
    pinMode(LED_PIN, OUTPUT);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(MQ2_SENSOR_PIN, ADC_ATTEN_DB_0);

    lcd.init();
    lcd.setBacklight(1);

    pinMode(BUZZER_PIN, OUTPUT);

    TimerHandle_t xTimer = xTimerCreate("SensorTimer", TIMER_PERIOD, pdTRUE, (void *)0, timerCallback);
    if (xTimer != NULL) {
        xTimerStart(xTimer, 0);
    }

    pinMode(BUTTON_SEM_PIN, INPUT_PULLDOWN);
    pinMode(LED_BLINK_PIN, OUTPUT);
    btnSemaphore = xSemaphoreCreateCounting(5, 0);
    if (btnSemaphore == NULL) {
        Serial.println("Failed to create semaphore!");
        while (1);
    }

    // Tạo các task
    xTaskCreate(readMQ2SensorTask, "ReadMQ2SensorTask", 2048, NULL, 2, NULL);
    xTaskCreate(TaskPlayMelody, "PlayMelody", 2048, NULL, 2, NULL);
    xTaskCreate(ledControlTask, "LED Control", 2048, NULL, 3, NULL);
    xTaskCreate(btnSemaphoreTask, "BtnSemaphoreTask", 2048, NULL, 2, NULL);
    xTaskCreate(ledBlinkTask, "LED_BlinkTask", 2048, NULL, 2, NULL);
}

void loop() {
    // Không sử dụng loop trong FreeRTOS
}
