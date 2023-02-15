#include <Wire.h>
#include <VL53L1X.h>
#include <MadgwickAHRS.h>
#include <Arduino_LSM6DS3.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <frequencyToNote.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

int dist = 0;
float ptwist=0.0;
int distInt = 200;
int notes[] = { 47, 48, 50, 52, 53, 55, 59, 60, 62, 64, 65, 67, 71 };
int notesLow[] = { 29, 31, 36, 41, 43, 47, 48 };
int notesOn[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int notesLowOn[] = { 0, 0, 0, 0, 0, 0, 0 };

int arrSize = 7;
int arrSize2 = 13;

int noteOffInt = 10000;
long ptime = 0;
long ptimeTurn = 0;

VL53L1X sensor;
// initialize a Madgwick filter:
Madgwick filter;
// sensor's sample rate is fixed at 104 Hz:
const float sensorRate = 104.00;

// values for orientation:
float twist = 0.0;
float tilt = 0.0;
float turn = 0.0;
// variable for changing midinotes
int midinote;
float pTilt = 0.0;
float pTurn = 0.0;
float center = 0.0;

// threshold for when to trigger or reset a note
int threshold = 2;

float iTilt;
float iTurn;

bool initi = 0;

void setup() {
  while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000);  // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1)
      ;
  }
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);
  sensor.startContinuous(33);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    // stop here if you can't access the IMU:
    while (true)
      ;
  }
  filter.begin(sensorRate);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  // ToF sensor
  dist = sensor.read();
  // Serial.print(dist);
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  //Serial.println();

  if (dist > 0 && dist < 1000) {
    for (int i = 0; i < arrSize; i++) {
      if (dist >= i * distInt && (i * distInt) + distInt && notesLowOn[i] == 0) {
        noteOn(0, notesLow[i], 90);
        notesLowOn[i] = 1;
        Serial.print(notesLow[i]);
        Serial.println(" on");
        for (int j = 0; j < arrSize; j++) {
          if (j != i) {
            noteOff(0, notesLow[j], 127);
            notesLowOn[j] = 0;
            Serial.print(notes[j]);
            Serial.println(" off");
          }
        }
      }
    }


  } else {
    if (millis() - ptime >= noteOffInt) {
      for (int i = 0; i < arrSize; i++) {
        noteOff(0, notesLow[i], 127);
        Serial.print(notesLow[i]);
        Serial.println(" off");
        ptime = millis();
      }
    }
  }

  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;

  // check if the IMU is ready to read:
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    // read accelerometer & gyrometer:
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro);

    // update the filter, which computes orientation:
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);

    // print the heading, pitch and roll
    twist = filter.getRoll();
    tilt = filter.getPitch();
    turn = filter.getYaw();
  }
  /// instrument range -160 - 120
  if (initi == 0) {
    pTurn = turn;
    pTilt = tilt;
    center = turn;
    initi = 1;
  }

  midinote = map(turn, center - 15, center + 15, arrSize2, 0);
  midinote = constrain(midinote, 0, arrSize2);

  Serial.print("cnt: ");
  Serial.print(center);
  Serial.print(" tnr ");
  Serial.print(turn);
  Serial.print(" note: ");
  Serial.print(midinote);
  Serial.print(" twist: ");
  Serial.print(twist);
  Serial.print(" tlt: ");
  Serial.println(tilt);
  // twist
  int twistv = map(twist,30,-30,0,127);
  twistv = constrain(twistv,0,127);
  if(abs(twist-ptwist)>5 ){
    controlChange(2,16,twistv);
  }
  if (tilt > threshold) {
    if (notesOn[midinote] == 0) {
      noteOn(1, notes[midinote], 80);
      notesOn[midinote] = 1;
    } else {
      /// do nothing
    }
    pTilt = tilt;
  } else {
    for (int i = 0; i < arrSize2; i++) {
      noteOff(1, notes[i], 80);
      notesOn[i] = 0;
    }

    pTilt = tilt;
  }
  int rst = digitalRead(5);
  if (rst == 0) {
    center = turn;
    threshold = tilt;
    for (int i = 0; i < 128; i++) {
      noteOff(1, notes[i], 80);
    }
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
