#include <DHT11.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

// Moisture setup
int moisturePin = A0;
int maxValue = 1023;
int minValue = 300;
int pMoisture_old = 0;

// Moisture end

int moistureThreshold = 40; // Threshold below which the plant needs water
int soilMax = 48; // soilmusture above 90 = stop watering

// - For Arduino: Connect the sensor to Digital I/O Pin 7.
DHT11 dht11(7);

void setup() {
  // Humidity
  pinMode(moisturePin, INPUT);

  // Initialize serial communication to allow debugging and data readout.
  // Using a baud rate of 9600 bps.
  Serial.begin(9600);
  
  // for LED RBG lys
  WiFiDrv::pinMode(25, OUTPUT); //define red pin
  WiFiDrv::pinMode(26, OUTPUT); //define green pin
  WiFiDrv::pinMode(27, OUTPUT); //define blue pin
}






void loop() {
    int temperature = 0;
    int humidity = 0;

    // Attempt to read the temperature and humidity values from the DHT11 sensor.

    int result = dht11.readTemperatureHumidity(temperature, humidity);
    int sensorValue = analogRead(moisturePin);
    int pMoisture = map(sensorValue, maxValue, minValue, 0, 100); // omregner til procent
    
    if (result == 0) { 

        if (abs(pMoisture_old - pMoisture) <= 10 && pMoisture <= 100) { // forskel på 10 eller mere i %
          Serial.print(temperature);
          Serial.print(",");
          Serial.print(humidity);
          Serial.print(",");
          // Serial.print(sensorValue);
          // Serial.print(",");
          Serial.println(pMoisture);
          pMoisture_old = pMoisture;
        } else {
          pMoisture_old = pMoisture;
        }
    } else {
      // Print error message based on the error code.
      Serial.println(DHT11::getErrorString(result));
      pMoisture_old = pMoisture;
    } 

    if (pMoisture < moistureThreshold || pMoisture > soilMax) {
      // LED Control: Watered too little - red
      if (pMoisture < moistureThreshold) {
        //  WiFiDrv::analogWrite(26, 0); // green off 
        WiFiDrv::analogWrite(25, 255); // trun on red = water your plant
      } else {
        WiFiDrv::analogWrite(25, 0); // turn off red
      }

      // LED Control: Watered too much - blue
      if (pMoisture > soilMax) {
        // WiFiDrv::analogWrite(26, 0); // green off
        WiFiDrv::analogWrite(27, 255); // trun on blue = STOP watering your plant
      } else {
        WiFiDrv::analogWrite(27, 0); // turn off blue
      }
    } else {
      WiFiDrv::analogWrite(26, 255); // green on
    }
    
    delay(600000);
}

