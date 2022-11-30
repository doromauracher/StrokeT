/*
  SD card read/write

  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 51
 ** MISO - pin 50
 ** SCK - pin 52
 ** CS - pin 53 

  created: 20.11.2022
  by Dorothea Mauracher


  only works with FAT16 or FAT32 formatted SD cards!
  
*/


#include <SPI.h>
#include <SD.h>

File myFile;

// CS
const int chipSelect = 53;

// filenames for different modes
const char mode1[] = "MODE1.txt";
const char mode2[] = "MODE2.txt";
const char mode3[] = "MODE3.txt";

int chooseMode = 1;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
  pinMode(chipSelect, OUTPUT);

  //-------------------------------------- TO DO -----------------------------------------
  // adapt to lcd display
  
  // SD card initialization
  Serial.print("Initializing SD card...");

  if (!SD.begin()) { 
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a sd card inserted?");
    Serial.println("* is the card formatted as FAT16 or FAT32");
    Serial.println("if so, please remove card and insert again!");
    while (1);
  }
}

void loop() {
  
  switch (chooseMode) {
    case 1:
      writeFile(mode1);
      break;
    case 2:
      writeFile(mode2);
      break;
    case 3:
      writeFile(mode3);
      break;
  }
}

void writeFile(char filename[]) {
  // open the file
  // if file does not exist - create one
  myFile = SD.open(filename, FILE_WRITE);
  if(myFile) {
    myFile.print("finger1: ");
    myFile.print(","); 
    myFile.print("finger2: ");
    myFile.print(",");
    myFile.print("finger3: ");
    myFile.print(",");
    myFile.println("finger4: ");
    myFile.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(filename);
  }
}
