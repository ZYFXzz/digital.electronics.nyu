int ledPins[5] = {7, 8, 9, 10, 11};

int arpSwitchPin = 29;
int octaveSwitchPin = 28;
int chordSwitchPin = 30;
int systemSwitchPin = 33;

int analogPots[4] = {A14, A15, A16, A17};

int switchPins[4] = {29, 28, 30, 33};

int vel = 127;

int arpValues[4] = {0, 4, 7, 11};

int octave = 0;

int delayTime = 500;

int notes[5] = {60, 62, 64, 65, 67};
int modifiedNotes[5];

bool delayState = false;
bool lastDelayState = false;


void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(switchPins[i], INPUT);
  }
}



void potDelayTime() {

  lastDelayState = delayState;

  int delayPotVal = analogRead(A12);  // put your main code here, to run repeatedly:
  int mappedDelayPotVal = map(delayPotVal, 0, 1023, -4, 4);
  int lastMappedNumber = mappedDelayPotVal;

  if (lastMappedNumber != mappedDelayPotVal) {
    Serial.print("pot value is ");
    Serial.println(mappedDelayPotVal);
  }
  delay(10);

  delayTime = 1000 + (mappedDelayPotVal * 150);
  Serial.print("delayTime is ");
  Serial.println(delayTime);
}


void sequence() {                    // the sequence function, play 4 notes in a role

  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
    int a = analogRead(analogPots[i]);
    int note = map(a, 1023, 0, 60, 73 );
    if (digitalRead(octaveSwitchPin) == HIGH) {
      note = note + 12;
    }
    Serial.print(i);
    Serial.print(" ");
    Serial.println(note);
    if (digitalRead(chordSwitchPin) == HIGH) {
      vel = 127;
      for (int j = 0; j < 4; j ++) {
        int n = note + arpValues[j];
        usbMIDI.sendNoteOn(n, vel, 1);
      }
      delay(50);
      for (int j = 0; j < 4; j ++) {
        int n = note + arpValues[j];
        usbMIDI.sendNoteOff(n, 0, 1);
      }
      delay(delayTime);
      digitalWrite(ledPins[i], LOW);
    } else if (digitalRead(arpSwitchPin) == HIGH) {
      for (int j = 0; j < 4; j ++) {
        int n = note + arpValues[j];
        usbMIDI.sendNoteOn(n, vel, 1);
        delay(50);
        usbMIDI.sendNoteOff(n, 0, 1);
      }
      delay(delayTime);
      digitalWrite(ledPins[i], LOW);
    } else {
      usbMIDI.sendNoteOn(note, vel, 1);
      delay(delayTime);
      digitalWrite(ledPins[i], LOW);
      usbMIDI.sendNoteOff(note, 0, 1);
    }

  }
}


void loop() {

  potDelayTime();

  if (digitalRead(systemSwitchPin) == HIGH) {
    sequence();
  } else {
    digitalWrite(ledPins[4], HIGH);
    delay(1000);
    digitalWrite(ledPins[4], LOW);
    delay(1000);
  }

}
