//arduinoのライブラリのAudioを使用して圧力センサのアナログ値で音量を変える　音はランダム
//うぃんうぃん言うからwinwin....
//ふざけた名前ですみません...

#include <Audio.h>

AudioClass *theAudio;
const int ana_pin = A0;

int noteFrequencies[16] = {
  262, 277, 294, 311, 330, 349, 370, 392,
  415, 440, 466, 494, 523, 554, 587, 622
};

bool isPlaying = false;  // 今音を出しているか
int currentFreq = 0;     // 今の音程

void setup() {
  Serial.begin(115200);
  theAudio = AudioClass::getInstance();
  theAudio->begin();
  theAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, 0, 0);
}

void loop() {
  int val = analogRead(ana_pin);
  // Serial.println(val);

  // 圧力→音量(-40〜0dB)
  float norm = (float)(val - 80) / (570 - 80);  // 0.0〜1.0に正規化
  norm = constrain(norm, 0.0, 1.0);
  int velo = 0 - (int)(norm * 40);  // 0〜-40 に変換

  if (val <= 550) {
    if (!isPlaying) {
      // 押し始めなら音程を決める
      int ran_num = random(0, 15);
      currentFreq = noteFrequencies[ran_num];
      isPlaying = true;
    }
    // 押している間は音量だけ変える
    Serial.println(velo);
    theAudio->setBeep(1, velo, currentFreq);
  } else {
    if (isPlaying) {
      // 離した瞬間に止める
      theAudio->setBeep(0, 0, 0);
      isPlaying = false;
    }
  }
}
