const int analogInPin = A0;     // Analog input pin that the potentiometer is attached to
const int maxCelsius = 80;     // Maximum Temparature
const int minCelsius = 0;     // Minimum Temparature
const int maxFanruntime = 60;// Max fan running time
int setCelsius = 0;         // Celsius set by user [will bechanged by potentiometer]
int potValue = 0;          // Potentiometer value  (Read from potentiometer)
int roomCelsius = 0;      // Room temprature from (Read from sensore)
int boxCelsius = 0;      // Box inside temrature  (Read from sensore)
int fanRuntime = 60;    // Main fan runtime after peltier turn off (in sec);

void setup() {
   digitalWrite(13, HIGH);   // turn the LED on 
   pinMode(13, OUTPUT); //LED Indicator
   pinMode(12, OUTPUT); //TEC on/off switch/status
   pinMode(11, OUTPUT); //TEC cold/heat switch
   pinMode(10, OUTPUT); //Main Fan switch
   //set all pin low
   digitalWrite(12, LOW);
   digitalWrite(11, LOW);
   digitalWrite(10, LOW);
   // initialize serial communications at 9600 bps:
   Serial.begin(9600);
   //calibration
   
}

void loop() {
   digitalWrite(13, HIGH);   // turn the LED on
   
  //Fan Run time logic
  if(fanRuntime == 0){
     digitalWrite(10, LOW); //turn off fan
    }else{
       digitalWrite(10, HIGH); //turn on fan
      }

      
  // read the analog in value:
  potValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  setCelsius = map(potValue, 0, 1023, minCelsius, maxCelsius);
  //Temprature logic
  if(setCelsius == boxCelsius){
    //Set temprature reached turn off peltier
    digitalWrite(12, LOW);   //Turn off main realy
    digitalWrite(11, LOW); //Turn off polarity relays
    delay(300);          //3 Second dealy
    Serial.println("Done 3s delay");
    
  }else{
    
    //polarity logic
    if(setCelsius > roomCelsius){
      digitalWrite(11, HIGH); //Turn on polarity relays, switch to Heating mode;
      Serial.println("Heating..");
    }else{
      digitalWrite(11, LOW); //Turn off polarity relays, switch to cooling mode;
      Serial.println("Cooling");
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
   Serial.println(fanRuntime);
}

