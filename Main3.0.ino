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
// Moisture end

// LED setup
int ledPin = 13; // Use digital pin 13 for the LED
int moistureThreshold = 30; // Threshold below which the plant needs water
// LED end

// Create an instance of the DHT11 class.
// - For Arduino: Connect the sensor to Digital I/O Pin 2.
DHT11 dht11(7);
 int pMoisture_old = 0;

void setup() {
 
  // Humidity
  pinMode(moisturePin, INPUT);
  // Humidity end

    // LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Turn off LED initially
  // LED end

  // Initialize serial communication to allow debugging and data readout.
  // Using a baud rate of 9600 bps.
  Serial.begin(9600);
  
  // Uncomment the line below to set a custom delay between sensor readings (in milliseconds).
  // dht11.setDelay(500); // Set this to the desired delay. Default is 500ms.
}

void loop() {
    int temperature = 0;
    int humidity = 0;

    // Attempt to read the temperature and humidity values from the DHT11 sensor.

    int result = dht11.readTemperatureHumidity(temperature, humidity);
    int sensorValue = analogRead(moisturePin);
    int pMoisture = map(sensorValue, maxValue, minValue, 0, 100); // omregner til procent
    
    if (abs(pMoisture_old - pMoisture) <= 10 && pMoisture <= 100 ) { // forskel på 10 eller mere i %

        if (result == 0) {
            Serial.print(temperature);
            Serial.print(",");
            Serial.print(humidity);
            Serial.print(",");
            // Serial.print(sensorValue);
            // Serial.print(",");
            Serial.println(pMoisture);
            pMoisture_old = pMoisture;
        } else {
            // Print error message based on the error code.
            Serial.println(DHT11::getErrorString(result));
        }
    } else {
      // Serial.print(pMoisture);
      // Serial.println("%, Stor forskel"); // skal fjernes før der skal laves csv fil 
      pMoisture_old = pMoisture;
    } 
      // LED Control: Turn on LED if moisture is below the threshold
      if (pMoisture < moistureThreshold) {
          digitalWrite(ledPin, HIGH); // Turn on LED
      } else {
              digitalWrite(ledPin, LOW); // Turn off LED
      }

    

  
    delay(500);
}

