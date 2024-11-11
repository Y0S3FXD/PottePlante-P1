#include <stdio.h>
#include <stdlib.h>

#define Locate_File = ""
int main(void)
{

    Read_Data(Locate_File);
    Convert_Data(double Temp[], double Moisture[]);
}
void Read_Data(Locate_File)
{
    FILE *input_file = fopen(Locate_File, "r");
    while (!ferror(input_file) && !feof(input_file))
    {
        size_t buffer_size = 0;
        char *buffer = NULL;

        getline(&buffer, &buffer_size, input_file);

        char *token = strtok(buffer, delimiter);

        while (token != NULL)
        {
            double value;
            sscanf(token, "%lf", &value);

            if (column == column_x)
            {
                x = value;
            }
            else if (column == column_y)
            {
                y = value;
            }

            column += 1;
            token = strtok(NULL, delimiter);
        }
    }
}
void Convert_Data(double Temp[], double Moisture[])
{

    Plot_Data(Temp[], Moisture[]);
}
void Plot_Data()
{
}
