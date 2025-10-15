//sfzsink使ってやってみた
//16ボタンさん使用　ssci.to/9171
//16この音をボタンに配置して押したらSDカードに入ってるwavファイルの音が出る
//sfzファイルにノート番号とwavの紐付けが記載されている

#include <Arduino.h>
#include <SFZSink.h>
#include "ShiftregisterSwScanner.h"

constexpr size_t NUM_SW = 15;
constexpr uint32_t PIN_SERIAL_OUT = 4;
constexpr uint32_t PIN_NPL        = 5;
constexpr uint32_t PIN_CP         = 6;

int buffer[NUM_SW] = {};

// ドから半音階 16音（MIDIノート番号）
int midiNotes[NUM_SW] = {
  60, 61, 62, 63, 64, 65, 66, 67,   // C4〜G4
  68, 69, 70, 71, 72, 73, 74   // G#4〜D#5
};

int velo_array[3] = {127, 64, 5};

SFZSink sink("SawLpf.sfz");   // SDカードに置いたSFZファイルを指定

void onSwitchChanged(const uint32_t switch_index, const int off_on) {
  buffer[switch_index] = off_on;

  // 状態表示
  for (auto i = 0u; i < NUM_SW; ++i) {
    Serial.print(buffer[i] ? 1 : 0);
  }
  Serial.println("");

  int count = 0;
  for(int i = 0;i<3;i++){
    if(count >= 2) count = 0;
    else count++;
  }

  // 音のオン・オフ制御
  int note = midiNotes[switch_index];
  int velo = 127;

  if (off_on == 1) {  
    // 押されたとき → MIDIノートオン
    sink.sendNoteOn(note, velo, DEFAULT_CHANNEL);
    Serial.println(velo);
  } else {
    // 離されたとき → MIDIノートオフ
    sink.sendNoteOff(note, 0, DEFAULT_CHANNEL);
  }
}

kinoshita_lab::ShiftregisterSwScanner swScanner(
    PIN_NPL, PIN_CP, PIN_SERIAL_OUT, onSwitchChanged, NUM_SW);

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  //setup instrument
  if (!sink.begin()) {
    Serial.println("ssproclib init error.");
    while (true) {
      delay(1000);
    }
  }else{
    Serial.println("setup ok");
  }

  swScanner.update();
}

void loop() {
  swScanner.update();
  sink.update();
}
