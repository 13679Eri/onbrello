//ロータリーコードのみ

int pin1 = 2;
int pin2 = 3;
int pin4 = 4;
int pin8 = 5;

void setup() {
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(pin4, INPUT_PULLUP);
  pinMode(pin8, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  int value = (digitalRead(pin1) ? 0 : 1)
              | (digitalRead(pin2) ? 0 : 2)
              | (digitalRead(pin4) ? 0 : 4)
              | (digitalRead(pin8) ? 0 : 8);

  Serial.print(digitalRead(pin1));
  Serial.print(",");
  Serial.print(digitalRead(pin2));
  Serial.print(",");
  Serial.print(digitalRead(pin4));
  Serial.print(",");
  Serial.println(digitalRead(pin8));

    Serial.println(value);
  delay(200);
}
