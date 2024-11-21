#include <DHT11.h>

void loop() {
    int temperature = 0;
    int humidity = 0;

    // Attempt to read the temperature and humidity values from the DHT11 sensor.
    int result = dht11.readTemperatureHumidity(temperature, humidity);

    // Fetch current time in milliseconds since the program started.
    unsigned long currentTime = millis();

    // Read soil moisture sensor value.
    int sensorValue = analogRead(rainPin);
    int soilMoisture = map(sensorValue, 1023, 300, 0, 100);

    // Check the result of the temperature and humidity readings.
    if (result == 0) {
        // Format output: time, temperature, humidity, soil moisture
        Serial.print(currentTime);
        Serial.print(", ");
        Serial.print(temperature);
        Serial.print(", ");
        Serial.print(humidity);
        Serial.print(", ");
        Serial.println(soilMoisture);

        // Indicate soil moisture state using LEDs.
        if (sensorValue < thresholdValue) {
            digitalWrite(redLED, LOW);
            digitalWrite(greenLED, HIGH);
        } else {
            digitalWrite(redLED, HIGH);
            digitalWrite(greenLED, LOW);
        }
    } else {
        // Print error message if reading fails.
        Serial.println(DHT11::getErrorString(result));
    }

    // Delay to avoid flooding the serial output.
    delay(1000);
}
