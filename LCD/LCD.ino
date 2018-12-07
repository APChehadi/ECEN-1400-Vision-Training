#include <LiquidCrystal.h>
#include <Wire.h>
int avgTime;
int recieve;
int byteNum = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() {
  Serial.begin(9600);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Average time:");
  lcd.setCursor(0, 1);
  if (byteNum==0) {
    lcd.print(avgTime + (String)" ms");
  }
  delay(150);
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  if (byteNum == 0) {
    recieve = Wire.read() * 100;
    byteNum++;
    Serial.println(avgTime);
  } else if (byteNum == 1) {
    avgTime = recieve + Wire.read();
    byteNum = 0;
  }
}
