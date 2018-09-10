// ---------------------------------------------------------------------------
// Mr. Touch Key
// MIDI Touch Keyboard Firmware
// v1.0
// David Cool
// http://davidcool.com
//
// Project archive: https://github.com/davidcool/Mr-Touch-Key
//
// Firmware for a MIDI keyboard using an MPR121 touch sensor breakout
// and Arduino Pro Micro 32u4 (Leonardo)
//
// The keyboard can interface with Mr. Roboto (https://github.com/davidcool/Mr-Roboto) over serial
// ---------------------------------------------------------------------------

#include "Wire.h"
#include "Adafruit_MPR121.h"

#define BUTTONS  12
#define IRQ_PIN  9

// mpr121 init
Adafruit_MPR121 cap = Adafruit_MPR121();

// keep track of touched keys
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

unsigned long currentMillis = 0; 
unsigned long previousMillis = 0;
unsigned long noteDuration = random(250,3000);

// MIDI note mapping
// Octave   Note Numbers
//          C   C#  D   D#  E   F   F#  G   G#  A   A#  B
//    0     0   1   2   3   4   5   6   7   8   9   10  11
//    1     12  13  14  15  16  17  18  19  20  21  22  23
//    2     24  25  26  27  28  29  30  31  32  33  34  35
//    3     36  37  38  39  40  41  42  43  44  45  46  47
//    4     48  49  50  51  52  53  54  55  56  57  58  59
//    5     60  61  62  63  64  65  66  67  68  69  70  71
//    6     72  73  74  75  76  77  78  79  80  81  82  83
//    7     84  85  86  87  88  89  90  91  92  93  94  95
//    8     96  97  98  99  100 101 102 103 104 105 106 107
//    9     108 109 110 111 112 113 114 115 116 117 118 119
//    10    120 121 122 123 124 125 126 127

// prime dynamic values
int channel = 0;
int pitch[] = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};
int vel[] = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
int serialRead[2];                                                  // serial data from keyboard containing current pitch and note on/off value
bool count = 0;

int noteIndex[256];
//int *p = noteIndex;
int noteIndexSize = 0;
int stopIndex = 0;

void setup() {

  //On the Leonardo use Serial1 for communication, pins 0 for RX, 1 for TX
  Serial1.begin(9600);
  delay(2000);

  // set mpr121 IRQ pin to input
  pinMode(IRQ_PIN, INPUT);

  // bail if the mpr121 init fails
  if (! cap.begin(0x5A))
    while (1);

  // set sensitivity of touch surface, lower numbers increase sensitivity
  cap.setThreshholds(12, 6);
}

void loop() {

  currentMillis = millis();
  
  if (Serial1) {
    count = 1;
  } else if ( (!Serial1) && (count == 1) ) {
    asm volatile ("  jmp 0");
  }
    
  if (Serial1.available() >= 2){
      byte b1 = Serial1.read();
      serialRead[0] = b1;
      
      byte b2 = Serial1.read();
      serialRead[1] = b2;   

      play_serial_note();
      
  }

  stop_serial_note();

  // if mpr121 irq goes low, there have been
  // changes to the button states, so read the values
  if(digitalRead(IRQ_PIN) == LOW)
    readButtons();

}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                       SERIAL NOTE HANDLERS                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// deal with notes transmitted over serial
void play_serial_note() {

    // note on check
    if (serialRead[0] == 1) {
      
      // play pressed note
      midi(channel, 0x9, serialRead[1], random(30,127));
      //delay(5);
      int indexSize = sizeof(noteIndex);
      if (indexSize >= 256) {
        noteIndexSize = 0;
      }
      noteIndex[noteIndexSize] = serialRead[1];
      noteIndexSize++;
      
    }
    
}

void stop_serial_note() {

    if (currentMillis - previousMillis >= noteDuration) {
      previousMillis = currentMillis;

      if (stopIndex >= 256) {
        stopIndex = 0;
      }

      // play note off
      midi(channel, 0x8, noteIndex[stopIndex], random(30,127));
      //delay(5);

      noteDuration = random(250,3000);
    }
  
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                       BUTTON PRESS HANDLERS                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// deal with note on and off presses
void handle_note() {

  for (uint8_t i=0; i < BUTTONS; i++) {

    // note on check
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {

      // send "on" signal as "1"  to the serial port
      Serial1.write(1 % 256);

      // send note value to the serial port
      Serial1.write(pitch[i] % 256); 
      delay(5); //allows all serial sent to be received together
      
      // play pressed note
      midi(channel, 0x9, pitch[i], vel[i]);

    }

    // note off check
    if ( !(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {

      // send "off" signal as "0"  to the serial port
      Serial1.write(0 % 256);

      // send note value to the serial port
      Serial1.write(pitch[i] % 256); 
      delay(5); //allows all serial sent to be received together
      
      // play note off
      midi(channel, 0x8, pitch[i], 0x0);

    }

  }

}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                         SEQUENCER CALLBACKS                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// the callback to send midi commands. this specific callback is designed to be
// used with an arduino leonardo or micro and the arcore midi
// usb modifications
//
// for more info on arcore:
// https://github.com/rkistner/arcore
void midi(byte channel, byte command, byte arg1, byte arg2) {

  // init combined byte
  byte combined = command;

  // shift if necessary and add MIDI channel
  if(combined < 128) {
    combined <<= 4;
    combined |= channel;
  }

  // send midi message
  MIDIEvent event = {command, combined, arg1, arg2};
  MIDIUSB.write(event);
  MIDIUSB.flush();

}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                      GENERAL UTILITY FUNCTIONS                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// read the currently touched buttons and detect any
// pressed patterns that match command states. this
// set of states is set up for 6 pads, but you could
// easily modify the hex values to match the ammount
// pads you are using
void readButtons() {

  // read current values
  currtouched = cap.touched();

  switch(currtouched) {

    default:
        handle_note();

  }

  // save current values to compare against in the next loop
  lasttouched = currtouched;

}

// get the index of the pressed button. this is used by
// the velocity and note modes to tell which button to change
void get_position() {

  for(int i=0; i < BUTTONS; i++) {
    if ( (currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
    }
  }

}

