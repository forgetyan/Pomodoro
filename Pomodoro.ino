#include <ESPert.h>
#include <elapsedMillis.h>

#define RED 3  //PIN NUMBER THAT CONTROL THE RED COLOR OF OUR LED
#define GREEN 4  //PIN NUMBER THAT CONTROL THE GREEN COLOR OF OUR LED
#define BLUE 5  //PIN NUMBER THAT CONTROL THE BLUE COLOR OF OUR LED

ESPert espert;
elapsedMillis timeElapsed;
// delay in milliseconds before Break Time Mode is activated
unsigned int interval = 3600000; //Every hour (interval is in milliseconds)

int val; //value of the blue pin to make it flash
bool isBusy = false; //BUSY = RED mode, Else GREEN mode

void setup() {
  espert.init();
  //SETUP OUR PINS
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  checkLightMode();  //Init our LED
}

void loop() {
  if (espert.button.isOn()){
    isBusy = !isBusy; //That busy ?? u sure ?
    //Switch between RED or GREEN Mode
    checkLightMode();
    delay(500);
  }
  if (timeElapsed > interval)
  {
    //Flash in BLUE for 30 seconds : i * delay = 60 * 500 = 30000 milliseconds
    //Break time - Time to train pipes!!
    closeLight(); //reset light so we can flash in blue color
    for ( int i = 0; i < 60; i++ ) {
      if (espert.button.isOn()){
        //A way to stop the break time and return to green or red light mode
        delay(100);
        break;
      }
      val = ( val == 0 ) ? 255 : 0;
      changeColor( 0, 0, val);  //Play with the blue!! Flash tes lumières si tu t'ennuies!
      delay ( 500 );
    }
    checkLightMode();
    timeElapsed = 0;       // reset the counter to 0 so the counting starts over...
  }
}

//Change the Color of our LED, RGB, 0 to 255
void changeColor(int red, int green , int blue){
  analogWrite( RED, red );
  analogWrite( GREEN, green );
  analogWrite( BLUE, blue );
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
