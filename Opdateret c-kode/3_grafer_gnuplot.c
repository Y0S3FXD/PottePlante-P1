#include <stdio.h>
#include <stdlib.h>

void createGnuplotScript(const char *filename, const char *datafile, int column, const char *title, const char *output) {
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

int main(void) {
    FILE *fp = NULL;

    // Open the CSV file
    FILE *csvFile = fopen("plant_data.csv", "r");
    if (csvFile == NULL) {
        perror("Error opening CSV file");
        return 1;
    }

    // Open a temporary file to store parsed data for gnuplot
    fp = fopen("data.tmp", "w");
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
    float soilMoisture, humidity, temperature;
    while (fgets(line, sizeof(line), csvFile)) {
        if (sscanf(line, "%d,%f,%f,%f", &measurement, &temperature, &humidity, &soilMoisture) == 4) {
            fprintf(fp, "%d %f %f %f\n", measurement, temperature, humidity, soilMoisture);
        }
    }

    fclose(csvFile);
    fclose(fp);

    // Create gnuplot scripts for each plot
    createGnuplotScript("temperature.gnu", "data.tmp", 2, "Temperature (°C)", "Temperature");
    createGnuplotScript("humidity.gnu", "data.tmp", 3, "Humidity (%)", "Humidity");
    createGnuplotScript("soil_moisture.gnu", "data.tmp", 4, "Soil Moisture (%)", "Soil Moisture");
    // Execute gnuplot for each script
    system("gnuplot -persistent temperature.gnu");
    system("gnuplot -persistent humidity.gnu");
    system("gnuplot -persistent soil_moisture.gnu");
    return 0;
}
