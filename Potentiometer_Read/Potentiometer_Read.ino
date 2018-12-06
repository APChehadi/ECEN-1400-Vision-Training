unsigned long time;
int buttonStateYellow = 0; //detect button state
int ledPinYellow = 5;
int buttonStateGreen = 0;
int buttonStateRed = 0;
bool buttonReady = false; //if the game is running
int ledStartTime;
int reactionTime;
int avgTime;
bool gameStart = false; //if game has started

int potPin = A0;    // select the input pin for the potentiometer
float potVal = 0;    // variable to store the value coming from the potentiometer
float potPrev = -1; //previous potentiometer value
int difficultyTime=0;

void setup() {
  pinMode(ledPinYellow,OUTPUT); //set inputs and outputs
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(potPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  buttonStateYellow = digitalRead(2); //set button pins
  buttonStateGreen = digitalRead(3);
  buttonStateRed = digitalRead(4);
  potVal = analogRead(potPin);    // read the value from the sensor
  potVal = (potVal*(1.00/1023)); //modify potVal to be 0-1
  
  time = millis();
    if(millis()-difficultyTime > 500){ //print difficulty if changed
      if(abs((potPrev - potVal)*100) > 1){ //if difficulty changed more than 1%
         Serial.println((String)"Difficulty: " + (int)(potVal*100) + "%"); //print difficulty
         Serial.println("Press red to start");
         gameStart=false; //stop game if difficulty is changed
         digitalWrite(ledPinYellow, LOW); //turn off LED
        }
       difficultyTime = time;
       potPrev = potVal;
    }

if(buttonStateRed==1){ //start game
  gameStart=true;
}

if(gameStart==true){ //if game has been started, run this
    if(buttonReady==false){ //if the game is not already running
      delay(random(300,3000)*(2-(potVal+.5))); //set timer
      digitalWrite(ledPinYellow, HIGH);   // turn on LED
      Serial.println("Start");
      buttonReady=true;  
      time = millis();
      ledStartTime = time;
    }
  
  if(buttonReady == true){
    if(buttonStateYellow==1){
      Serial.print("Time: ");
      time = millis();
      reactionTime = time-ledStartTime;
      Serial.println(reactionTime);    //prints time since program started       
      buttonReady=false;
      digitalWrite(ledPinYellow, LOW);
    }
    delay(10); //comment
  }
} 
  
  /*Serial.print(buttonStateYellow);            // print the potentimeter value
  Serial.print(buttonStateGreen);
  Serial.println(buttonStateRed);
  Serial.println(potVal);*/
}
