int cLedPin = 7;
int dLedPin = 8;
int eLedPin = 9;
int fLedPin = 10;
int gLedPin = 11;

int cButtPin = 37;
int dButtPin = 36;
int eButtPin = 35;
int fButtPin = 34;
int gButtPin = 33;

int octaveSwitchPin = 38;
int arpSwitchPin = 29;
int velSwitchPin = 28;
int vel = 127;
int scaleSwitchPin = 30;

int potVal = 0;
int mappedPotVal = 0;
int mappedNumber = 0;
int lastMappedNumber = 0;

int octave = 0;

int noteCc = 60;   //note Xx indicates a initial value of those buttons, for calculation use
int noteC;
int noteDd = 62;
int noteD;
int noteEe = 64;
int noteE;
int noteFf = 65;
int noteF;
int noteGg = 67;
int noteG;

int arpScale = 4;

bool octaveState = false;
bool lastOctaveState = false;
bool arp;
bool key;

void printMode() {

}

void setup() {

  Serial.begin(9600);

  pinMode(cButtPin, INPUT);
  pinMode(dButtPin, INPUT);
  pinMode(eButtPin, INPUT);
  pinMode(fButtPin, INPUT);
  pinMode(gButtPin, INPUT);

  pinMode(cLedPin, OUTPUT);  // put your setup code here, to run once:
  pinMode(dLedPin, OUTPUT);
  pinMode(eLedPin, OUTPUT);
  pinMode(fLedPin, OUTPUT);
  pinMode(gLedPin, OUTPUT);

  pinMode(octaveSwitchPin, INPUT);
  pinMode(arpSwitchPin, INPUT);
  pinMode(velSwitchPin, INPUT);
}

void potIfOctave() {

  lastOctaveState = octaveState;
  //
  //  if (digitalRead(octaveSwitchPin) == HIGH) {
  //    octave = 1;
  //    octaveState = true;
  //  } else {
  //    octave = 0;
  //    octaveState = false;
  //  }


  mappedNumber = mappedPotVal;                                                        // used the pot() from week3 assignment

  potVal = analogRead(A13);  // put your main code here, to run repeatedly:
  mappedPotVal = map(potVal, 0, 1023, -4, 4);
  octave = mappedPotVal;
  lastMappedNumber = mappedPotVal;
  if (lastMappedNumber != mappedNumber) {
    Serial.print("pot value is ");
    Serial.println(mappedPotVal);
  }
  delay(10);

  //  if (octaveState != lastOctaveState) {
  //    Serial.println(" octave change");
  //  }
  //  noteC = 60 + 12 * octave;
  //  noteD = 62 + 12 * octave;
  //  noteE = 64 + 12 * octave;
  //  noteF = 65 + 12 * octave;
  //  noteG = 67 + 12 * octave;
  noteC = noteCc + (12 * octave);
  noteD = noteDd + (12 * octave);
  noteE = noteEe + (12 * octave);
  noteF = noteFf + (12 * octave);
  noteG = noteGg + (12 * octave);

}

void ifOctave() {

  lastOctaveState = octaveState;

  if (digitalRead(octaveSwitchPin) == HIGH) {
    octave = 1;
    octaveState = true;
  } else {
    octave = 0;
    octaveState = false;
  }

  if (octaveState != lastOctaveState) {
    Serial.print("octave change ");
    Serial.println(octave);
  }
  //  noteC = 60 + (12 * octave);
  //  noteD = 62 + (12 * octave);
  //  noteE = 64 + (12 * octave);
  //  noteF = 65 + (12 * octave);
  //  noteG = 67 + (12 * octave);



}


void ifArp() {
  if (digitalRead(arpSwitchPin) == HIGH) {
    arp = true;
    key = false;

  } else {
    arp = false;
    key = true;
  }

}

void randomVel() {
  if (digitalRead(velSwitchPin) == HIGH) {
    vel = random(30, 127);
  }
}

void ifScale() {
  if (digitalRead(scaleSwitchPin) == HIGH) {
    noteEe = 63;
  } else {
    noteEe = 64;
  }
}




void checkKeyboardButt(int buttPin, int ledPin, int note) {

  if (digitalRead(buttPin) == HIGH) {
    Serial.println("button is pressed");

    usbMIDI.sendNoteOn(note, vel, 1);
    digitalWrite(ledPin, HIGH);
    delay(500);
    usbMIDI.sendNoteOff(note, 0, 1);
    digitalWrite(ledPin, LOW);

  }

}

void keyMode() {

  checkKeyboardButt(cButtPin, cLedPin, noteC);
  checkKeyboardButt(dButtPin, dLedPin, noteD);
  checkKeyboardButt(eButtPin, eLedPin, noteE);
  checkKeyboardButt(fButtPin, fLedPin, noteF);
  checkKeyboardButt(gButtPin, gLedPin, noteG);

}

void arpMode(int note) {

  while (digitalRead(cButtPin) == HIGH) {

    //    for (int note = 0, note <= 4, i ++) {
    //      randomVel();
    //      Serial.print("velocity is ");
    //      Serial.println(vel);
    //      int note0 = note;
    //      usbMIDI.sendNoteOn(note0, vel, 1);
    //      digitalWrite(cLedPin, HIGH);
    //      Serial.print("note is ");
    //      Serial.println(note0);
    //      delay(500);
    //      usbMIDI.sendNoteOff(note0, 0, 1);
    //      digitalWrite(cLedPin, LOW);
    //    }

    Serial.println("Arp Mode");

    randomVel();
    Serial.print("velocity is ");
    Serial.println(vel);
    int note0 = note;
    usbMIDI.sendNoteOn(note0, vel, 1);
    digitalWrite(cLedPin, HIGH);
    Serial.print("note is ");
    Serial.println(note0);
    delay(500);
    usbMIDI.sendNoteOff(note0, 0, 1);
    digitalWrite(cLedPin, LOW);

    randomVel();
    Serial.print("velocity is ");
    Serial.println(vel);
    int note1 = note + 4;
    usbMIDI.sendNoteOn(note + 4, vel, 1);
    digitalWrite(dLedPin, HIGH);
    Serial.print("note is ");
    Serial.println(note1);
    delay(500);
    usbMIDI.sendNoteOff(note1, 0, 1);
    digitalWrite(dLedPin, LOW);

    randomVel();
    Serial.print("velocity is ");
    Serial.println(vel);
    int note2 = note1 + 3;
    usbMIDI.sendNoteOn(note2, vel, 1);
    digitalWrite(eLedPin, HIGH);
    Serial.print("note is ");
    Serial.println(note2);
    delay(500);
    usbMIDI.sendNoteOff(note2, 0, 1);
    digitalWrite(eLedPin, LOW);

    randomVel();
    Serial.print("velocity is ");
    Serial.println(vel);
    int note3 = note2 + 4;
    usbMIDI.sendNoteOn(note3, vel, 1);
    digitalWrite(fLedPin, HIGH);
    Serial.print("note is ");
    Serial.println(note3);
    delay(500);
    usbMIDI.sendNoteOff(note3, 0, 1);
    digitalWrite(fLedPin, LOW);
  }

}


void loop() {

  ifScale();
  potIfOctave();
  ifArp();

  randomVel();
  if (arp == true) {
    arpMode(noteC);
  } else if (key == true) {
    keyMode();
  }

}
