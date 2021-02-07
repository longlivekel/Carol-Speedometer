// Power-Down Add-On Hardware Test Code
// const byte ledPin = 13;      // Onboard LED
// const byte interruptPin = 2; //D2
// volatile byte state = LOW;
// void setup()
// {
//   pinMode(ledPin, OUTPUT);                                              //D13 as Output
//   attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING); // ISR
// }
// void loop()
// {
//   digitalWrite(ledPin, state); // Enable LED
// }
// void blink()
// {
//   state = !state;
// }

// Save variables automatically to Arduino EEPROM on power-down
// Ian - This seems like the start of a second file. I don't think we need the first one at all.

#include <EEPROM.h> // Library comes with Arduino IDE
#include <SD.h>

int SomeVariable; // Some Variable
int PD_PIN = 2;   // D2 used for  power-down detection (INT.0)

int EE_ADDR = 0; // The address of each Data EEPROM command is selected by writing to the EEADDR register. Wont need it

void setup()
{

  EEPROM.get(EE_ADDR, SomeVariable); // Retrieve last stored value of SomeVariable from EEPROM

  attachInterrupt(digitalPinToInterrupt(PD_PIN), writeToSd, FALLING); // Set-up Interrupt Service Routine (ISR)

  Serial.print("Initializing SD card...");

  if (!SD.begin(53))
  {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  Serial.println("initialization done.");

  myFile = SD.open("odometer.txt");

  if (myFile)
  {
    Serial.println("odometer.txt:");

    // read from the file until there's nothing else in it:
    if (myFile.available())
    {
      odo = myFile.parseFloat();
      distance = odo;
      Serial.println("whats in the file?");
      Serial.println(myFile + "<-- newest line in the file");
      Serial.println("odometer value obtained from SD");
    }
    // close the file:
    myFile.close();
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening odometer.txt");
  }
}

void loop()
{
  // Do something great
}

void writeToSd()
{
  myFile = SD.open("odometer.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile)
  {
    Serial.print("Writing to odometer.txt...");
    myFile.println("farts " + millis());
    // close the file:
    myFile.close();
    Serial.println("File written and closed.");
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening odometer.txt");
  }
}
