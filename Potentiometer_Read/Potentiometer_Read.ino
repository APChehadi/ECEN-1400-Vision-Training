#include <Wire.h>
#define piezoPin 8
#define ledPinRed 7
#define ledPinGreen 6
#define ledPinYellow 5
#define butPinRed 4
#define butPinGreen 3
#define butPinYellow 2
unsigned long time;
int buttonStateYellow = 1; //detect button state
int buttonStateGreen = 1;
int buttonStateRed = 1;
int currentButton = 1;
bool buttonReady = false; //if the game is running
int ledStartTime;
float reactionTime;
bool gameStart = false; //if game has started
int potPin = A0;    // select the input pin for the potentiometer
float potVal = 0;    // variable to store the value coming from the potentiometer
float potPrev = -1; //previous potentiometer value
int difficultyTime = 0;
float avgTime = 0;
float playTime = 0;
float turnCounter = 0;
int firstGame = 0;
int buzzDuration = 500;
int sendNum = 0;

void setup() {
  pinMode(ledPinYellow, OUTPUT); //set inputs and outputs
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  pinMode(butPinRed, INPUT_PULLUP);
  pinMode(butPinYellow, INPUT_PULLUP);
  pinMode(butPinGreen, INPUT_PULLUP);
  pinMode(potPin, INPUT);
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
  delay(700);
}

void loop() {
  buttonStateYellow = digitalRead(butPinYellow); //set button pins
  buttonStateGreen = digitalRead(butPinGreen);
  buttonStateRed = digitalRead(butPinRed);
  potVal = analogRead(potPin);    // read the value from the sensor
  potVal = (potVal * (1.00 / 1023)); //modify potVal to be 0-1

  if (gameStart == false) {
    time = millis();
    if (millis() - difficultyTime > 100) { //print difficulty if changed
      if (abs((potPrev - potVal) * 100) > 1) { //if difficulty changed more than 1%
        tone(piezoPin, 165, buzzDuration/10);
        if (firstGame > 0) {
          avgTime = (int)(playTime / turnCounter); //set avgTime
          avgTime = ((int)avgTime);
          sendNum = ((int)(avgTime / 100)); //format avgTime for 1 byte transfer
          //Serial.println(sendNum);
          Wire.beginTransmission(45); // transmit to device #8
          Wire.write(sendNum);       // sends average time in milliseconds
          Wire.endTransmission();    // stop transmitting
          delay(50);
          sendNum = ((int)avgTime % 100); //format avgTime for 1 byte transfer
          sendNum = (int)sendNum;
          //Serial.println(sendNum);
          Wire.beginTransmission(45); // transmit to device #8
          Wire.write(sendNum);       // sends average time in milliseconds
          Wire.endTransmission();    // stop transmitting
          //Serial.println((String)"Average Time: " + avgTime + " ms"); //print difficulty
          //Serial.println((String)"Average Time: " + (int)avgTime + " ms"); //print difficulty
          //Serial.println("Resetting...");
          delay(100);
          software_Reset(); //reset code
        }
        Serial.println((String)"Difficulty: " + (int)(potVal * 100) + "%"); //print difficulty
        Serial.println("Press all buttons to start");
        tone(piezoPin, 523, buzzDuration);
      }
      difficultyTime = time;
      potPrev = potVal;
    }
    if (buttonStateRed == 0) { //start game
      if (buttonStateGreen == 0) {
        if (buttonStateYellow == 0) {
          gameStart = true;
          turnCounter = 0;
          playTime = 0;
          firstGame++;
          delay(10);
        }
      }
    }
  }
  if (gameStart == true) { //if game has been started, run this
    if (buttonReady == false) { //if the game is not already running
      if (buttonStateYellow == 1) {
        if (buttonStateGreen == 1) {
          if (buttonStateRed == 1) {
            randomSeed(millis());
            currentButton = random(0, 3);
            delay(random(10, 100) * (11 - (10 * potVal) + .5)); //set timer
            delay(10);
            time = millis();
            ledStartTime = time;
            buttonReady = true;
            if (currentButton == 0) {
              digitalWrite(ledPinYellow, HIGH);   // turn on LED
            } else if (currentButton == 1) {
              digitalWrite(ledPinGreen, HIGH);   // turn on LED
            } else if (currentButton == 2) {
              digitalWrite(ledPinRed, HIGH);   // turn on LED
            }
          }
        }
      }
    }
    if (buttonReady == true) {
      if (buttonStateYellow == 0) {
        if (currentButton == 0) {
          time = millis();
          reactionTime = time - ledStartTime;
          digitalWrite(ledPinYellow, LOW);
          buttonReady = false;
          tone(piezoPin, 392, buzzDuration);
          turnCounter++;
          playTime = playTime + reactionTime;
          delay(100);
        } else {
          gameStart = false;
          buttonReady = false;
          digitalWrite(ledPinYellow, LOW); //turn off LED
          digitalWrite(ledPinGreen, LOW); //turn off LED
          digitalWrite(ledPinRed, LOW); //turn off LED
          //Serial.println("End Round");
          tone(piezoPin, 131, buzzDuration);
          potPrev = -1;
          difficultyTime = 0;
          delay(100);
        }
      }
      if (buttonStateGreen == 0) {
        if (currentButton == 1) {
          time = millis();
          reactionTime = time - ledStartTime;
          digitalWrite(ledPinGreen, LOW);
          buttonReady = false;
          tone(piezoPin, 329, buzzDuration);
          turnCounter++;
          playTime = playTime + reactionTime;
          delay(100);
        } else {
          gameStart = false;
          buttonReady = false;
          digitalWrite(ledPinYellow, LOW); //turn off LED
          digitalWrite(ledPinGreen, LOW); //turn off LED
          digitalWrite(ledPinRed, LOW); //turn off LED
          //Serial.println("End Round");
          tone(piezoPin, 131, buzzDuration);
          potPrev = -1;
          difficultyTime = 0;
          delay(100);
        }
      }
      if (buttonStateRed == 0) {
        if (currentButton == 2) {
          time = millis();
          reactionTime = time - ledStartTime;
          digitalWrite(ledPinRed, LOW);
          buttonReady = false;
          tone(piezoPin, 261, buzzDuration);
          turnCounter++;
          playTime = playTime + reactionTime;
          delay(100);
        } else {
          gameStart = false;
          buttonReady = false;
          digitalWrite(ledPinYellow, LOW); //turn off LED
          digitalWrite(ledPinGreen, LOW); //turn off LED
          digitalWrite(ledPinRed, LOW); //turn off LED
          //Serial.println("End Round");
          tone(piezoPin, 131, buzzDuration);
          potPrev = -1;
          difficultyTime = 0;
          delay(100);
        }
      }
    }
  }
}
void software_Reset() // Restarts program
{
  asm volatile ("  jmp 0");
}
