//一応アナログリード用サンプル残しとく
const int ana_pin = A0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int val = analogRead(ana_pin);
  Serial.println(val);
  delay(50);
}
