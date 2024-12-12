#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>


void readFromUsbPort(const char *usbPort3, const char *plantData);
void createGnuplotScript(const char *gnuplotScriptName, const char *datafile, int column, const char *title);

int main(void) {
    const char *usbPort3 = "\\\\.\\COM3";       // COM port name
    const char *plantData = "plant_data.csv";   // CSV file name
    const char *gnuplotData = "data.tmp";       // Temporary data file for gnuplot

    readFromUsbPort(usbPort3, plantData);

    // Open the CSV file
    FILE *csvFile = fopen(plantData, "r");
    if (csvFile == NULL) {
        printf("Error opening CSV file");
        return 1;
    }

    // Open a temporary file to store data for gnuplot
    FILE *fp = fopen(gnuplotData, "w");
    if (fp == NULL) {
        printf("Error creating temporary data file");
        fclose(csvFile);
        return 1;
    }

    // Skip the header line in the CSV file
    char line[64];                      // Previously 256, arbitrary number for the length of a line in CSV file
    fgets(line, sizeof(line), csvFile); // Read and ignore the header

    // Read and write each line to the temporary file
    int measurement;
    float temperature, humidity, soilMoisture;
    while (fgets(line, sizeof(line), csvFile)) {
        if (sscanf(line, "%d,%f,%f,%f", &measurement, &temperature, &humidity, &soilMoisture) == 4) {
            fprintf(fp, "%d %f %f %f\n", measurement, temperature, humidity, soilMoisture);
        }
    }

    fclose(csvFile);
    fclose(fp);

    // Create gnuplot scripts for each plot
    createGnuplotScript("temperature.gnu", gnuplotData, 2, "Temperature (°C)");
    createGnuplotScript("humidity.gnu", gnuplotData, 3, "Humidity (%)");
    createGnuplotScript("soil_moisture.gnu", gnuplotData, 4, "Soil Moisture (%)");

    // Execute gnuplot for each script
    system("gnuplot -persistent temperature.gnu");
    system("gnuplot -persistent humidity.gnu");
    system("gnuplot -persistent soil_moisture.gnu");

    return 0;
}

// Function to read data from COM port and save it to a CSV file
void readFromUsbPort(const char *usbPort3, const char *plantData) {
    HANDLE hSerial = CreateFile(usbPort3, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL); // Open COM port
    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Could not open COM port.\n");
        return;
    }

    char buffer[128];           // Buffer for incoming data        
    char lineBuffer[256] = {0}; // Temporary buffer to assemble complete lines
    DWORD bytesRead = 0;
    int lineBufferPos = 0;
    int measurementCount = 1;   // Measurement counter

    // Open the CSV file
    FILE *csvFile = fopen(plantData, "w");
    if (csvFile == NULL) {
        printf("Could not open file.\n");
        CloseHandle(hSerial);
        return;
    }

    // Write header to CSV
    fprintf(csvFile, "Measurement,Temperature,Humidity,SoilMoisture\n");

    // Read data from COM port and write to CSV
    printf("Reading data from %s...\n", usbPort3);
    while (1) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {                
                for (int i = 0; i < bytesRead; i++) {                                       // Process incoming data
                    char c = buffer[i];
                    if (c == '\n' || c == '\r') {                                           // Check for end of line (e.g., '\n' or '\r')
                        if (lineBufferPos > 0) {
                            lineBuffer[lineBufferPos] = '\0';                               // Terminate the line with '\0'
                            printf("Data: %s\n", lineBuffer);                               // Print data to terminal
                            fprintf(csvFile, "%d,%s\n", measurementCount++, lineBuffer);    // Write data to CSV with counter
                            fflush(csvFile);                                                // Ensure data is written to file
                            lineBufferPos = 0;                                              // Reset lineBuffer
                        }
                    } else if (lineBufferPos < sizeof(lineBuffer) - 1) {                        
                        lineBuffer[lineBufferPos++] = c;                                    // Append character to lineBuffer
                    }
                }
            }
        } else {
            printf("Error reading from COM port\n");
            break;
        }
    }

    // Close files and COM port
    fclose(csvFile);
    CloseHandle(hSerial);
    printf("Data collection completed.\n");
}

// Function to create a gnuplot script
void createGnuplotScript(const char *gnuplotScriptName, const char *datafile, int column, const char *title) {
    FILE *script = fopen(gnuplotScriptName, "w");
    if (!script) {
        printf("Error creating gnuplot script");
        exit(1);
    }
    fprintf(script, "set title \"%s\"\n", title);
    fprintf(script, "set xlabel \"Measurement Number\"\n");
    fprintf(script, "set ylabel \"Value (%%)\"\n");
    fprintf(script, "set yrange [0:100]\n");
    fprintf(script, "plot '%s' using 1:%d with lines title \"%s\"\n", datafile, column, title);
    fclose(script);
}


