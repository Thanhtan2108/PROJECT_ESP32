# Sơ lược

## Sơ đồ chân

| Pin OLED | Pin ESP32 DEVKIT V1 |
|----------|---------------------|
| VCC      | 3.3V                |
| GND      | GND                 |
| SCL      | GPIO 22             |
| SDA      | GPIO 21             |

## Cài đặt các thư viện

- Adafruit SSD1306

- (Có thể dùng U8g2) để thay cho SSD1306

- Adafruit_GFX

- Firebase ESP32 Client

- (Có thể dùng Firebase Arduino Client Library for ESP8266 and ESP32) thay cho Firebase ESP32 Client

## Hướng dẫn thiết lập

### Thêm các thư viện

```c
#include <WiFi.h>               // Thư viện WiFi
#include <FirebaseESP32.h>      // Thư viện Firebase

#include <Wire.h>               // Thư viện I2C
#include <Adafruit_GFX.h>       // Thư viện đồ họa
#include <Adafruit_SSD1306.h>   // Thư viện OLED SSD1306
```

### Cấu hình cho WIFI

Ở đầu chương trình, sau khai báo thư viện

```c
// Cấu hình WiFi
#define WIFI_SSID "YOUR_WIFI_SSID"      // Thay bằng SSID WiFi của bạn
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"  // Thay bằng password WiFi
int retryCount = 0;          // Đếm số lần retry nếu WIFI/Firebase lỗi
```

Trong void setup(), sau khi khởi tạo serial

```c
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
```

### Cấu hình cho Firebase

Ở đầu chương trình, bên dưới cấu hình WIFI

```c
#define FIREBASE_HOST "link dẫn đến thư mục quản lý Realtime Database"
#define FIREBASE_AUTH "Database secrets"
```

Tại `Realtime Database` -> copy link nằm ở trên thư mục quản lý Database -> paste vào `FIREBASE_HOST`

Tại `Project settings` -> tab `service accounts` -> `Database secrets` -> `Show` -> copy mã -> paste vào `FIREEBASE_AUTH`

Trông void setup(), bên dưới kết nối WIFI

```c
  // Cấu hình Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  // Kết nối Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
```

## Làm việc với Firebase

### Trạng thái kêt nối với Firebase

Để kiểm tra xem trạng thái kết nối `ESP32` với `WIFI` và `Firebase` đã ok chưa

```c
if (WiFi.status() == WL_CONNECTED && Firebase.ready()){}
```

### Lấy dữ liệu là String

Dùng `Firebase.getString(fbdo, "/đường dẫn đến biến cần lấy dữ liệu")`

Dữ liệu được lấy về đã được lưu trong `fbdo.stringData()`

Để dùng để giao tiếp `ESP32` với `Firebase` cần tạo 1 biến chứa giá trị này

```c
String isplayText = fbdo.stringData();
```

### Đọc dữ liệu lỗi, không đọc được

```c
Serial.println(fbdo.errorReason());
```
