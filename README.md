# StrokeT

## sdStrokeT
sd module to save data on a formatted sd card. Save data as a .txt file <br>
saves data for every mode in a seperated file
* mode1 : count of every finger
* mode2 : overall time of the reaction game
* mode3 : max. strength of each finger

## servoPoti
module to control servo motors with a potentiometer <br>
analog read of poti and write it to servo motor

## Code Changes
* mode 2:
 * counting of wrong pushed buttons implemented
 * timer adapted to not count when pushing the right button
* mode 3:
 * added calculation of maximal force

# TO DOs
* timestamp - for each file of saved data
