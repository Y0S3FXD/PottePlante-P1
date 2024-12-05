#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

// Function to read data from COM port and save it to a CSV file
void readFromComPort(const char *comPortName, const char *filename) {
    // Open COM port
    HANDLE hSerial = CreateFile(comPortName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Could not open COM port.\n");
        return;
    }

    // Buffer for incoming data
    char buffer[128];
    DWORD bytesRead;

    // Temporary buffer to assemble complete lines
    char lineBuffer[256] = {0};
    int lineBufferPos = 0;

    // Measurement counter
    int measurementCount = 1;

    // Open the CSV file
    FILE *csvFile = fopen(filename, "w");
    if (csvFile == NULL) {
        printf("Could not open file.\n");
        CloseHandle(hSerial);
        return;
    }

    // Write header to CSV
    fprintf(csvFile, "Measurement,Temperature,Humidity,SoilMoisture\n");

    // Read data from COM port and write to CSV
    printf("Reading data from %s...\n", comPortName);
    while (1) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                // Process incoming data
                for (DWORD i = 0; i < bytesRead; i++) {
                    char c = buffer[i];

                    // Check for end of line (e.g., '\n' or '\r')
                    if (c == '\n' || c == '\r') {
                        if (lineBufferPos > 0) {
                            lineBuffer[lineBufferPos] = '\0'; // Terminate the line
                            printf("Data: %s\n", lineBuffer); // Print data to console
                            fprintf(csvFile, "%d,%s\n", measurementCount++, lineBuffer); // Write data to CSV with counter
                            fflush(csvFile); // Ensure data is written to file
                            lineBufferPos = 0; // Reset lineBuffer
                        }
                    } else if (lineBufferPos < sizeof(lineBuffer) - 1) {
                        // Append character to lineBuffer
                        lineBuffer[lineBufferPos++] = c;
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
void createGnuplotScript(const char *filename, const char *datafile, int column, const char *title) {
    FILE *script = fopen(filename, "w");
    if (!script) {
        perror("Error creating gnuplot script");
        exit(1);
    }
    fprintf(script, "set title \"%s\"\n", title);
    fprintf(script, "set xlabel \"Measurement Number\"\n");
    fprintf(script, "set ylabel \"Value (%%)\"\n");
    fprintf(script, "set yrange [0:100]\n");
    fprintf(script, "plot '%s' using 1:%d with lines title \"%s\"\n", datafile, column, title);
    fclose(script);
}

// Main function
int main(void) {
    const char *comPortName = "\\\\.\\COM3"; // COM port name
    const char *filename = "plant_data.csv"; // CSV file name
    const char *dataFile = "data.tmp"; // Temporary data file for gnuplot

    // Read data from COM port
    readFromComPort(comPortName, filename);

    // Open the CSV file
    FILE *csvFile = fopen(filename, "r");
    if (csvFile == NULL) {
        perror("Error opening CSV file");
        return 1;
    }

    // Open a temporary file to store parsed data for gnuplot
    FILE *fp = fopen(dataFile, "w");
    if (fp == NULL) {
        perror("Error creating temporary data file");
        fclose(csvFile);
        return 1;
    }

    // Skip the header line in the CSV file
    char line[256];
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
    createGnuplotScript("temperature.gnu", dataFile, 2, "Temperature (°C)");
    createGnuplotScript("humidity.gnu", dataFile, 3, "Humidity (%)");
    createGnuplotScript("soil_moisture.gnu", dataFile, 4, "Soil Moisture (%)");

    // Execute gnuplot for each script
    system("gnuplot -persistent temperature.gnu");
    system("gnuplot -persistent humidity.gnu");
    system("gnuplot -persistent soil_moisture.gnu");

    return 0;
}
