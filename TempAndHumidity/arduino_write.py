import serial
import time
import csv

# Åben seriel porten (skift 'COM3' til den port, hvor din Arduino er tilsluttet)
ser = serial.Serial('COM3', 9600, timeout=1)

# Opret eller åben en CSV-fil til at gemme data
with open('sensor_data.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Timestamp', 'Sensor Value'])  # Skriv header i CSV

    # Læs data kontinuerligt
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()  # Læs data og fjern ekstra whitespaces
            if data:
                # Del dataen op i de enkelte værdier
                timestamp, sensor_value = data.split(',')
                
                # Skriv data til CSV-fil
                writer.writerow([timestamp, sensor_value])
                print(f"Data skrevet: {timestamp}, {sensor_value}")

                time.sleep(1)  # Vent et sekund (tilpasset til din Arduino loop)
