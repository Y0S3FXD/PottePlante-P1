#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;  // Pin for the SD card module

void setup() {
  Serial.begin(9600);
  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  
  Serial.println("SD card initialized.");
  
  // Create a new file
  File dataFile = SD.open("data.csv", FILE_WRITE);
  
  // Check if the file opened correctly
  if (dataFile) {
    // Write header
    dataFile.println("Time,Value");
    dataFile.close();  // Close the file
  } else {
    Serial.println("Error opening data.csv");
  }
}

void loop() {
  // Simulated sensor value
  int sensorValue = analogRead(A0);  // Read from analog pin A0
  unsigned long currentTime = millis();  // Get current time
  
  // Open the file again
  File dataFile = SD.open("data.csv", FILE_WRITE);
  
  // Check if the file opened correctly
  if (dataFile) {
    // Write data in CSV format
    dataFile.print(currentTime);
    dataFile.print(",");
    dataFile.println(sensorValue);
    dataFile.close();  // Close the file
  } else {
    Serial.println("Error opening data.csv");
  }
  
  delay(1000);  // Delay for 1 second before the next read
}
