#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

// I2C address for DS1307
#define DS1307_ADDR 0x68

RTC_DS1307 rtc;
/* Địa chỉ	Mục đích
0x00 - 0x06	Lưu trữ thời gian (giây, phút, giờ, ngày, thứ, tháng, năm)
0x07	      Control register
0x08 - 0x3F	RAM khả dụng cho người dùng (56 byte)
có 56 giá trị offset từ 0 đến 55 tương ứng vị trí của 56 byte trong RAM nội bộ
*/

// Write one byte to DS1307 RAM at given offset (0x00..0x3F)
void writeRtcRAM(uint8_t offset, uint8_t value) {
  Wire.beginTransmission(DS1307_ADDR);   // Bắt đầu giao tiếp với DS1307
  Wire.write(0x08 + offset);             // Ghi địa chỉ RAM muốn truy cập (bắt đầu từ 0x08)
  Wire.write(value);                     // Ghi giá trị vào địa chỉ đó
  Wire.endTransmission();                // Kết thúc giao tiếp
}

// Read one byte from DS1307 RAM at given offset
uint8_t readRtcRAM(uint8_t offset) {
  Wire.beginTransmission(DS1307_ADDR);   
  Wire.write(0x08 + offset);             // Chọn địa chỉ cần đọc
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)DS1307_ADDR, (uint8_t)1); // Yêu cầu đọc 1 byte
  if (Wire.available()) return Wire.read();           // Trả về byte đọc được
  return 0;                                           // Nếu lỗi, trả về 0
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  // I2C with custom SDA/SCL
  Wire.begin(21, 22);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, setting current time...");
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  Serial.println("DS1307 RTC Initialized");

  // Lưu báo thức 13:55 vào EEPROM của DS1307
  writeRtcRAM(0, 13);   // offset 0: giờ báo thức
  writeRtcRAM(1, 55);  // offset 1: phút báo thức

  // In ra để xác nhận
  Serial.print("Đã lưu giờ báo thức: ");
  Serial.print(readRtcRAM(0));
  Serial.print(":");
  Serial.println(readRtcRAM(1));
}

void loop() {
  DateTime now = rtc.now();

  // Hiển thị thời gian thực
  Serial.print("Date: ");
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(' ');
  Serial.print("Time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);

  // Đọc giờ/phút báo thức đã lưu
  uint8_t alarmHour = readRtcRAM(0);
  uint8_t alarmMinute = readRtcRAM(1);

  // Kiểm tra khớp báo thức (và chỉ kiểm tra 1 lần mỗi phút)
  if (now.hour() == alarmHour && now.minute() == alarmMinute && now.second() == 0) {
    Serial.println("BÁO THỨC! ");
  }

  delay(1000);
}
