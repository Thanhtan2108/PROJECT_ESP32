#include <Arduino.h>

#include <WiFi.h>               // Thư viện WiFi
#include <FirebaseESP32.h>      // Thư viện Firebase

#include <Wire.h>               // Thư viện I2C
#include <Adafruit_GFX.h>       // Thư viện đồ họa
#include <Adafruit_SSD1306.h>   // Thư viện OLED SSD1306


//==================CONFIG WIFI==============================
// Cấu hình WiFi
#define WIFI_SSID "Thanh Tan"      // Thay bằng SSID WiFi của bạn
#define WIFI_PASSWORD "daquenmatroi"  // Thay bằng password WiFi
int retryCount = 0;          // Đếm số lần retry nếu WIFI/Firebase lỗi


//==================CONFIG FIREBASE===========================
#define FIREBASE_HOST "https://dispaly-text-on-oled-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "7wJ8gTpI7Iz3e21V2V7Peq1DvlXw6KjFGwbndjKr"

// Tạo đối tượng Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


//==================CONFIG OLED 0.96 INCH===================
#define SCREEN_WIDTH 128  // Chiều rộng màn hình (pixels)
#define SCREEN_HEIGHT 64  // Chiều cao màn hình (pixels)

// Khởi tạo OLED với I2C (SDA=21, SCL=22 mặc định, địa chỉ 0x3C)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//==================DEFINE CODE====================================
// Biến lưu dữ liệu từ Firebase
String displayText = "";
String lastDisplayText = "";  // Biến lưu giá trị trước đó để tránh nhấp nháy


void setup() {
  Serial.begin(115200);  // Khởi tạo Serial Monitor

  //===================CONNECT WIFI=========================
  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Kết nối WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (retryCount++ > 10) {  // Giới hạn 5 giây (10 * 500ms)
      Serial.println("\nKhông thể kết nối WiFi!");
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi kết nối thành công!");
  }

  //===================Khởi tạo Firebase====================
  // Cấu hình Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  // Kết nối Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  //===================CODE=================================
  // Khởi tạo I2C với chân SDA=21, SCL=22 (mặc định ESP32)
  Wire.begin(21, 22);  // Nếu dùng chân khác, thay đổi ở đây

  // Kiểm tra kết nối OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Địa chỉ 0x3C cho 128x64
    Serial.println(F("Lỗi khởi tạo SSD1306! Kiểm tra kết nối."));
    for (;;);  // Dừng chương trình nếu lỗi
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
  Serial.println("OLED khởi tạo thành công!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && Firebase.ready()) {
    if (Firebase.getString(fbdo, "/DISPLAY_TEXT/TEXT")) {
      displayText = fbdo.stringData();
      Serial.print("TEXT từ Firebase: ");
      Serial.println(displayText);

      // Chỉ cập nhật OLED nếu text thay đổi
      if (displayText != lastDisplayText) {
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println(displayText);
        display.display();
        lastDisplayText = displayText;
      }
      retryCount = 0;  // Reset retry count khi thành công
    } else {
      Serial.print("Lỗi đọc TEXT: ");
      Serial.println(fbdo.errorReason());
      if (retryCount++ < 5) {  // Retry 5 lần trước khi hiển thị lỗi
        delay(1000);
        return;
      }
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Loi ket noi FB");
      display.display();
    }
  } else {
    Serial.println("Firebase hoặc WiFi không sẵn sàng!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Chua ket noi FB");
    display.display();
    delay(1000);
  }

  delay(1000);  // Đọc lại sau 1 giây
}
