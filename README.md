# Mr-Touch-Key
A laser cut touch MIDI keyboard project.

This project is available as kits at:
http://shop.davidcool.com

It uses:
- 1x Arduino Pro Micro
- 1x MPR-121 Touch Sensor (3.3v logic ONLY)
- 1x Bi-directional logic level converter (This LLC only needs 5V supply and it regulates down to the 3.3V needed for MPR-121) I used this one: https://www.amazon.com/gp/product/B0148BLZGE/ref=oh_aui_detailpage_o01_s00?ie=UTF8&psc=1
- 1x 3.5mm stereo female jack (has 3 leads used for TX, RX & GND using serial communication)
- 22 AWG hookup wire
- 1/2" copper tape
- 1/8" birch plywood
- 10mm M3 hex flathead bolts & M3 nuts

Tools:
- Soldering iron
- Solder
- Wire strippers
- helping hands
- 2mm hex wrench/driver
- 3D printer
- Laser cutter

If you don't have access to a laser cutter or 3D printer kits are available at:
http://shop.davidcool.com

This project is designed to interact with another project, Mr. Roboto found here:
https://github.com/davidcool/Mr-Roboto

- In mode 3 on Mr. Roboto as the robot moves it sends random MIDI notes to the keyboard to be played.
- In mode 4 on Mr. Roboto as the keyboard is played different parts of the robot will move.

![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/mr_touch_key_schematic_v1.png)
![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/mr_touch_key_schematic_v2.png)
![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/IMG_7016.jpg)
![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/IMG_7017.jpg)
![alt text](https://github.com/davidcool/Mr-Touch-Key/blob/master/_pictures/IMG_5801.jpg)
