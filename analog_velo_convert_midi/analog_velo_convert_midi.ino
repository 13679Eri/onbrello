//圧力センサのアナログ値を音量に変換してmidiの命令でwavファイルを鳴らす
#include <Arduino.h>
#include <SFZSink.h>

const int ana_pin = A0;
bool isPlaying = false;  // 今音を出しているか
int currentVelo = 0;
int note = 45;

// ドから半音階 16音（MIDIノート番号）
// int midiNotes[13] = {
//   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
// };

SFZSink sink("SawLpf.sfz");  // SDカードに置いたSFZファイルを指定

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
}

void loop() {
  int val = analogRead(ana_pin);
  bool flag = false;

  // if (!push) {
  //   if (!flag) {
  //     flag = true;
  //     Serial.println("on");
  //     sink.sendNoteOn(note, 80, 0);
  //   }
  // } else if (push && flag) {
  //   flag = false;
  //   sink.sendNoteOff(note, 0, 0);
  // }

  //Serial.println(val);

  // 圧力→音量(10〜127)
  int velo = (val - 80) * (0 - 127) / (570 - 80) + 127;
  // Serial.println(velo);

  if (val <= 550) {
    if (!isPlaying) {
      isPlaying = true;
      sink.sendNoteOn(note, velo, 1);
      currentVelo = velo;
      Serial.print("note on velo: ");
      Serial.println(velo);
    } else if (abs(velo - currentVelo) > 10) {
      // 音量差が大きい時のみ再トリガー
      sink.sendNoteOff(note, 0, 1);
      // delay(20);
      sink.sendNoteOn(note, velo, 1);
      currentVelo = velo;
      Serial.print("update velo: ");
      Serial.println(velo);
    }
  } else if (isPlaying) {
    sink.sendNoteOff(note, 0, 1);
    isPlaying = false;
    Serial.println("note off");
  }

  sink.update();
  // delay(10);
}