#include <stdio.h>
#include <windows.h>
#include <string.h>

void readFromUsbPort(const char *comPortName, const char *filename) {
    // Åbn COM-port
    HANDLE hSerial = CreateFile(comPortName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Kunne ikke aabne COM-porten.\n");
        return;
    }

    // Buffer til at modtage data
    char buffer[128];
    DWORD bytesRead;

    // Midlertidig buffer til at samle komplette linjer
    char lineBuffer[256] = {0};
    int lineBufferPos = 0;

    // Tæller til målinger
    int measurementCount = 1;

    // Åbn en CSV-fil
    FILE *csvFile = fopen(filename, "w");
    if (csvFile == NULL) {
        printf("Filen kunne ikke aabnes.\n");
        CloseHandle(hSerial);
        return;
    }

    // Skriv header til CSV
    fprintf(csvFile, "Measurement,Temperature,Humidity,SoilMoisture\n");

    // Læs data fra COM-port og skriv til CSV
    printf("Laeser data fra %s...\n", comPortName);
    while (1) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                // Behandling af indkommende data
                for (DWORD i = 0; i < bytesRead; i++) {
                    char c = buffer[i];

                    // Tjek for slutning af linje (f.eks. '\n' eller '\r')
                    if (c == '\n' || c == '\r') {
                        if (lineBufferPos > 0) {
                            lineBuffer[lineBufferPos] = '\0'; // Afslut linjen
                            printf("Data: %s\n", lineBuffer); // Udskriv data til konsollen
                            fprintf(csvFile, "%d,%s\n", measurementCount++, lineBuffer); // Skriv data til CSV med tæller
                            fflush(csvFile); // Sørg for, at data bliver skrevet til filen
                            lineBufferPos = 0; // Nulstil lineBuffer
                        }
                    } else if (lineBufferPos < sizeof(lineBuffer) - 1) {
                        // Tilføj karakteren til lineBuffer
                        lineBuffer[lineBufferPos++] = c;
                    }
                }
            }
        } else {
            printf("Fejl ved laesning fra COM-port\n");
            break;
        }
    }

    // Luk filer og COM-port
    fclose(csvFile);
    CloseHandle(hSerial);
    printf("Dataindsamling afsluttet.\n");
}

int main(void) {
    const char *comPortName = "\\\\.\\COM3"; // COM-portnavn
    const char *filename = "plant_data.csv"; // CSV-filnavn

    readFromUsbPort(comPortName, filename);
    return 0;
}
