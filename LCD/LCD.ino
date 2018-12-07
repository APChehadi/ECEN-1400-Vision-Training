#include <LiquidCrystal.h>          //the liquid crystal libarry contains commands for printing to the display
#include <Wire.h>
int recieved;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);   // tell the RedBoard what pins are connected to the display
void setup() {
  Serial.begin(9600);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  lcd.begin(16, 2);                 //tell the lcd library that we are using a display that is 16 characters wide and 2 characters high
  lcd.clear();                     //clear the display
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Average time:");
  lcd.setCursor(0, 1);
  lcd.print(recieved + (String)" ms");
  delay(150);
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  recieved = Wire.read();    // receive byte as an integer
  Serial.println(recieved);         // print the integer
}




