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
int tempoPotPin = A11;
int steps;
int stepPotPin = A2;
int pitch;
int waveIndex;
int wave;

int octaveButtonPin = 27;
int snapSwitchPin = 26;
int systemSwitchPin = 33;
int majorSwitchPin = 28;
int controlPins[4] = {majorSwitchPin, octaveButtonPin, snapSwitchPin, systemSwitchPin};
float snappedMajorNotes[8] = {0, 2, 4, 5, 7, 9, 11, 12};
float snappedMinorNotes[8] = {0, 2, 3, 5, 7, 8, 10, 12};


int frequencies[4] = {262, 330, 392, 494};

unsigned long lastStepTime = 0;
int stepLength = 500;

int currentStep = 0;
int totalSteps = 4;

bool directionRight = true;
bool lastDirectionState = true;

void setup() {
  AudioMemory(12);  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(controlPins[i], INPUT);
  }
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);
  //  waveform1.begin(WAVEFORM_SINE);// test tone generator
  //  waveform1.amplitude(0.2); //amplitude (volume) can be 0 to 1 and this is usually a good volume for headphones
  //  waveform1.frequency(262);
}

void stepForward() {
  currentStep = currentStep + 1;
  if (currentStep == 4) {
    currentStep = 0;
  }
  Serial.println(currentStep);
}
void stepBackward() {
  currentStep = currentStep - 1;
  if (currentStep == -1) {
    currentStep = 3;
  }
  Serial.println(currentStep);
}

void waveform() {                  //choose from 4 waveforms, mapped the pot value to an index number 0-3
  waveIndex =  analogRead(A10);
  waveIndex = map(waveIndex, 0, 1023, 0, 3);
  wave = waveforms[waveIndex];
}

void noteSequence(float noteC) {            // the codes to generate a sequence of notes
  steps = analogRead(stepPotPin);
  steps = map(steps, 0, 1023, 1, 4);
  tempo = analogRead(tempoPotPin);
  tempo = map(tempo, 0, 1023, 150, 1500);

  if (directionRight) {
    Serial.print("Right");
    noteStep(noteC, currentStep);
    digitalWrite(ledPins[currentStep], HIGH);
    if (millis() > lastStepTime + stepLength) {
      lastStepTime = millis();
      digitalWrite(ledPins[currentStep], LOW);
      stepForward();
    }
    } else {
      Serial.print("Left");
      noteStep(noteC, currentStep);
      digitalWrite(ledPins[currentStep], HIGH);
      if (millis() > lastStepTime + stepLength) {
        lastStepTime = millis();
        digitalWrite(ledPins[currentStep], LOW);
        stepBackward();
      }
    }

  }

//noteStep() is code for generate a note for each step/increment
void noteStep(float note, int i) {   // note, i is the a local variable indicate step

  pitch = analogRead(potPins[i]);
  if (digitalRead(snapSwitchPin) == HIGH) {

    pitch = map(pitch, 0, 1023, 0, 7);
    if (digitalRead(majorSwitchPin) == HIGH) {  // major, and minor switch
      pitch = note * pow(2, snappedMajorNotes[pitch] / 12.0);
    } else {
      pitch = note * pow(2, snappedMinorNotes[pitch] / 12.0);
    }
  } else {
    pitch = map(pitch, 0, 1023, note, note * 2);
  }
  waveform1.begin(wave);
  waveform1.amplitude(0.2);
  waveform1.frequency(pitch);
  digitalWrite(ledPins[i], HIGH);
  delay(tempo);
  digitalWrite(ledPins[i], LOW);
}

void loop() {

  if (digitalRead(systemSwitchPin)  == HIGH) {
    Serial.println(directionRight);
    directionRight = lastDirectionState;
    waveform();  //select a waveform

    if (digitalRead(octaveButtonPin) == HIGH) {
      noteSequence(523.25);  // high middle C
    } else {           
      noteSequence(261.63);// low middle C
    }

  } else {
    Serial.println("system off");
    waveform1.amplitude(0.0);  // mute everything
    lastDirectionState = !directionRight;
    

  }


}
