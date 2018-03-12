//This code was constructed in Oulu Science Hack Day hackathon by the Hello Strawberry team
//The project kept going after the event.
//You can check the project status from https://www.facebook.com/StrawberryMoi/

//Sensors used in this project
//1.  KY-033 Tracking sensor for the water level indication (pole high/low)
//2.  KY-018 Photoresistor for water dirtyness measurement
//2.1 KY-011 Two color LED for water dirtyness measurement
//3.  KY-015 Air temperature and humidity measurement
//4.  KY-028 Digital measurement for water temperature (approximation for temp vs. voltage converion)

bool Alarm;
int WaterLevel = 10; // Declaration of the sensor input pin

//KY015 DHT11 Temperature and humidity sensor 
int DHpin = 8;
byte dat [5];
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}

// Declaration and initialization of the input pins
int Analog_Eingang = A0; // X-axis-signal
int Digital_Eingang = 3; // Button
  
//KY018 Photo resistor module
int sensorPin = A5; // select the input pin for the potentiometer
int ledPin = 13; // select the pin for the LED
int sensorValue = 0; // variable to store the value coming from the sensor

int greenled = 12;
int redled = 4;
   
void setup ()
{
  Serial.begin(9600); // Initialization serial output
  pinMode (WaterLevel, INPUT) ; // Initialization sensor pin
  pinMode (DHpin, OUTPUT);
  pinMode (Analog_Eingang, INPUT);   
  pinMode(ledPin, OUTPUT);   
  pinMode(greenled, OUTPUT);   
  pinMode(redled, OUTPUT);   
}
   
// The program reads the status of the sensor pins
// shows via serial terminal if the linetracker is on the line or not
void check_water_level ()
{
  bool val = digitalRead (WaterLevel) ; // The current signal of the sensor will be read
  if (val == HIGH) // If a signal is detected the LED will light up.
  {
    Serial.print("WATERLEVEL 1");
    Serial.print (' ');
    Alarm = 0;
  }
  else
  {
    Serial.print("WATERLEVEL 0");
    Serial.print (' ');
    Alarm = 1;
  }
}

void check_air_temperature_and_humidity()  {
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected
 
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response
 
  pinMode (DHpin, INPUT);
  while (digitalRead (DHpin) == HIGH);
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data
 
  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
 
  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
  
  Serial.print ("AIRHUMIDITY ");
  Serial.print (dat [0], DEC); // display the humidity-bit integer;
  Serial.print ('.');
  Serial.print (dat [1], DEC); // display the humidity decimal places;
  Serial.print (' ');
  //Serial.println ('%');
  Serial.print ("AIRTEMPERATURE ");
  Serial.print (dat [2], DEC); // display the temperature of integer bits;
  Serial.print ('.');
  Serial.print (dat [3], DEC); // display the temperature of decimal places;
  Serial.print (' ');
  //Serial.println ('C');
  
}

void check_water_temperature() {
  float Analog;
  int Digital;
  double Temp;
    
  // Current values will be read and converted to voltage
  //1023
  Analog = analogRead (Analog_Eingang); 
  Digital = digitalRead (Digital_Eingang);
  
  //... and outputted here
  Serial.print("WATERTEMPERATURE ");
  Serial.print(Analog);
  Serial.print (' ');

  
  Temp = log(10000.0*((1024.0/Analog-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  Serial.print("WATERTEMPERATURECONVERSION ");
  Serial.print(Temp);
  Serial.print (' ');
  //return Temp;
}

void check_water_dirtyness(){
  digitalWrite(ledPin, HIGH);
  delay(50);
  sensorValue = analogRead(sensorPin);
  digitalWrite(ledPin, LOW);
  Serial.print("LIGHTVALUE ");
  Serial.println(sensorValue, DEC);
}

void green_led_on(){
  digitalWrite(greenled, LOW);
 
}
void green_led_off(){
  digitalWrite(greenled, HIGH);
 
}

void red_led_on(){
  digitalWrite(redled, LOW);
 
}
void red_led_off(){
  digitalWrite(redled, HIGH);
 
}


void loop(){
  check_air_temperature_and_humidity();
  check_water_level();  
  check_water_temperature();
  check_water_dirtyness();
  
  if (Alarm == 0){
    red_led_off();
    green_led_on();
    }
  else {
    green_led_off();
    red_led_on();
  }
  //Serial.println(Alarm);
  //Serial.println("------------------------------------");
  delay(2000); // Break of 2000ms between the measurements 
  }
