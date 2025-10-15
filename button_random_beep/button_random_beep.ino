//arduinoのaudioライブラリを使ってボタンを押したらLEDがついて音(ランダム)が鳴る

#include <Audio.h>
AudioClass *theAudio;

int noteFrequencies[16] = {
  262, 277, 294, 311, 330, 349, 370, 392,
  415, 440, 466, 494, 523, 554, 587, 622
};

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
bool isPlaying = false;
int currentFreq = 0;  // 今の音程

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  theAudio = AudioClass::getInstance();
  theAudio->begin();
  theAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, 0, 0);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    if (!isPlaying) {
      int ran_num = random(0, 15);
      currentFreq = noteFrequencies[ran_num];
      theAudio->setBeep(1, 0, currentFreq);
      // turn LED on:
      Serial.println("HIGH");
      digitalWrite(ledPin, HIGH);
      isPlaying = true;
    }
  } else {
    if (isPlaying) {
      // turn LED off:
      digitalWrite(ledPin, LOW);
      theAudio->setBeep(0, 0, 0);
      isPlaying = false;
    }
  }
}
