/*
  SD card basic file example

  This example shows how to create and destroy an SD card file
  The circuit. Pin numbers reflect the default
  SPI pins for Uno and Nano models:
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - depends on your SD card shield or module.
     Pin 10 used here for consistency with other Arduino examples
    (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 24 July 2020
  by Tom Igoe

  This example code is in the public domain.
*/
#include <SD.h>
#include <SPI.h>
const int chipSelect = 10;  //?
File odoFile;



void setup() {
  //////////////////////////////////////////////
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
while (!Serial);

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
///////////////////////////////////////////////////
  if (SD.exists("odometer.txt")) {
    Serial.println("odometer.txt exists.");
  } else {
    Serial.println("odometer.txt doesn't exist.");
  }

  /*  // delete the file:
  Serial.println("Removing odometer.txt...");
  SD.remove("odometer.txt");

  if (SD.exists("odometer.txt")) {
    Serial.println("odometer.txt exists.");
  } else {
    Serial.println("odometer.txt doesn't exist.");
  }*/
  
////////////////////////////////////////////////
  /*
  // open a new file and immediately close it:
  Serial.println("Creating odometer.txt...");
  odoFile = SD.open("odometer.txt", FILE_WRITE);
  odoFile.close();
  */
//////////////////////////////////////////////////
  /*
  // Check to see if the file exists:
  if (SD.exists("odometer.txt")) {
    Serial.println("odometer.txt exists.");
  } else {
    Serial.println("odometer.txt doesn't exist.");
  }

  
////////////////////////////////////////////////////
  // delete the file:
  Serial.println("Removing odometer.txt...");
  SD.remove("odometer.txt");

  if (SD.exists("odometer.txt")) {
    Serial.println("odometer.txt exists.");
  } else {
    Serial.println("odometer.txt doesn't exist.");
  }
  */
  
}

void loop() {
  // nothing happens after setup finishes.
}
