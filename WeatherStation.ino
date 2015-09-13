#include <Wire.h> //Library for I2C devices (used in Sparkfun library)
#include "SparkFunHTU21D.h" //Sparkfun library for the HTU21D sensor DOWNLOAD: https://github.com/sparkfun/HTU21D_Breakout/tree/V_H1.2_L1.1.0
#include <LiquidCrystal.h> //Library for controlling LCD's
HTU21D myHumidity; //Create an instance of the object
LiquidCrystal lcd(3, 2, 4, 5, 6, 7); //Define the pins for the LCD

int updateSpeed = 1000;     //Controls the update speed of the sensor
const int redLed = 9;       //Defines the pin for the red LED in the RGB backlight and LED
const int greenLed = 10;    //Defines the pin for the green LED in the RGB backlight and LED
const int blueLed = 11;     //Defines the pin for the blue LED in the RGB backlight and LED
const int comfortMax = 80;  //Defines the max tempurature inside the comfort zone
const int comfortMin = 65;  //Defines the min temperature inside the comfort zone
const int coldMin = 15;     //Defines the min temperature for the cold range (change this to the min temperature in your region)
const int heatMax = 100;    //Defines the max temperature for the hot range (change this to the max temperature in your region)
const int hotRedMin = 180;  //Defines the min light for the red LED when its hot
const int hotRedMax = 255;  //Defines the max light for the red LED when its hot
const int hotBlueMin = 55;  //Defines the min light for the blue LED when its hot
const int hotBlueMax = 0;   //Defines the max light for the red LED when its hot
const int comfortGreen = 255;  //Defines the amount of light for the green LED when it is comfortable
int redLight;    //Variable used to store the amount of red light
int blueLight;   //Variable used to store the amount of blue light
int greenLight;  //Variable used to store the amount of green light
int i;           //Variable used to store the data from the map function

void setup()
{
  lcd.init(1,3,255,2,4,5,6,7,0,0,0,0); //Initiate the LCD with the pins (this is only needed if you are using a Galileo)
  lcd.begin(16, 2);      // Define the number of rows and columns on the LCD
  myHumidity.begin();    // Initialize the temperature and humidity sensor
  lcd.print("Temp: ");   // Print "Temp: " on the LCD
  lcd.setCursor(0,1);    // Move to the second row of the LCD
  lcd.print("Humd: ");   // Print "Humd: " on the LCD
}

void loop()
{
  float humd = myHumidity.readHumidity();     //Define humd as the humidity readings
  float tempC = myHumidity.readTemperature(); //Define tempC as the temperature readings (in celcius)
  float tempF = convertCtoF(tempC);           //Define tempF as converting tempC to faherenheit
  lcd.setCursor(6,0);    //Set the cursor to the 7th space on the first row of the LCD
  lcd.print(tempF);      //Print the temperature in fahrenheit
  lcd.print(" F ");      //Print the " F " after the temperature reading
  lcd.setCursor(6,1);    // Set the cursor to the 7th space on the second row of the LCD
  lcd.print(humd);       // Print the humidity
  lcd.print(" % ");      // Print " % " after the humidity reading
  
  if(tempF<comfortMax && tempF>comfortMin){ //If the temperature is inside the comfort zone
    analogWrite(greenLed,comfortGreen);  //Set the screen and LED to green
    analogWrite(blueLed,0);              //Turn off the blue light
    analogWrite(redLed,0);               //Turn off the red light
  }
  
  if(tempF<comfortMin){ //If the temperature is below the comfort zone (in the cold zone)
    i = map(tempF, coldMin, comfortMin, 125, 255); //Map the current temperature to a number between 125 and 255
    constrain(i,0,255);  //Constrain the value between 0 and 255 
    blueLight = i;       // Set blue light = i; 
    greenLight = 255-i;  //Set green light = 255-i;
    // This way as the temperature gets colder the LED becomes a lighter blue
    analogWrite(redLed, 0);  //Turn the red LED off
    analogWrite(greenLed, greenLight);   //Set the level for the green LED
    analogWrite(blueLed, blueLight);     // Set the level for the blue LED
  }
  
  if(tempF>comfortMax){ //If the temperature is above the comfort zone (hot zone)
    redLight = map(tempF, comfortMax, heatMax, hotRedMin , hotRedMax);  //Map the redLight in between the redMin and redMax
    blueLight = map(tempF, comfortMax, heatMax, hotBlueMin, hotBlueMax); //Map the blue light in between blueMin and blueMax
    constrain(blueLight,0,255);   //Constrain the value between 0 and 255
    constrain(redLight,0,255);    //Constrain the value between 0 and 255
    analogWrite(greenLed, 0);     //Turn the green LED off
    analogWrite(redLed, redLight);  //Set the level for the red LED
    analogWrite(blueLed, blueLight); //Set the level for the blue LED
    //This way the color starts off as pinkish and gets a darker red as the temperature increases
  }
  delay(updateSpeed); //Delay the whole program by the update speed (To save energy and make the text readable)
  
}//loop

float convertCtoF(float tempC){ //Function for converting Celcius to Fahrenheit
  return (1.8*tempC)+32; //Return the value of the conversion
}
