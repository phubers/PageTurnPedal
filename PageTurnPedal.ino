#include <Keyboard.h>

//#include <EEPROM.h>

//const int modeAddress = 0;
//int useKeys;

int upButton = 18;
int downButton = 19;

int prevUpState = 0;
int prevDownState = 0;
int homeState = 0;
  
const int debounceDelay = 20;

int debounce(int _pin) 
{
  boolean state;
  boolean previousState;

  previousState = digitalRead(_pin);
  for (int counter = 0; counter < debounceDelay; counter++) 
  {
    delay(1);
    state= digitalRead(_pin);
    if (state != previousState) {
      counter = 0;
      previousState = state;
    }
  }
  if (state == LOW)
    return 1;
  else
    return 0;
}

void setup() 
{
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
/*
  // Check whether one of the buttons is pressed while booting
  // and set the control mode accordingly
  if (true) {
    useArrows = 1;
    EEPROM.update(modeAddress, use_arrows);
  } else {
    useArrows = EEPROM.read(modeAddress);
  }
  */
  Keyboard.begin();
}


void loop() {
  int upState = debounce(upButton);
  int downState = debounce(downButton);

  boolean upChanged = upState != prevUpState;
  boolean downChanged = downState != prevDownState;
  boolean upReleased = upChanged && !upState;
  boolean downReleased = downChanged && !downState;
  
  // Check if any button has changed state & gone back to false
  if (upReleased || downReleased)
  {
    // One button released but other still pressed
    if ((upReleased && downState) || (downReleased && upState))
    {
      //Serial.print("One released, other still pressed\n");
      homeState = 1;
    }
    else if ((upReleased || downReleased) && homeState && !upState && !downState)
    {
      //Serial.print("Home\n");
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('d');
      delay(80);
      Keyboard.releaseAll();
      homeState = 0;
    }
    else if (upReleased && downReleased && !upState && !downState)
    {
      //Serial.print("Home\n");
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('d');
      delay(80);
      Keyboard.releaseAll();
      homeState = 0;      
    }
    else if (upReleased && !upState)
    {
      //Serial.print("Up\n");
      Keyboard.write(KEY_LEFT_ARROW);
   }
    else if (downReleased && !downState)
    {
      //Serial.print("Down\n");
      Keyboard.write(KEY_RIGHT_ARROW);
    }
    else
    {
      //Serial.print("Button released\n");
      //homeState = 0;
    }
  } 
  prevUpState = upState;
  prevDownState = downState;
  delay(50);
}
