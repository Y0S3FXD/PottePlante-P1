#include <DHT11.h>

// Globale variabler
int moisturePin = A0;
int maxValue = 1023;
int minValue = 300;
int pMoistureOld = 0;

DHT11 dht11(7); //Digital I/O Pin 7

void setup() {
  pinMode(moisturePin, INPUT);

  Serial.begin(9600);
}

void loop() {
    int temperature = 0;
    int humidity = 0;

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
    delay(120000);
}