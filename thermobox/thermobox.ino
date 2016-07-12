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
const int analogInPin = A4;     // Analog input pin that the potentiometer is attached to
const int boxPin     =A5;        //thermo box sensor pin
const int roomPin    =A6;        //room sensor pin
const int maxCelsius = 80;     // Maximum Temparature
const int minCelsius = 0;     // Minimum Temparature
const int maxFanruntime = 60; // Max fan running time
float setCelsius = 0;         // Celsius set by user [will bechanged by potentiometer]
float potValue = 0;          // Potentiometer value  (Read from potentiometer)
float roomCelsius = 0;      // Room temprature from (Read from sensore)
float boxCelsius = 0;      // Box inside temrature  (Read from sensore)
int fanRuntime = 60;    // Main fan runtime after peltier turn off (in sec);

LiquidCrystal lcd(7,6,5,4,3,2);
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
   delay(3000); //let sensore calibrate 
  if(EEPROM.read(0) == 0 || EEPROM.read(0) == null){
    EEPROM.update(0,25); //set to default 25 C (room temprature)
  }
      
}

void loop() {
   digitalWrite(13, HIGH);   // turn the LED on
   Serial.println(EEPROM.read(0));
  //Get Room temprture 
  roomCelsius = (analogRead(A6)*0.48828125);
  boxCelsius = (analogRead(A5)*0.48828125);
  //Serial.println(analogRead(A5));
  //Serial.println(analogRead(A5)*0.48828125);

  // read the analog in value:
  potValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  setCelsius = map(potValue, 0, 1023, minCelsius, maxCelsius);

  
  lcd.setCursor(0, 0); //1st Line 
  lcd.print("B:");
  lcd.print(boxCelsius);
  lcd.print("c");
  lcd.print(" ");
  lcd.print("R:");
  lcd.print(roomCelsius);
  lcd.print("c");
  lcd.print("S:");
  lcd.print(setCelsius);
  lcd.print("c");
  lcd.print(" ");
  lcd.setCursor(0, 1); //2nd Line  
  //Fan Run time logic
  if(fanRuntime == 0){
     digitalWrite(10, LOW); //turn off fan
    }else{
       digitalWrite(10, HIGH); //turn on fan
      }

      
  
  //Temprature logic
  if(setCelsius == boxCelsius){
    //Set temprature reached turn off peltier
    digitalWrite(12, LOW);   //Turn off main realy
    digitalWrite(11, LOW); //Turn off polarity relays
    delay(300);          //3 Second dealy
    //Serial.println("Done 3s delay");
    lcd.print("Done :)");
  }else{
    
    //polarity logic
    if(setCelsius > roomCelsius){
      digitalWrite(11, HIGH); //Turn on polarity relays, switch to Heating mode;
     // Serial.println("Heating..");
      lcd.print("Heating...");
    }else{
      digitalWrite(11, LOW); //Turn off polarity relays, switch to cooling mode;
      //Serial.println("Cooling");
      lcd.print("Cooling...");
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
  // Serial.println(fanRuntime);
}

