# Mr-Touch-Key
A laser cut touch MIDI keyboard project.

It uses:
- Arduino Pro Micro
- MPR-121 Touch Sensor (3.3v logic ONLY)
- Bi-directional logic level converter (This LLC only needs 5V supply and it regulates down to the 3.3V needed for MPR-121)
- 3.5mm stereo jack (has 3 leads used for TX, RX & GND using serial communication)

This project is designed to interact with another project, Mr. Roboto found here:
https://github.com/davidcool/Mr-Roboto

In mode 3 on Mr. Roboto as the robot moves it sends random MIDI notes to the keyboard to be played.
In mode 4 on Mr. Roboto as the keyboard is played different parts of the robot will move.

![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/mr_touch_key_schematic_v1.png)
![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/IMG_7016.jpg)
![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/IMG_7017.jpg)
![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/IMG_5797.jpg)
![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/IMG_5801.jpg)
