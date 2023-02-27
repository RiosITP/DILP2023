#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <frequencyToNote.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>
/*
  Sensor Threshold Crossing w/MIDI
  NYU / IMA - Designing Interfaces for Live Performances 2023
  by David Rios Feb 2023
*/

int light;
int plight = 0;
int threshold = 350; // adjust to your need

void setup() {
  Serial.begin(9600);
  // wait for a serial connection.  If theres no connection the sketch freezes
  while (!Serial)
    ;
  light = analogRead(A1);
}

void loop() {
  light = analogRead(A1);
  // uncomment below to get readings and adjust threshold accordingly
  Serial.println(light);
  if (light >= threshold) {
    if (plight <= threshold) {
      plight = light;
      controlChange(2,16,127);
      Serial.println("Over Threshold");
      delay(10);
    }
  } else {
    if (plight > threshold) {
      plight = light;
      controlChange(2,17,127);
      Serial.println("Under Threshold");
      delay(10);
    }
  }
  delay(5);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}