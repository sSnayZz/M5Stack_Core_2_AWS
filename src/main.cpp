#include <Arduino.h>
#include <M5Unified.h>  

// put function declarations here:
int myFunction(int, int);

void setup() {
  M5.begin();

  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_YELLOW);
  M5.Lcd.setCursor(80,0);

  M5.Lcd.println("Hello World !");
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}