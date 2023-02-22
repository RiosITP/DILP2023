/*
  NYU IMA - Designing Interfaces for Live Performance 2023
  Code for DILP Sensor MIDI Mapping to Ableton Class 2023
  by David Rios
*/

#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <frequencyToNote.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

int b1 = 1;
int pb1 = 1;

int calibrate = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  int fsr = analogRead(A0);
  int photocell = analogRead(A1);
  int pot = analogRead(A2);

  int force = map(fsr, 0, 900, 0, 127);
  int light = map(photocell, 0, 100, 127, 0);
  int knob = map(pot, 0, 1023, 0, 127);

  //Prints for debugging values
  // Serial.print(light);
  // Serial.print(" ");
  // Serial.print(pot);
  // Serial.print(" ");
  // Serial.println(fsr);
  b1 = digitalRead(2);

  if (b1 == 0 && pb1 == 1) {
    pb1 = 0;
    if (calibrate < 8) {
      calibrate++;
      //Serial.println(calibrate);
    } else {
      calibrate = 8;
    }
  }
  if (b1 == 1 && pb1 == 0) {
    pb1 = 1;
  }
 // use a button toggle between which sensor you want to use
  switch (calibrate) {
    case 0:
      Serial.println("Click button to map MIDI Sensors");
      break;
    case 1:
      Serial.println("Click button to map next force Sensor");
      break;
    case 2:
      Serial.println("Mapping force Sensor");
      controlChange(1, 16, force);
      break;
    case 3:
      Serial.println("Click button to map knob Sensor");
      break;
    case 4:
      Serial.println("Mapping knob");
      controlChange(2, 16, knob);
      break;
    case 5:
      Serial.println("Click button to map light Sensor");
      break;
    case 6:
      Serial.println("Mapping light Sensor");
      controlChange(3, 16, light);
      break;
    case 7:
      Serial.println("Click button to start all controls");
      break;
    case 8:
      controlChange(1, 16, force);
      controlChange(2, 16, knob);
      controlChange(3, 16, light);
      delay(5);
      break;
  }
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