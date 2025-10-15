/**
 * @file drum.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief Unit Synth Drum Example
 * @version 0.2
 * @date 2024-04-07
 *
 *
 * @Hardwares: M5Core + Unit Synth
 * @Platform Version: Arduino M5Stack Board Manager v2.1.0
 * @Dependent Library:
 * M5UnitSynth: https://github.com/m5stack/M5Unit-Synth
 */

#include <M5Core2.h>
#include "M5UnitSynth.h"

M5UnitSynth synth;

int velo = 0;
int vibValue = 0;        // ビブラート値 (0〜127)
bool vibActive = false;  // ビブラート中かどうか
unsigned long lastVibTime = 0;
bool noteOn = false;  // 音が鳴っているかどうか

// 設定
const uint8_t CHANNEL = 0;
const uint8_t NOTE = 60;  // C4

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.println("Unit Synth Drum");
  synth.begin(&Serial2, UNIT_SYNTH_BAUD, 33, 32);
  synth.setVolume(CHANNEL, 127);        // 初期音量
  synth.setVibrate(CHANNEL, 0, 0, 0);  // 初期ビブラートなし
}

void loop() {
  M5.update();

  if (M5.BtnA.isPressed()) {
    synth.setInstrument(0, CHANNEL, 74);
    // Serial.println("A");
    if (!noteOn) {
      synth.setNoteOn(CHANNEL, 80, 80);  // 音を出す
      noteOn = true;
    }

    if (velo < 127) {
      Serial.println(velo);
      velo++;
      synth.setVolume(0, velo);  // CC#11 = Expression
      delay(10);                 // 上昇スピード（調整可能）
    }
  } else if (M5.BtnA.wasReleased()) {
    synth.setNoteOff(CHANNEL, 80, 0);  // 音を止める
    noteOn = false;

    velo = 0;
    synth.setVolume(0, velo);
  }

  if (M5.BtnB.isPressed()) {
    synth.setVolume(CHANNEL, 127);
    synth.setInstrument(0, CHANNEL, 41);
    if (!vibActive) {
      vibActive = true;
      if (!noteOn) {
        synth.setNoteOn(CHANNEL, NOTE, 127);
        noteOn = true;
      }
    }
  } else if (M5.BtnB.wasReleased()) {
    vibActive = false;
    synth.setVibrate(CHANNEL, 0, 0, 0);
    if (noteOn) {
      synth.setNoteOff(CHANNEL, NOTE, 0);
      noteOn = false;
    }
  }

  if (vibActive) {
    unsigned long now = millis();
    if (now - lastVibTime > 200) {  // 200msごとに更新
      lastVibTime = now;
      static int phase = 0;
      int rate = 40;                                       // ビブラートの速さ (0〜127)
      int depth = abs(sin(phase * 3.14159 / 180.0) * 40);  // 揺れの深さ
      int delay = 0;                                       // 0で即時反映
      synth.setVibrate(0, rate, depth, delay);
      phase += 20;
      if (phase >= 360) phase = 0;
      Serial.print(rate);
      Serial.print(",");
      Serial.println(phase);
    }
  }
  delay(5);
}

void generate_velo() {
  if (velo >= 0) {
    velo++;
  } else if (velo == 127) {
    velo = 127;
  }
  Serial.println(velo);
}

void piano(int velo) {
  synth.setNoteOn(1, 1, velo);
}

void durm() {
  synth.setNoteOn(9, 36, velo);
  synth.setNoteOn(9, 42, velo);
  delay(300);
  synth.setNoteOn(9, 42, velo);
  delay(300);
  synth.setNoteOn(9, 38, velo);
  synth.setNoteOn(9, 42, velo);
  delay(300);
  synth.setNoteOn(9, 42, velo);
  delay(300);

  synth.setNoteOn(9, 36, velo);
  synth.setNoteOn(9, 42, velo);
  delay(300);
  synth.setNoteOn(9, 36, velo);
  synth.setNoteOn(9, 42, velo);
  delay(300);
  synth.setNoteOn(9, 38, velo);
  synth.setNoteOn(9, 42, velo);
  delay(300);
  synth.setNoteOn(9, 42, velo);
  delay(300);
}
