#include <Arduino.h>

#include <Wire.h>               // Thư viện I2C
#include <Adafruit_GFX.h>       // Thư viện đồ họa
#include <Adafruit_SSD1306.h>   // Thư viện OLED SSD1306

//==================CONFIG OLED 0.96 INCH===================
#define SCREEN_WIDTH 128  // Chiều rộng màn hình (pixels)
#define SCREEN_HEIGHT 64  // Chiều cao màn hình (pixels)

// Khởi tạo OLED với I2C (SDA=21, SCL=22 mặc định, địa chỉ 0x3C)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);  // Khởi tạo Serial Monitor

  // Khởi tạo I2C với chân SDA=21, SCL=22 (mặc định ESP32)
  Wire.begin(21, 22);  // Nếu dùng chân khác, thay đổi ở đây

  // Kiểm tra kết nối OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Địa chỉ 0x3C cho 128x64
    Serial.println(F("Lỗi khởi tạo SSD1306! Kiểm tra kết nối."));
    for (;;);  // Dừng chương trình nếu lỗi
  }

  Serial.println("OLED khởi tạo thành công!");
  delay(2000);  // Đợi 2 giây

  display.clearDisplay();  // Xóa màn hình

  // Cài đặt text
  display.setTextSize(1);      // Kích thước font (1 = nhỏ nhất)
  display.setTextColor(SSD1306_WHITE);  // Màu trắng
  display.setCursor(0, 10);    // Vị trí bắt đầu (x=0, y=10)

  // Hiển thị text
  display.println("Hello, world!");
  display.display();  // Cập nhật màn hình

  Serial.println("Hiển thị text trên OLED!");
}

void loop() {
  // Code lặp lại (có thể thêm logic động ở đây)
  // Ví dụ: Hiển thị thời gian hoặc dữ liệu sensor
  delay(1000);
}
