#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10);
  }

  delay(3000);

  Serial.println("--- THÔNG SỐ CHIP ---");
  Serial.printf("Flash size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
  Serial.printf("PSRAM size: %d MB\n", ESP.getPsramSize() / (1024 * 1024));
}

void loop()
{
  // Để trống nếu không muốn in liên tục
}