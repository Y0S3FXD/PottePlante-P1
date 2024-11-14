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

    // Check the results of the readings.
    // If the reading is successful, print the temperature and humidity values.
    // If there are errors, print the appropriate error messages.
    if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }
    int sensorValue = analogRead(rainPin);
  int p = map(sensorValue, 1023, 300, 0, 100);
  Serial.print("The soil moisture value is: ");
  Serial.println(p);
  if(sensorValue < thresholdValue){
    Serial.println(" - Doesn't need watering");
    Serial.println("");
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);

  }
  else {
    Serial.println(" - Time to water your plant");
    Serial.println("");
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);

  }
  delay(500);
}

