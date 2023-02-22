#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <frequencyToNote.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

int pfsr = 0;
int pPot = 0;
int pPhoto = 0;

int lthreshold = 50;
int kthreshold = 50;
int fthreshold = 20;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pfsr = analogRead(A0);
  pPhoto = analogRead(A1);
  pPot = analogRead(A2);
}

void loop() {
  int fsr = analogRead(A0);
  int photocell = analogRead(A1);
  int pot = analogRead(A2);

  int force = map(fsr, 0, 900, 0, 127);
  int light = map(photocell, 0, 900, 127, 0);
  int knob = map(pot, 0, 1023, 0, 127);

  if (abs(fsr - pfsr) >= fthreshold) {
    controlChange(1, 17, force);
    pfsr = fsr;
    Serial.print(abs(fsr - pfsr));
    Serial.print(" fsr ");
    Serial.println(fsr);
    delay(5);
  }

  if (abs(photocell - pPhoto) >= lthreshold) {
    light = constrain(light,0,127);
    controlChange(2, 17, light);
    pPhoto = photocell;
    Serial.print(abs(photocell - pPhoto));
    Serial.print(" light cell ");
    Serial.println(photocell);
    delay(10);
  }

  if (abs(pot - pPot) >= kthreshold) {
    controlChange(3, 17, knob);
    pPot = pot;
    Serial.print(abs(pot - pPot));
    Serial.print(" pot ");
    Serial.println(pot);
    delay(5);
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