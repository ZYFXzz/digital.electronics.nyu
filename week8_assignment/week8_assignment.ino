#include <Adafruit_NeoPixel.h>

// replace the 32 below with whatever pin your Neopixel is connected to
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(2, 5, NEO_RGB);

int tempo = 1000;
int i = 0;

void setup() {
  neopixel.begin();
  neopixel.clear();
  neopixel.show();
}



void loop() {
  // // the Neopixel setPixel function works like...
  // // setPixelColor(int address, int red, int green, int blue);
  // // the address is 0 by default until we start adding more neopixels, just set it to 0 and don't worry about it
  // // red, green, and blue can be numbers from 0 to 255, with zero being off, and 255 being all the way up

  // // turn neopixel red
  // neopixel.setPixelColor(0, 255, 0, 0);
  // neopixel.setPixelColor(1, 255, 0, 0);
  // neopixel.show();
  // delay(tempo);

  // // turn neopixel green
  // neopixel.setPixelColor(0, 0, 255, 0);
  // neopixel.setPixelColor(1, 0, 255, 0);
  // neopixel.show();
  // delay(tempo);

  // // turn neopixel blue
  // neopixel.setPixelColor(0, 0, 0, 255);
  // neopixel.setPixelColor(1, 0, 0, 255);
  // neopixel.show();
  // delay(tempo);

  // // turn neopixel white - by setting same amount of red, green, and blue
  // // setting all to 255 is blindingly bright so I used a lower number
  // neopixel.setPixelColor(0, 100, 100, 100);
  // neopixel.setPixelColor(1, 100, 100, 100);
  // neopixel.show();
  // delay(tempo);

  // // turn neopixel yellow - by mixing some yellow and some green
  // neopixel.setPixelColor(0, 127, 127, 0);
  // neopixel.setPixelColor(1, 127, 127, 0);
  // neopixel.show();
  // delay(tempo);

  // // turn neopixel pink - by mixing some red and some blue
  // neopixel.setPixelColor(0, 127, 0, 127);
  // neopixel.setPixelColor(1, 127, 0, 127);
  // neopixel.show();
  // delay(tempo);

  // // turn neopixel off - by not lighting up any colors
  // neopixel.setPixelColor(0, 0, 0, 0);
  // neopixel.setPixelColor(1, 0, 0, 0);
  // neopixel.show();
  // delay(tempo);



  while (i < 1000) {

    neopixel.setPixelColor(0, random(0, 256), random(0, 256), random(0, 256));
    neopixel.setPixelColor(1, random(0, 256), random(0, 256), random(0, 256));
    neopixel.show();
    delay(tempo);
    i++;
  }
}
