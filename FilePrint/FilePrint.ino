#include <SPI.h> 
#include <SD.h> 
#include "DHT.h"
#define DHTPIN 2     Pin 2
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE); 

#include <Wire.h>  

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  

int lightSensor = 0; 
int lightValue = 0;  
int LEDS= 3;                   
const int chipSelect = 10; 

unsigned long second= 1000;
unsigned long minute= 60000;
unsigned long previousMillis=0; 
the loop


void setup()
     

{
  pinMode(10, OUTPUT);
  pinMode (LEDS, OUTPUT);      
  Serial.begin(9600);          
  lcd.begin(16,2);          
  lcd.setCursor(0,0);
  

  lcd.print("Hello, world!");   
  delay(1000);
  lcd.clear();

  lcd.setCursor(0,0); 
  lcd.print("Initializing SD card...");   
 
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;

  }
 delay (1000);
 lcd.clear(); 
 
lcd.setCursor(0,0); //Start at character 4 on line 0
  lcd.print("Initializing SD card..."); 
 SD.remove("datalog.txt");

 delay (1000);

lcd.clear(); 


}

void loop()

{             
   // Following code initialise sensor and checks it it working    
  float h = dht.readHumidity();  // Read temperature as Celsius
  float t = dht.readTemperature();  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);   
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) 
  
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
    lcd.setCursor(0,0);     
  lcd.print("Failed to read from DHT sensor!");
  }


//The print on LCD code
  lcd.setCursor(0,0); 
  lcd.print("H");
  lcd.setCursor(2,0);
  lcd.print(h);
  lcd.setCursor(7,0);
  lcd.print("%");
  
  
  lcd.setCursor(0,1); 
  lcd.print("T");
  lcd.setCursor(2,1);
  lcd.print(t); // prints temperature in Celcius
  lcd.setCursor(7,1); 
  lcd.print("c");
  lcd.setCursor(9,1);
  lcd.print(f);  // prints temperature in Faherenheit
  lcd.setCursor(14,1);
  lcd.print("f");    
  
  
   
  
  // Following code create minute delay between reading light sensor and recording data
  lightValue = analogRead(lightSensor);  
  unsigned long currentMillis = millis(); 
        if ((unsigned long)(currentMillis - previousMillis) >= 60000)  
      
            {    
                
                lcd.setCursor(9,0);
                lcd.print("L     "); 
                lcd.setCursor(11,0); 
                lcd.print(lightValue); 



                
                              File dataFile = SD.open("datalog.txt", FILE_WRITE); // HERE I NEED TO WRITE ALL THE DATA RECEIVED FROM THE SENSORS I CAN ONLY WRITE TEMP at the moment 't'
                               if (dataFile) {
                               dataFile.println(t);
                                dataFile.close();
            
                                              }
                              // if the file isn't open, pop up an error:
                              else       {
                               lcd.setCursor(11,0); 
                                lcd.print("FILE ERROR"); 
                                      }
                                           
                              
                  previousMillis = currentMillis;                             
                  if (lightValue >= 20)
                        {digitalWrite (LEDS,HIGH);}
                    else  
                        {digitalWrite (LEDS,LOW);}
            }
  


    
    

  
 
}