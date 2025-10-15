//Audio関数でSpresenseで音鳴らすことができたのでボタンで鳴らすのをやってみた
//16ボタンさん使用 ssci.to/9171

#include <Arduino.h>
#include <Audio.h>
#include "ShiftregisterSwScanner.h"

constexpr size_t NUM_SW = 16;
constexpr uint32_t PIN_SERIAL_OUT = 4;
constexpr uint32_t PIN_NPL = 5;
constexpr uint32_t PIN_CP = 6;

AudioClass *theAudio;

int buffer[NUM_SW] = {};

//ドから2オクターブ
// int noteFrequencies[NUM_SW] = {
//   262, 294, 330, 349, 392, 440, 494, 523,
//   587, 659, 698, 784, 880, 988, 1047, 1175
// };

int noteFrequencies[NUM_SW] = {
  262, 277, 294, 311, 330, 349, 370, 392,
  415, 440, 466, 494, 523, 554, 587, 622
};

void onSwitchChanged(const uint32_t switch_index, const int off_on) {
  buffer[switch_index] = off_on;

  for (auto i = 0u; i < NUM_SW; ++i) {
    Serial.print(buffer[i] ? 1 : 0);
  }
  Serial.println("");

  // 音のオン・オフ制御
  if (off_on == 1) {  // 押されたとき
    int freq = noteFrequencies[switch_index];
    theAudio->setBeep(1, -40, freq);  // 音量-40dBで鳴らす
  } else {
    // 離されたら音を止める
    theAudio->setBeep(0, 0, 0);
  }
}

kinoshita_lab::ShiftregisterSwScanner swScanner(PIN_NPL, PIN_CP, PIN_SERIAL_OUT,
                                                onSwitchChanged, NUM_SW);

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:

  // Audio 初期化
  theAudio = AudioClass::getInstance();
  theAudio->begin();
  theAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, 0, 0);  // スピーカーモード

  swScanner.update();
}

void loop() {
  swScanner.update();
  // delay(100);
  //  put your main code here, to run repeatedly:
}