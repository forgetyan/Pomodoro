#include <elapsedMillis.h>

#define RED_PIN 3  //PIN NUMBER THAT CONTROL THE RED COLOR OF OUR LED
#define GREEN_PIN 4  //PIN NUMBER THAT CONTROL THE GREEN COLOR OF OUR LED
#define BLUE_PIN 5  //PIN NUMBER THAT CONTROL THE BLUE COLOR OF OUR LED
#define BUTTON_PIN 13  //INPUT PIN NUMBER FOR THE BUTTON

//ESPert espert;
elapsedMillis timeElapsed;
// delay in milliseconds before Break Time Mode is activated
unsigned long interval = 3000;//3600000; //Every hour (interval is in milliseconds)

int val; //value of the blue pin to make it flash
bool isBusy = false; //BUSY = RED mode, Else GREEN mode
bool lastButtonPressed = 0;
bool isInTimeOutMode = 0;
unsigned long timeoutEndTime = 0;
unsigned long nextBlueChange = 0;
unsigned long nextValidButtonPress = 0;
bool isBlueOn = false;

void setup() {
  Serial.begin(9600);
  //SETUP OUR PINS
  pinMode(BUTTON_PIN, INPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  checkLightMode();  //Init our LED
}

void loop() {
  bool buttonPressed = checkButtonPressed();
  unsigned long currentMillis = millis();
  if(!isInTimeOutMode) {
    if(buttonPressed){
      isBusy = !isBusy; //That busy ?? u sure ?
      //Switch between RED or GREEN Mode
      checkLightMode();
    }
    //Break time - Time to train pipes!!
    if (timeElapsed > interval)
    {
      closeLight(); //reset light so we can flash in blue color
      isInTimeOutMode = true;
      timeoutEndTime = currentMillis + 10000;
    }
  }
  else
  {
    //Flash in BLUE for 30 seconds : i * delay = 60 * 500 = 30000 milliseconds
    if(nextBlueChange < currentMillis)
    {
      Serial.println("Change");
      nextBlueChange = currentMillis + 500;
      isBlueOn = !isBlueOn;
      Serial.println(isBlueOn);
      if(isBlueOn){
        changeColor(0, 0, 255);
      }
      else{
        changeColor(0, 0, 0);
      }
      
    }
    if(buttonPressed || timeoutEndTime < currentMillis){
      //A way to stop the break time and return to green or red light mode
      isInTimeOutMode = 0;
      timeElapsed = 0;
      checkLightMode();
    }
  }
}

//Change the Color of our LED, RGB, 0 to 255
void changeColor(int red, int green , int blue){
  // If 255, led is brighter when using digitalWrite (less voltage drop) so use digitalWrite when possible
  if (red > 0 && red < 255) {
    analogWrite(RED_PIN, red);
  }
  else{
    digitalWrite(RED_PIN, red > 0);
  }
  if (green > 0 && green < 255) {
    analogWrite(GREEN_PIN, green);
  }
  else {
    digitalWrite(GREEN_PIN, green > 0 );
  }
  if (blue > 0 && blue < 255) {
    analogWrite(BLUE_PIN, blue);
  }
  else {
    digitalWrite(BLUE_PIN, blue > 0 );
  }
}

//Check if we in busy mode or not
void checkLightMode(){
  if (isBusy){
    lightRed(); //RED mode - We are currently busy, please call again!
  }else{
    lightGreen(); //Green mode - Hi, may i help you ?
  }
}

//Turn LED off
void closeLight(){
  changeColor(0,0,0);
}

//Turn LED red
void lightRed(){
  changeColor(255,0,0);
}

//Turn LED Green
void lightGreen(){
  changeColor(0,255,0);
}

bool checkButtonPressed() {
  bool buttonPressed = 0;
  int val = digitalRead(BUTTON_PIN);
  // If the button is pressed and it wasn't last time, consider button pressed
  if(lastButtonPressed == false && val == LOW && nextValidButtonPress < millis())
  {
    buttonPressed = 1;
  }
  // When releasing button, wait at least 100 ms before triggering next button press
  if(lastButtonPressed == true && val == HIGH) {
    nextValidButtonPress = millis() + 100;
  }
  lastButtonPressed = val == LOW;
  //Serial.println(String(val));
  return buttonPressed;
}

