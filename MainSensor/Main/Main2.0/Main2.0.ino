/**
 * DHT11 Sensor Reader
 * This sketch reads temperature and humidity data from the DHT11 sensor and prints the values to the serial port.
 * It also handles potential error states that might occur during reading.
 *
 * Author: Dhruba Saha
 * Version: 2.1.0
  */

// Include the DHT11 library for interfacing with the sensor.
#include <DHT11.h>

// Moisture setup
int moisturePin = A0;
int maxValue = 1023;
int minValue = 300;

// Create an instance of the DHT11 class.
// - For Arduino: Connect the sensor to Digital I/O Pin 7.
DHT11 dht11(7);
 int pMoistureOld = 0;

void setup() {
 
  // Humidity
  pinMode(moisturePin, INPUT);

  Serial.begin(9600);
  
}

void loop() {
    int temperature = 0;
    int humidity = 0;

    // Attempt to read the temperature and humidity values from the DHT11 sensor.

    int result = dht11.readTemperatureHumidity(temperature, humidity);
    int sensorValue = analogRead(moisturePin);
    int pMoisture = map(sensorValue, maxValue, minValue, 0, 100); // omregner til procent
    
    if (abs(pMoistureOld - pMoisture) <= 10 && pMoisture <= 100 ) { // forskel på 10 eller mere i %

        if (result == 0) {
            Serial.print(temperature);
            Serial.print(",");
            Serial.print(humidity);
            Serial.print(",");
            // Serial.print(sensorValue);
            // Serial.print(",");
            Serial.println(pMoisture);
            pMoistureOld = pMoisture;
        } else {
            // Print error message based on the error code.
            Serial.println(DHT11::getErrorString(result));
        }
    } else {
      // Serial.print(pMoisture);
      // Serial.println("%, Stor forskel"); // skal fjernes før der skal laves csv fil 
      pMoistureOld = pMoisture;
    }

    

  
    delay(500);
}

