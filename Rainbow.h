#ifndef Rainbow_h
#define Rainbow_h

#include <Particle.h>
#include <neopixel.h>

#include "Globals.h"
#include "OperationMode.h"

class Rainbow {
private:
    Adafruit_NeoPixel* s;
    Timer* timer;
    uint16_t j;
    
public:
    Rainbow(Adafruit_NeoPixel* strip) : s(strip), j(0) {
        Particle.function("makeRainbow", &Rainbow::SetRainbowMode, this);
        timer = new Timer(20, &Rainbow::UpdateLeds, *this);
    }
    
    ~Rainbow() {
        if (timer) {
            delete timer;
            timer = 0;
        }
    }

    void MakeRainbow() {
        if (timer->isActive()) {
            return;
        }

        SetOperationMode(RAINBOW);
        timer->start();
    }
    
    void UpdateLeds() {    
        j++;
        if (j >= 256 ) {
            j = 0;
        }

        uint16_t i;
        for(i=0; i< s->numPixels(); i++) {
            s->setPixelColor(i, Wheel(s, (i+j) & 255));
        }
        
        s->show();
        if (operationMode != RAINBOW) {
            timer->stop();
        }
    }
   
    int SetRainbowMode(String)
    {
        MakeRainbow();
        return 0;
    }

    bool IsRainbowActive() {
        return operationMode == RAINBOW && timer->isActive();
    }
};

#endif // Rainbow_h

