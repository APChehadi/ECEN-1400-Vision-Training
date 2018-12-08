//Citations: Transmit code modified from Lecture 22
#include <LiquidCrystal.h>
#include <Wire.h>
int avgTime;
int receive;
int byteNum = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // set pins
void setup() {
  Serial.begin(9600);
  Wire.begin(45); //join i2c bus with address #45
  Wire.onReceive(receiveEvent); //register event
  lcd.begin(16, 2); //initializes lcd
  lcd.clear(); //clears lcd
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0); //sets lcd to top left
  lcd.print("Average time:");
  lcd.setCursor(0, 1); //sets lcd to bottom left
  if (byteNum == 0) { //if no data is currently being sent
    lcd.print(avgTime + (String)" ms");
  }
  delay(150);
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  if (byteNum == 0) { //if this it the first byte being recieved
    receive = Wire.read() * 100; //multiply number recieved my 100 (to format correctly)
    byteNum++; //note first value has been recieved
    Serial.println(avgTime);
  } else if (byteNum == 1) { //on second byte recieved
    avgTime = receive + Wire.read(); //add remainder value onto first value
    byteNum = 0; // prepare to recieve new data
  }
}




