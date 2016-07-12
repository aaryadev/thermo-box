#include <LiquidCrystal.h>
#include <EEPROM.h>
/*
 Thermo Box Project 
 -------------------
 Arduino pro mini for smart controller
 TEC1-12706 is heatexchange usig peltier
 16x2 display for status and temprature display
 LM35 for temrature sensore
 3 LED with 220 Oms for indication
 3 Relay with BC547 driver for peltier power switch with polarity change
 Heatsink with fan for peltier heat exhaust
 */
const int boxPin     =A5;        //thermo box sensor pin
const int maxCelsius = 80;     // Maximum Temparature
const int minCelsius = 0;     // Minimum Temparature
const int maxFanruntime = 60; // Max fan running time
float setCelsius = 0;         // Celsius set by user [will bechanged by potentiometer]
float boxCelsius = 0;      // Box inside temrature  (Read from sensore)
int fanRuntime = 60;    // Main fan runtime after peltier turn off (in sec);
float progress   = 0; //Thermo exchange progress
float initBoxTmp = 0; //Initial Box temprature 

LiquidCrystal lcd(7,6,5,4,3,2); //setup the pins for display
void setup() {
   digitalWrite(13, HIGH);   // turn the LED on 
   digitalWrite(8, HIGH);   // turn the screen backlight on
   pinMode(13, OUTPUT); //LED Indicator
   pinMode(12, OUTPUT); //TEC on/off switch/status
   pinMode(11, OUTPUT); //TEC cold/heat switch
   pinMode(10, OUTPUT); //Main Fan switch
   pinMode(A4, INPUT);
   pinMode(A5, INPUT);
   pinMode(A6, INPUT);
   //set all pin low
   digitalWrite(12, LOW);
   digitalWrite(11, LOW);
   digitalWrite(10, LOW);
   // initialize serial communications at 9600 bps:
   Serial.begin(9600);
   // set up the LCD's number of columns and rows:
   lcd.begin(16, 2);
   lcd.print("Thermo Box"); //Print data
   lcd.setCursor(0, 1); //2nd Line
   lcd.print("Booting....");
  if(EEPROM.read(0) > maxCelsius){ //Usually vallue will be 255 as default
    EEPROM.write(0,25); //write default value to room temprature
  }
   delay(3000); //let sensore calibration
   initBoxTmp=(analogRead(A5)*0.48828125);
}

void loop() {
  digitalWrite(13, HIGH);   // turn the LED on
  boxCelsius = (analogRead(A5)*0.48828125); //Read data from LM35 and convert to Celsius
  setCelsius = EEPROM.read(0);
  
  lcd.setCursor(0, 0); //1st Line 
  lcd.print("ST:");
  lcd.print((int)setCelsius);
  lcd.print("\337C");
  lcd.print(" ");
  lcd.print("BOX:");
  lcd.print((int)boxCelsius);
  lcd.print("\337C");
  lcd.setCursor(0, 1); //2nd Line  
  //Fan Run time logic
  if(fanRuntime == 0){
     digitalWrite(10, LOW); //turn off fan
    }else{
       digitalWrite(10, HIGH); //turn on fan
    }

      
  
  //Temprature logic
  if(setCelsius == (int)boxCelsius){
    //Set temprature reached turn off peltier
    digitalWrite(12, LOW);   //Turn off main realy
    digitalWrite(11, LOW); //Turn off polarity relays
    delay(300);          //3 Second dealy
    //Serial.println("Done 3s delay");
    lcd.print("Done :) 100 %");
  }else{
    
    //polarity logic
    if(setCelsius > boxCelsius){
      digitalWrite(11, HIGH); //Turn on polarity relays, switch to Heating mode;
      //calculate progress
      progress = (boxCelsius/setCelsius)*100;
      lcd.print("Heating...");
      lcd.print((int)progress);
      lcd.print("%");
    }else{
      digitalWrite(11, LOW); //Turn off polarity relays, switch to cooling mode;
      //Calculate progress in reverse pattern
      if(boxCelsius > initBoxTmp){
        initBoxTmp =boxCelsius+0.1; //If user places very hot object inside thermobox and it slowly start to heat up the box
      }
      progress=100-((100/initBoxTmp)*boxCelsius); //this calculation only works till min temp 0 Celsius

      lcd.print("Cooling...");
      lcd.print((int)progress);
      lcd.print("%");
      }
      
    //Turn on peltier to start heat exchange
    digitalWrite(12, HIGH);   //Turn on main Realy
    if(fanRuntime == 0){
      fanRuntime = maxFanruntime; //Reset Fan Run timer
      }
    if(fanRuntime <  maxFanruntime){
      fanRuntime ++;           //Increase fan run time
    }
    
  }
  
  delay(500);                // wait 500ms
  digitalWrite(13, LOW);    // turn the LED off 
  delay(500);              // wait 500ms

  if(fanRuntime > 0){
    fanRuntime --; //decrease 1 sec,till 0
   }
}

