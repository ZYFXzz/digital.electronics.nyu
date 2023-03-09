#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=368.3600158691406,353
AudioOutputI2S           i2s1;           //xy=741.3600158691406,365
AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
AudioConnection          patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=621.3600158691406,210
// GUItool: end automatically generated code

int waveforms[4] = { WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH, WAVEFORM_SQUARE };
int ledPins[4] = { 29, 30, 31, 32 };
int potPins[4] = { A14, A15, A16, A17 };

int ledVisualizerPins[6] = { 0, 1, 2, 3, 4, 5};

int tempo = 250;
int tempoPotPin = A11;
int steps;  // how many steps in a sequence
int stepPotPin = A2;
int pitch;
int waveIndex = 0;
int wave;
float volume;
int numberOfLeds;


// pins and their controls:
int octaveButtonPin = 27;
bool octaveState = LOW;
bool lastOctaveState = LOW;
bool octaveHigh = LOW;

int snapSwitchPin = 26;
int systemSwitchPin = 33;
int majorSwitchPin = 28;

int waveButtonPin = 24;
bool waveState = LOW;
bool lastWaveState = LOW;


int controlPins[5] = { majorSwitchPin, octaveButtonPin, snapSwitchPin, systemSwitchPin, waveButtonPin };
float snappedMajorNotes[8] = { 0, 2, 4, 5, 7, 9, 11, 12 };
float snappedMinorNotes[8] = { 0, 2, 3, 5, 7, 8, 10, 12 };

unsigned long lastStepTime = 0;

int currentStep = 0;
int totalSteps = 4;

bool sequenceDirection = true;  //default sequence direction is Right
bool newSequenceDirection;



void setup() {
  AudioMemory(12);  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) { pinMode(ledPins[i], OUTPUT); }
  for (int i = 0; i < 5; i++) { pinMode(controlPins[i], INPUT); }
  for (int i = 0; i < 6; i++) { pinMode(ledVisualizerPins[i], OUTPUT); }

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);
  //  waveform1.begin(WAVEFORM_SINE);// test tone generator
  //  waveform1.amplitude(0.2); //amplitude (volume) can be 0 to 1 and this is usually a good volume for headphones
  //  waveform1.frequency(262);
}

void stepForward() {
  currentStep = currentStep + 1;
  if (currentStep >= 4) {
    currentStep = 0;
  }
  //  Serial.println(currentStep);
}
void stepBackward() {
  currentStep = currentStep - 1;
  if (currentStep <= -1) {
    currentStep = 3;
  }
  //  Serial.println(currentStep);
}
void checkOctave() {
  lastOctaveState = octaveState;
  octaveState = digitalRead(octaveButtonPin);

  if (lastOctaveState == LOW and octaveState == HIGH) {
    octaveHigh = !octaveHigh;
    delay(5);
  } else {
    delay(5);
  }
}
void waveform() {  //choose from 4 waveforms, mapped the pot value to an index number 0-3
  lastWaveState = waveState;
  waveState = digitalRead(waveButtonPin);

  if (lastWaveState == LOW and waveState == HIGH) {
    Serial.println("a");
    waveIndex = waveIndex + 1;
    if (waveIndex + 1 > 4) {
      waveIndex = 0;
    }
    delay(5);
  } else {
    delay(5);
  }
  Serial.println(waveIndex);
  wave = waveforms[waveIndex];
}

void closeVisualizer() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledVisualizerPins[i], LOW);
  }
}


void lightLedVisualizer() {
  closeVisualizer();
  numberOfLeds = map(volume, 0, 0.5, 0, 5);
  for (int i = 0; i <= numberOfLeds; i++) {
    digitalWrite(ledVisualizerPins[i], HIGH);
  }
  
}

//noteStep() is code for generate a note for each step/increment
void noteStep(float note, int i) {  // note, i is the a local variable indicate step

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
  waveform1.amplitude(volume);
  waveform1.frequency(pitch);
  digitalWrite(ledPins[i], HIGH);

  lightLedVisualizer();
}

void ifDirection() {
  if (sequenceDirection) {
    return stepForward();
  } else {
    return stepBackward();
  }
}

void noteSequence(float noteC) {  // the codes to generate a sequence of notes
  steps = analogRead(stepPotPin);
  steps = map(steps, 0, 1023, 1, 4);  // mapped to 1 - 4 steps per sequence
  tempo = analogRead(tempoPotPin);
  tempo = map(tempo, 0, 1023, 150, 1500);  // mapped to 150ms to 1500ms

  if (millis() > lastStepTime + tempo) {
    lastStepTime = millis();
    digitalWrite(ledPins[currentStep], LOW);

    ifDirection();

    noteStep(noteC, currentStep);
  }
}

void checkVolume() {
  volume = analogRead(A0);
  volume = map(volume, 0, 1023, 0.0, 0.5);
}


void loop() {

  if (digitalRead(systemSwitchPin) == HIGH) {

    checkOctave();
    sequenceDirection = newSequenceDirection;  //in the new loop, the SD will change to a new value(newSD)
    waveform();                                //select a waveform
    checkVolume();
    if (octaveHigh) {
      noteSequence(523.25);  // high middle C
    } else {
      noteSequence(261.63);  // low middle C
    }

  } else {

    waveform1.amplitude(0.0);  // mute everything
    digitalWrite(ledPins[currentStep], LOW);
    closeVisualizer();
    newSequenceDirection = !sequenceDirection;  // it means: when switched off, the newSD for the next loop is different than the last loop
    if(newSequenceDirection != sequenceDirection){
      Serial.println("System Off");
    }
  }
}
