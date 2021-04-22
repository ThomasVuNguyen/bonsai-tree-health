//photo
#define photo A1
int rawData;
int Lux;


// DHT temp and humidity
// temp C -0.44
// humid -1%
// Calibration check
// cali vs reading
// 22.54 vs 22.10
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 2     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
// cali vs reading
// 22.54 vs 22.10


//Capacitive Moisture and Temp

#include "Adafruit_seesaw.h"
// white wire to A4
// Green wire to A5
// Calibrate  vs Read
// 21.72    vs 23.53 
Adafruit_seesaw ss;


void setup() {
  //initiate photo read
  pinMode(photo, INPUT);
  Serial.begin(9600);

  //initiate DHT sensor
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  //SOil Sensor
  if (!ss.begin(0x36)) {
    //Serial.println("ERROR! seesaw not found");
    while(1);
  } else {
    //Serial.print("seesaw started! version: ");
    //Serial.println(ss.getVersion(), HEX);
  }
  delay(1500);

}

void loop() {
  //Calibrate and output photo
  rawData =  analogRead(photo);
  //Lux = rawData*1.51;
  Lux = rawData*-1.953 +1773+190+25;

  // Displaying
  Serial.print(millis());
  Serial.print(",");
  Serial.print("Brightness: ");
  Serial.print(",");
  Serial.print(Lux);
  Serial.print(",");
  Serial.print(" Lux ");
  Serial.print(",");


  //Calibrate DHT and display DHT

  //delay(delayMS);
  sensors_event_t event;
    dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    //Serial.println(F("Error reading temperature!"));

  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(",");
    Serial.print(event.temperature -0.44 );
    Serial.print(",");
    Serial.print(F("C"));
    Serial.print(",");

  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.print(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(",");
    Serial.print(event.relative_humidity -1 );
    Serial.print(",");
    Serial.print(F("%"));
    Serial.print(",");
  }


  //Soil Sensor
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);
  float mois = -286 +56*log(capread);
  

  Serial.print("Soil Temperature: "); 
  Serial.print(",");
  Serial.print(tempC -1.8); 
  Serial.print(",");
  Serial.print("*C");
  Serial.print(",");
  Serial.print("Soil Moisture: "); 
  Serial.print(",");
  Serial.print(mois); //  equation: moisture = -286+ 56 ln(capread).
  Serial.print(",");
  Serial.print(" %");
  


  //Make a new line
  Serial.println();

}
