#include <M5Unified.h>  

// put function declarations here:
int myFunction(int, int);

void setup() {
  
  m5::M5Unified::config_t cfg = M5.config();
  cfg.serial_baudrate = 115200;
  M5.begin(cfg);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_YELLOW);
  M5.Lcd.setCursor(80,0);

  M5.Lcd.println("Hello World !");
  Serial.println("Hello World !");
}

void loop() {
  Serial.printf("%06lu : Loop !\n", millis());
  delay(2000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}