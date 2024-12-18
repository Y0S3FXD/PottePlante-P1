#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *fp = NULL;
    FILE *gnupipe = NULL;
    char *GnuCommands[] = {
        "set title \"Plant Data Over Two Weeks\"",
        "set xlabel \"Measurement Number\"",
        "set ylabel \"Values\"",
        "plot 'data.tmp' using 1:2 with lines title 'Temperature (C)', "
        "'data.tmp' using 1:3 with lines title 'Humidity (%)', "
        "'data.tmp' using 1:4 with lines title 'Soil Moisture (%)'"
    };

    // Open the CSV file
    FILE *csvFile = fopen("plant_data.csv", "r");
    if (csvFile == NULL) {
        printf("Error opening CSV file");
        return 1;
    }

    // Open a temporary file to store parsed data for gnuplot
    fp = fopen("data.tmp", "w");
    if (fp == NULL) {
        printf("Error creating temporary data file");
        fclose(csvFile);
        return 1;
    }

    // Skip the header line in the CSV file
    char line[256];
    // fgets(line, sizeof(line), csvFile);

    // Read and write each line to the temporary file
    int measurement;
    float soilMoisture, humidity, temperature;
    while (fgets(line, sizeof(line), csvFile)) {
        if (sscanf(line, "%d,%f,%f,%f", &measurement, &soilMoisture, &humidity, &temperature) == 4) {
            fprintf(fp, "%d %f %f %f\n", measurement, soilMoisture, humidity, temperature);
        }
    }

    fclose(csvFile);
    fclose(fp);

    // Open a pipe to gnuplot
    gnupipe = _popen("gnuplot -persistent", "w");
    if (gnupipe == NULL) {
        printf("Error opening gnuplot");
        return 1;
    }

    // Send commands to gnuplot
    for (int i = 0; i < 4; i++) {
        fprintf(gnupipe, "%s\n", GnuCommands[i]);
    }

    _pclose(gnupipe);
    return 0;
}
