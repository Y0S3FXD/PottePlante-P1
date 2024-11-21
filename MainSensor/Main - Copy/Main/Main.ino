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
int rainPin = A0;
int greenLED = 6;
int redLED = 7;
// you can adjust the threshold value
int thresholdValue = 800;
// Moisture end

// Create an instance of the DHT11 class.
// - For Arduino: Connect the sensor to Digital I/O Pin 2.
DHT11 dht11(7);
 int p_old = 0;

void setup() {
 
  // Humidity
  pinMode(rainPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  // Humidity end

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
    int sensorValue = analogRead(rainPin);
    int p = map(sensorValue, 1023, 300, 0, 100);
    
    if (abs(p_old - p) <= 10 && p <= 100 ) { // forskel på 10 eller mere i %
        if (result == 0) {
            Serial.print(temperature);
            Serial.print(",");
            Serial.print(humidity);
            Serial.print(",");
            Serial.print(sensorValue);
            Serial.print(",");
            Serial.println(p);
            p_old = p;
        } else {
            // Print error message based on the error code.
            Serial.println(DHT11::getErrorString(result));
        }
    } else {
      Serial.print(p);
      Serial.println("%, Stor forskel"); // skal fjernes før der skal laves csv fil 
      p_old = p;
    }

    

  
    delay(20);
}

