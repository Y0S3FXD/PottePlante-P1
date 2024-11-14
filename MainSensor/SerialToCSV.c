#include <stdio.h>
#include <windows.h>

int main() {
    // Specify the COM port (adjust as needed, e.g., "COM5")
    const char *comPortName = "\\\\.\\COM5";
    HANDLE hSerial;

    // Open the serial port
    hSerial = CreateFile(comPortName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening serial port\n");
        return 1;
    }

    // Set serial parameters (9600 baud, 8N1 format)
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error getting serial port state\n");
        return 1;
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error setting serial port state\n");
        return 1;
    }

    // Set timeouts for the serial port
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Error setting serial port timeouts\n");
        return 1;
    }

    // Open a CSV file to save data
    FILE *csvFile = fopen("plant_data.csv", "w");
    if (csvFile == NULL) {
        printf("Error opening CSV file\n");
        CloseHandle(hSerial);
        return 1;
    }
    fprintf(csvFile, "SoilMoisture,Humidity,Temperature\n");  // Write CSV header

    // Buffer to store serial data
    char buffer[128];
    DWORD bytesRead;
    
    // Read serial data and save it to the CSV file
    printf("Reading data from serial port...\n");
    while (1) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            buffer[bytesRead] = '\0';  // Null-terminate the string
            if (bytesRead > 0) {
                printf("Data: %s", buffer);  // Print data for verification

                // Write data to CSV file
                fprintf(csvFile, "%s", buffer);
                fflush(csvFile);  // Ensure data is written immediately
            }
        } else {
            printf("Error reading from serial port\n");
            break;
        }
    }

    // Close the CSV file and serial port
    fclose(csvFile);
    CloseHandle(hSerial);
    printf("Data collection stopped.\n");
    return 0;
}
