#ifndef Globals_h
#define Globals_h

#include <Particle.h>
#include <neopixel.h>

#define HEX(x) (x <= '9' ? x - '0' : (x <= 'F' ? x - 'A' : x - 'a') + 10)

uint32_t Wheel(Adafruit_NeoPixel* strip, byte WheelPos) {
  if(WheelPos < 85) {
   return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

#endif // Globals_h

