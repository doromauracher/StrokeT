# Mira-π
a therapy device used for mirror-therapy and rehabilitation of affected fingers

## Code Structure
**main-file**: global variables are defined, the basic code structure runs in a loop <br>
**descriptions.h**: header file for global variables and definitions <br>
**mode1-4.cpp**: implementations of the different training modes - functions used in main file <br>
**mode1-4.h**: header file for the different training modes - needs to be included in the main file (to define the functions in the right order)

## sdStrokeT
sd module to save data on a formatted sd card. Save data as a .txt file <br>
saves data for every mode in a seperated file
* mode1 : count of every finger
* mode2 : overall time of the reaction game
* mode3 : max. strength of each finger

## servoPoti
module to control servo motors with a potentiometer <br>
analog read of poti and write it to servo motor

## MQTT Connection
### ESPconnectionToMega
ensures the connection between esp32 and arduino mega <br>
connection via Serial and characterwise
runs on esp32

### megaApplicationForEsp
ensures the connection on the arduino mega to a esp 32
runs on arduino mega

### MQTT_Code_StrokeT
ensures a connection of the device to a MQTT broker

### StrokeT final 
final code for AAL submission: connection to esp32 and to MQTT broker


## Code Changes for Project 1
* refactoring of code
* more object oriented 
* extraction of different training modes
* new game - mode 5

## Code Changes for AAL project
* mode 2:
  * counting of wrong pushed buttons implemented
  * timer adapted to not count when pushing the right button
* mode 3:
  * added calculation of maximal force

# TO DOs
* timestamp - for each file of saved data
