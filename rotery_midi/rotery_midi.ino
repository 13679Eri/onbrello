//可変抵抗だと思って買ったらロータリーコード？でした
//0~9で音量変えれるようにしたやつ
//https://akizukidenshi.com/catalog/g/g110399/

#include <SFZSink.h>

SFZSink sink("SawLpf.sfz");  // SDカードに置いたSFZファイルを指定
bool flag = false;
int prev_value = 0;
int note = 45; //ラ(音量変えたwavを作成した)

int pin1 = 2;
int pin2 = 3;
int pin4 = 4;
int pin8 = 5;

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  //setup instrument
  if (!sink.begin()) {
    Serial.println("ssproclib init error.");
    while (true) {
      delay(1000);
    }
  } else {
    Serial.println("setup ok");
  }
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(pin4, INPUT_PULLUP);
  pinMode(pin8, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  int value = (digitalRead(pin1) ? 0 : 8)
              | (digitalRead(pin2) ? 0 : 16)
              | (digitalRead(pin4) ? 0 : 32)
              | (digitalRead(pin8) ? 0 : 64);  //8~120で音量
  
  // Serial.print(digitalRead(pin1));
  // Serial.print(",");
  // Serial.print(digitalRead(pin2));
  // Serial.print(",");
  // Serial.print(digitalRead(pin4));
  // Serial.print(",");
  // Serial.println(digitalRead(pin8));

  if (value != prev_value) {
    if (flag) {
      Serial.println(value);
      flag = false;
      sink.sendNoteOff(note, 0, DEFAULT_CHANNEL);
      delay(10);
      sink.sendNoteOn(note, value, DEFAULT_CHANNEL);
    }
  } else {
    if (!flag) {
      Serial.println(value);
      flag = true;
      sink.sendNoteOn(note, value, DEFAULT_CHANNEL);
    }
  }
  prev_value = value;

  sink.update();
  delay(20);
}
