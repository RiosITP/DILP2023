/*
  Arduino to TouchDesigner Template
  NYU / IMA 
  Designing Interfaces for Live Performances 2023
  by David Rios
*/

void setup() {
  // Wait for a serial Connection to TD. If theres no connection the sketch freezes
  while(!Serial);
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  int b1 = digitalRead(2);
  int b2 = digitalRead(3);
  int b3 = digitalRead(4);
  int fsr = analogRead(A0);
  int light = analogRead(A1);
  int pot = analogRead(A2);
  
  // print comma separated values, send \n control character at the end
  // Serial.println() should also work
  Serial.print(b1);
  Serial.print(",");
  Serial.print(b2);
  Serial.print(",");
  Serial.print(b3);
  Serial.print(",");
  Serial.print(fsr);
  Serial.print(",");
  Serial.print(light);
  Serial.print(",");
  Serial.print(pot);
  Serial.print("\n");
}
