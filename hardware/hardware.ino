#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"
#include <string.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define SCROLLSPEED 1

#define ROTARY_ENCODER_A_PIN 5
#define ROTARY_ENCODER_B_PIN 17
#define ROTARY_ENCODER_BUTTON_PIN 16
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 2
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

void copyArray(char  array1[100][32], char array2[100][32], int arraySize);

int x, minX;
bool interruptHappened = false;

int numItems = 1;
int state=0;
char menuItems[100][32] = {"Passwords", "Long message", "Very long message", "Very very long message", "short"};
int numState0=1;

char state0[1][32]={"Enter the master password"};


int numState1=39;
char state1[50][32]={"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","@",".","OK"};
char state1Input[32]="";


int numState3 = 5;
char state3[5][32]={"Login","Add","Backup","Reset","Exit"};
int value=0;


/*
0 "Enter master pass"
1 Take master pass input. onclick check master
2 "Incorrect" (one more variable for keeping track of number of incorrect)
3 "login", "add", "backup", "reset", "exit"
4 website name

*/
void setup() {
  Serial.begin(115200);
  setupDisplay();
  setupRotaryEncoder();  
  copyArray(menuItems, state0, numState0);
  delay(2000);
}



void loop() { 
  delay(50);
  displayMessage(menuItems[value]);
//  delay(500/); //or do whatever you need to do...
  rotary_loop();
  delay(50);

}
void handleChange(){
  if(state==0){
    updateState(1);
    return;
  }
  if (state == 1){
    if (value == 38){//ok
      //Check Master Password
      if (checkMasterPassword(state1Input)){
        updateState(3);
        return;
      }
      
      else{
        updateState(0);
        return;
      }        
      
    }
    else{
      //append character to string
    }
  }

}

void updateState(int newState){
  switch(newState){
    case 0: state = 0;
            numItems=numState0;
            rotaryEncoder.setBoundaries(0, numItems - 1, true); 
            copyArray(menuItems,state0,numItems);
            break;
    case 1: state = 1;
            numItems=numState1;
            rotaryEncoder.setBoundaries(0, numItems - 1, true); 
            copyArray(menuItems,state1,numItems);
            break;
    case 2: state = 2;
            numItems=numState2;
            rotaryEncoder.setBoundaries(0, numItems - 1, true); 
            copyArray(menuItems,state2,numItems);
            break;
    case 3: state = 3;
            numItems=numState3;
            rotaryEncoder.setBoundaries(0, numItems - 1, true); 
            copyArray(menuItems,state3,numItems);
            break;
    case 4: state = 4;
            numItems=numState4;
            rotaryEncoder.setBoundaries(0, numItems - 1, true); 
            copyArray(menuItems,state4,numItems);
            break;
    case 5: state = 5;
            numItems=numState5;
            rotaryEncoder.setBoundaries(0, numItems - 1, true); 
            copyArray(menuItems,state5,numItems);
            break;
    case 6: state = 6;
            numItems=numState6;
            rotaryEncoder.setBoundaries(0, numItems - 1, true); 
            copyArray(menuItems,state6,numItems);
            break;
  }
}

void checkMasterPassword(char userInp[32]){
  return true;
}
void displayMessage(char *message){
  x = display.width();
  minX = -12*strlen(message);
  while (x > minX){
    if (interruptHappened){
      delay(10);
      interruptHappened=false;
      return;
      }
  
  display.setCursor(x,15);
  display.print(message);
  Serial.print(message);
  Serial.println(value);
  if (rotaryEncoder.isEncoderButtonClicked())
    rotary_onButtonClick();
  display.display();
  x=x-SCROLLSPEED;
  clearDisplay();
  }
  x= display.width();

}

void clearDisplay(){
  display.clearDisplay();
  topStuff();
}

void topStuff(){
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("Sat:13  03:56:32  67%");
  display.setTextSize(2);
}

void setupDisplay(){
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000); 
  clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  
}

void rotary_onButtonClick(){
  static unsigned long lastTimePressed = 0; // Soft debouncing
  if (millis() - lastTimePressed < 500)
    return;
  lastTimePressed = millis();
  Serial.print("button pressed ");
  Serial.print(millis());
  Serial.println(" milliseconds after restart");
  handleChange();
}

void rotary_loop(){
  if (rotaryEncoder.encoderChanged()){
    Serial.print("Value: ");
    Serial.println(rotaryEncoder.readEncoder());
    value = rotaryEncoder.readEncoder();
  }
  if (rotaryEncoder.isEncoderButtonClicked())
    rotary_onButtonClick();
  
}
void IRAM_ATTR readEncoderISR(){
    rotaryEncoder.readEncoder_ISR();
    interruptHappened = true;
    
}

void setupRotaryEncoder(){
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  bool circleValues = true;
  rotaryEncoder.setBoundaries(0, numItems - 1, circleValues); 
  rotaryEncoder.disableAcceleration(); 
}

void copyArray(char array1[100][32], char array2[100][32], int arraySize){
  for (int j = 0; j<arraySize; j++){
    strcpy(array1[j], array2[j]);
    }  
}
