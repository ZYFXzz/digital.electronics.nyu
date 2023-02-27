#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=180.36000061035156,281.36000061035156
AudioOutputI2S           i2s1;           //xy=553.3600006103516,293.36000061035156
AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
AudioConnection          patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=433.3600387573242,138.35997486114502
// GUItool: end automatically generated code

int waveforms[4] = {WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH, WAVEFORM_SQUARE};
int ledPins[4] = {29, 30, 31, 32};
int potPins[4] = {A14, A15, A16, A17};

int tempo = 250;
int tempoPin = A11;
int pitch;
float pitch2;
int waveIndex;
int wave;

int octaveSwitchPin = 27;
int snapSwitchPin = 26;
int systemSwitchPin = 33;
int switchPins[3] = {octaveSwitchPin, snapSwitchPin, systemSwitchPin};


int frequencies[4] = {262, 330, 392, 494};

unsigned long lastStepTime = 0;

void setup() {
  AudioMemory(12);  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(switchPins[i], INPUT);
  }
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);
  //  waveform1.begin(WAVEFORM_SINE);// test tone generator
  //  waveform1.amplitude(0.2); //amplitude (volume) can be 0 to 1 and this is usually a good volume for headphones
  //  waveform1.frequency(262);
}

void waveform() {                  //choose from 4 waveforms, mapped the pot value to an index number 0-3
  waveIndex =  analogRead(A10);
  waveIndex = map(waveIndex, 0, 1023, 0, 3);
  wave = waveforms[waveIndex];
}

void noteStep(int noteC) {            // all the codes to generate a note for each step/increment, with an argument of the starting note C position
  for (int i = 0; i < 4; i++) {
    tempo = analogRead(tempoPin);
    tempo = map(tempo, 0, 1023, 150, 1500);
    pitch = analogRead(potPins[i]);

    if (digitalRead(snapSwitchPin) == HIGH) {
      pitch = map(pitch, 0, 1023, 1, 12);
      pitch2 = 261.6 * pow(2, pitch / 12.0);
      Serial.println("Pitch Snapped");
      Serial.print("pitch2");
      Serial.println(pitch2);
    } else {
      pitch2 = map(pitch, 0, 1023, noteC, noteC * 2);
      Serial.println("Pitch un-snapped");
      Serial.print("pitch2");
      Serial.println(pitch2);   // can use the println() to see the snapped value
    }

    waveform1.begin(wave);
    Serial.println(wave);
    waveform1.amplitude(0.2);
    waveform1.frequency(pitch2);
    digitalWrite(ledPins[i], HIGH);
    delay(tempo);
    digitalWrite(ledPins[i], LOW);
  }

}

void loop() {

  if (digitalRead(systemSwitchPin)  == HIGH) {

    waveform();  //select a waveform
    
    if (digitalRead(octaveSwitchPin) == HIGH) {
      noteStep(523);  // high middle C
      Serial.println("octave + 1");
    } else {           // low middle C
      noteStep(261);
    }

  } else {
    waveform1.amplitude(0.0);  // mute everything
  }


}
