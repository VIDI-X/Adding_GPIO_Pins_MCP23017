//Code versions//
//esp32 by espressif v3.3.7
//MCP23017_WE by Wolfgang Ewald v1.6.12
//Adafruit ILI9341 by Adafruit v1.6.3
//Adafruit GFX Library by Adafruit v1.12.4

#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <SPI.h>

#include <Wire.h>
#include <MCP23017.h>

#define MCP_ADDRESS 0x27 // (A2/A1/A0 = LOW)
//#define RESET_PIN 5
#define I2C_SDA 33
#define I2C_SCL 32

// ILI9341 TFT LCD pins
#define TFT_CS   5
#define TFT_DC  21

// Create TFT object
Adafruit_ILI9341 TFT = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Create myMCP object
MCP23017 myMCP = MCP23017(MCP_ADDRESS);
int waitTime = 1000; // waitTime = waiting time (milliseconds)
byte portStatus;
bool pinStatus;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL, 100000);  // wake up I2C bus and tell it which pins to use

  if(!myMCP.Init()){                     // Try to connect to the MCP
    Serial.println("Not connected!");    // If connection fails, report it to serial
    while(1){}                           // and enter an infinite loop 
  }
  
  TFT.begin();
  TFT.setRotation(3);
  TFT.setTextSize(3);
  TFT.fillScreen(ILI9341_BLACK);
  TFT.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  TFT.println("    Adding");
  TFT.println(" External GPIO");
  TFT.println(" pins using the");
  TFT.println(" MCP23017 module!");
  
  myMCP.setPortMode(0b11111111, A);  // Port A: all pins are OUTPUT
  myMCP.setPortMode(0b00000000, B);  // Port B: all pins are INPUT
  delay(waitTime);
  myMCP.setAllPins(A, ON); // all LEDs turned on
  delay(waitTime);
  myMCP.setPinX(1, A, OUTPUT, LOW); // A1 turned off
  delay(waitTime);
  myMCP.setPort(0b11110000, A); // A4 - A7 turned on
  delay(waitTime);
  myMCP.setPort(0b01011110, A); // A0, A5, A7 turned off
  delay(waitTime);
  myMCP.setPinX(0, A, OUTPUT, HIGH); // A0 turned on
  delay(waitTime);
  myMCP.setPinX(4, A, OUTPUT, LOW); // A4 turned off
  delay(waitTime);
  myMCP.setAllPins(A, HIGH); // A0 - A7 all on
  delay(waitTime);
  myMCP.setPin(3, A, LOW); // A3 turned off
  delay(waitTime);
  
  for (int i = 0; i < 5; i++) { // A0 and A1 blinking
    myMCP.togglePin(0, A);// A0 toggle on-off
    delay(100);
    myMCP.togglePin(1, A);// A1 toggle on-off
    delay(100);
  }
  for (int i = 0; i < 5; i++) { // A1 and A0 blinking
    myMCP.togglePin(1, A);
    delay(200);
    myMCP.togglePin(0, A);
    delay(200);
  }
}

void loop() {
  myMCP.togglePin(1, A); // continue blinking A1
  delay(500);
  myMCP.togglePin(0, A); // continue blinking A0
  delay(500);
 
  portStatus = myMCP.getPort(A); // get status at port A
  TFT.setTextColor(ILI9341_RED, ILI9341_BLACK);
  TFT.setCursor(0, 120);
  TFT.print("GPIO A Status: ");
  Serial.print("GPIO A Status: ");
  TFT.setCursor(10, 150);
  TFT.print(portStatus, BIN);
  TFT.print("        ");
  Serial.println(portStatus, BIN);
  
  portStatus = myMCP.getPort(B);// get status at port B
  TFT.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  TFT.setCursor(0, 180);
  TFT.print("GPIO B Status: ");
  Serial.print("GPIO B Status: ");
  TFT.setCursor(10, 210);
  TFT.print(portStatus, BIN);
  TFT.print("        ");
  Serial.println(portStatus, BIN);
  
  //TFT.println("-----------------------");
  Serial.println("-----------------------");
}