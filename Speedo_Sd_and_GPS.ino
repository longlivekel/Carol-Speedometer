#include <Wire.h> //Needed for I2C to GPS

#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
SFE_UBLOX_GPS myGPS;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SwitecX25.h"
#include <SPI.h>
#include "SD.h"
const int chipSelect = 53;  //?
File odoFile;
//float odo = 123456.1234;

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to Ublox module.
float distance = 0;
float odo = 0;
const double StepsPerDegree = 1.9;         // Motor step is 1/3 of a degree of rotation  ... Maybe 2.9555
const unsigned int MaxMotorRotation = 315; // 315 max degrees of movement
const unsigned int MaxMotorSteps = MaxMotorRotation * StepsPerDegree;
unsigned int motorStep = 0;
double SpeedoDegreesPerMPH = (132 * StepsPerDegree)/ 120.0;

//----Define OLED Display Settings------  NOTE.. can do the same without this.. Relocated OLED_Reset
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
//-----End OLED Display Settings--------

//----Define Stepper motor library variables and pin outs-------------------------------------------
SwitecX25 Motor(MaxMotorSteps, 4, 5, 6, 7); // Create the motor object with the maximum steps allowed

void setup()
{
Serial.begin(9600);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
while (!Serial);

  Serial.print("Initializing SD card...");

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
///////////////////////////////////////////////////
  if (SD.exists("odometer.txt")) {
    Serial.println("odometer.txt exists.");
  } else {
    Serial.println("odometer.txt doesn't exist.");
    Serial.end();
  }
 
////////////////////////////
  Serial.begin(115200);

  while (!Serial)
    ; //Wait for user to open terminal
  Serial.println("Kel's Speedometer");
  Wire.begin();

  if (myGPS.begin(Wire, 0x3F) == false) //Connect to the Ublox module using Wire port
  {
    Serial.println(F("Ublox GPS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  // I2C OLED display
  display.begin(SSD1306_SWITCHCAPVCC); // initialize with the I2C addr 0x3C (128x32)
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(20, 10);
  display.println("CAROL");
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(70, 10);
  display.println(odo, 1);
  display.display();


Motor.zero(); //Initialize stepper at 0 location
  Motor.setPosition(460);  
  Motor.updateBlocking();
  delay(100);
  Motor.setPosition(0); //0MPH
  Motor.updateBlocking();
  delay(100);
  Motor.setPosition(0); //0MPH
  Motor.updateBlocking();
  delay(100);
//460 is 120
  //440 is 115    
  //421 is 110.   
  //405 is 105. 
  //382 is 100. 
  //364 is 95 
  //347 is 90 
  //331 is 85 
  //313 is 80. 
  //293 is 75  
  //273 is 70  
  //257 is 65
  //237is 60 
  //215 is 55 
  //192 is 50 
  //168 is 45 
  //147 is 40   
  //124 is 35 
  //101 is 30. 
  //78 is 25 
  //55 is 20.  
  //37 is 15 
  //25 is 10
  
}

void loop()
{

  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  if (millis() - lastTime > (1000))
  {
    lastTime = millis(); // Update the timer

    long speed = myGPS.getGroundSpeed();
    float speedMPH = (speed * 0.00223694);
    // distance is equal to the old distance plus the new speed / (polls per second * seconds in an hour)
    distance = speedMPH >= 0.5 ? distance + (speedMPH / (60 * 60)) : distance;

    //TEMP TODO: write speedMPH to the LCD

    //map the speedMPH to the stepper position
    
    //speedMPH = 50;
    
    double stepsPerMPH = 5.5;

    if (speedMPH < 20) {
      stepsPerMPH = 2;
    } else if (speedMPH >= 20 && speedMPH < 40) {
      stepsPerMPH = 3.2666; 
    } else if (speedMPH >=40 && speedMPH < 60) {
      stepsPerMPH = 4.5; 
    }

    motorStep = MphToStep(stepsPerMPH, speedMPH);
    Motor.setPosition(motorStep);



 Motor.updateBlocking();

    Serial.print(F(" Speed: "));
    Serial.print(speedMPH);
    Serial.print(F(" (mph)"));
    Serial.print(F(" Distance: "));
    Serial.print(distance, 6);
    Serial.print(F(" Miles"));
    // every 10th of a mile we change/write the odometer
    if (distance >= (odo + .1))
    {
      odo = distance;
      Serial.print(F(" | Odometer: "));
      Serial.print(odo, 1);

      // TODO: write odo to memory

      // TODO: write odo to LCD

      //display.setTextSize(3);
      //display.setTextColor(WHITE);
      display.clearDisplay();
      display.setCursor(0, 10); // WTF??
      display.println(odo, 1);
      display.display();

      // NOTE: This will have the disadvantage of losing up to .09 when you shut the car off
    }

    Serial.println();
  }
}

unsigned int MphToStep(double stepsPerMph, float mph)
{
  // convert mph to degrees on the speedo
  return (unsigned int)(stepsPerMph * mph);
}
