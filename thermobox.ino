
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int maxCelsius = 80;  // Maximum Temparature
const int minCelsius = -1; // Minimum Temparature
int setCelsius = 0;       // Celsius set by user [will bechanged by potentiometer]
int potValue = 0;        // Potentiometer value  (Read from potentiometer)
int roomCelsius = 0;    // Room temprature from (Read from sensore)
int boxCelsius = 0;    // Box inside temrature  (Read from sensore)

void setup() {
   pinMode(13, OUTPUT); //LED Indicator
   pinMode(12, OUTPUT); //TEC on/off switch/status
   pinMode(11, OUTPUT); //TEC cold/heat switch
   digitalWrite(13, HIGH);   // turn the LED on 
    // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  digitalWrite(13, HIGH);   // turn the LED on
  // read the analog in value:
  potValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  setCelsius = map(potValue, 0, 1023, minCelsius, maxCelsius);
  //check is set Temprature 
  
  delay(500);              // wait 500ms
  digitalWrite(13, LOW);    // turn the LED off 
  delay(500);              // wait 500ms
}

