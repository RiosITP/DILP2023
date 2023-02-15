/*
  by David Rios
  NYU IMA - Designing Interfaces for Live Performance 2023
  State Change MIDIUSB Template
  Code for DILP MIDI/ Arduino Class 2023
  Code based off Tom Igoe's Midi USB Examples
  https://tigoe.github.io/SoundExamples/midiusb.html
*/

// Include the Libraries
#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

// Button State Variables:
int b1 = 2;
int b2 = 3;
int b3 = 4;

// Button State change variables
int pb1 = 1;
int pb2 = 1;
int pb3 = 1;

// initial Note
int note = 60;


void setup() {
  Serial.begin(115200);
  // this example uses input pullup, this reverses the logic of buttons
  // 0 means pressed, 1 means not pressed when you read the buttons.
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
}

void loop() {
  /* 
  Detect a change.  digitalRead() returns the current value of the button
 compare digitalRead() to the previousButtonState: pb1 if the current state is different from the previous state. It means the button
 was pressd and you should either 
 1. Print some feedback 
 2. Play a Note
 3. Send a Control Change

 Then update the pb1 to reflect that the button was pressed
 */
  if (digitalRead(b1) == 0 && pb1 == 1) {
    noteOn(0, note, 80);
    controlChange(1, 16, 127);
    // Serial.print(note);
    // Serial.println(" on");
    pb1 = 0;
  }
  if (digitalRead(b1) == 1 && pb1 == 0) {
    noteOff(0, note, 127);
    Serial.print(note);
    Serial.println(" off");
    pb1 = 1;
  }

}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
}


// Control Change
// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}
