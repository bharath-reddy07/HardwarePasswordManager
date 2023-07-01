#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"

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

int x, minX;

int numItems = 5;
char menuItems[100][32] = {"Passwords", "Long message", "Very long message", "Very very long message", "short"};
int i=0;

void setup() {
  Serial.begin(9600);
  setupDisplay();
  setupRotaryEncoder();
}



void loop() {
  rotary_loop();
  delay(50);
  displayMessage(menuItems[i]);
  delay(500); //or do whatever you need to do...


}



void displayMessage(char *message){
  
  x = display.width();

  minX = -12*strlen(message);
  while (x > minX){
  clearDisplay();
  display.setCursor(x,15);
  display.print(message);

  display.display();
  x=x-SCROLLSPEED;
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
}

void rotary_loop(){
  if (rotaryEncoder.encoderChanged()){
    Serial.print("Value: ");
    Serial.println(rotaryEncoder.readEncoder());
    i = rotaryEncoder.readEncoder();
  }
  if (rotaryEncoder.isEncoderButtonClicked())
    rotary_onButtonClick();
  
}
void IRAM_ATTR readEncoderISR(){
    rotaryEncoder.readEncoder_ISR();
}

void setupRotaryEncoder(){
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  bool circleValues = true;
  rotaryEncoder.setBoundaries(0, numItems - 1, circleValues); 
  rotaryEncoder.disableAcceleration(); 
}