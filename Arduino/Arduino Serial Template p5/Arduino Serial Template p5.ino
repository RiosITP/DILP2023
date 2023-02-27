void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  int b1 = digitalRead(2);
  int b2 = digitalRead(3);
  int b3 = digitalRead(4);
  int pot1 = analogRead(A0);
  int pot2 = analogRead(A1);

  if (Serial.available() > 0) {
    int incoming = Serial.read();
    Serial.print(b1);
    Serial.print(",");
    Serial.print(b2);
    Serial.print(",");
    Serial.print(b3);
    Serial.print(",");
    Serial.print(pot1);
    Serial.print(",");
    Serial.println(pot2);
  }
}
