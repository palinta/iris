#ifndef ColorManager_h
#define ColorManager_h

#include <Particle.h>
#include <neopixel.h>

#include "Globals.h"
#include "OperationMode.h"

#define COLOR_COUNT 255
const uint16_t PIXEL_COUNT = 5;

class ColorManager {
private:
    Adafruit_NeoPixel* s;
    uint8_t idList[COLOR_COUNT];
    uint32_t colorList[COLOR_COUNT];
    uint16_t index;
    uint8_t brightness;
    
public:
    ColorManager(Adafruit_NeoPixel* strip) : s(strip), index(0), brightness(128) {
        Particle.function("defineColor", &ColorManager::DefineColor, this);
        Particle.function("setColor", &ColorManager::SetColor, this);
        Particle.function("brightness", &ColorManager::SetBrightness, this);
    }
    
    ~ColorManager(){
        // empty
    }
    
    // DefineColor - defines a color with an ID, and the R, G, B, W (optional) parameters
    // color string format: N#XXXXXX#YY
    // N      - the color id
    // XXXXXX - the color in hex format
    // YY     - the white parameter - optional
    int DefineColor(String color)
    { 
        if (color.length() < 8) {
            return -1;
        }
        uint8_t id = color[0];
        uint8_t startIndex = 2;
        uint8_t wIndex = 9;
        
        uint8_t r = HEX(color[startIndex + 0]) * 16 + HEX(color[startIndex + 1]);
        uint8_t g = HEX(color[startIndex + 2]) * 16 + HEX(color[startIndex + 3]);
        uint8_t b = HEX(color[startIndex + 4]) * 16 + HEX(color[startIndex + 5]);
        
        uint8_t w = 255;
        
        if (color.length() == (wIndex + 2)) {
            w = HEX(color[wIndex + 0]) * 16 + HEX(color[wIndex + 1]);
        }

        this->SetColorByID(id, s->Color(r, g, b));
        
        Particle.publish("Defined new color", color.substring(1));
        return 0;   
    }
    
    // SetBrightness - set the brightness of the led strip
    // Parameter format: FF - a value between 0 - 255 in hex format
    int SetBrightness(String brightness) {
        if (brightness.length() < 2) {
            return -1;
        }
        this->brightness = HEX(brightness[0]) * 16 + HEX(brightness[1]);
        Particle.publish("Set Brightness", String(this->brightness));
        s->setBrightness(this->brightness);
        s->show();
        return 0;
    }

    void SetBrightnessByDirectValue(int brightness) {
        Particle.publish("Set Brightness", String(brightness));
        if (brightness <=0 || brightness > 255) {
            return;
        }
        this->brightness = brightness;
        s->setBrightness(this->brightness);
        s->show();
    }
    
    int SetColor(String color) {
        uint8_t startIndex = 1;
        
        operationMode = CONFIGURABLE_MODE;
        // r
        uint8_t g = HEX(color[startIndex + 0]) * 16 + HEX(color[startIndex + 1]);
        // g
        uint8_t r = HEX(color[startIndex + 2]) * 16 + HEX(color[startIndex + 3]);
        uint8_t b = HEX(color[startIndex + 4]) * 16 + HEX(color[startIndex + 5]);
        
        for (uint16_t i = 0; i < PIXEL_COUNT; i++) {
            s->setPixelColor(i, s->Color(r, g, b));
        }
        
        Particle.publish("Defined new color", color.substring(1));
        
        s->setBrightness(this->brightness);
        s->show();
        return 0;
    }

    // SetColor - set the colors of the pixels in a given range
    // Parameter format: XYYYYYYYYY...
    // X - the range ID: 
    //      0 or "0": sets the pixels between 0 - 32
    //      1 or "1": sets the pixels between 33 - 65
    //      2 or "2": sets the pixels between 66 - 99
    // Y - defined color ID
    int SetColorByID(String colors)
    {
        if (colors.length() < 2) {
            // We need at least  2 byte, an address and color id
            return -1;
        }
        operationMode = CONFIGURABLE_MODE;
        unsigned char id = colors[0];
        switch (id) {
            case 0: 
            case '0': 
                Particle.publish("SetColor from index 0 to 32", colors.substring(1));
                this->SetPixelColorsInRange(0, 32, colors.substring(1));
                break;
            case 1: 
            case '1': 
                Particle.publish("SetColor from index 33 to 65", colors.substring(1));
                this->SetPixelColorsInRange(33, 65, colors.substring(1));
                break;
            case 2: 
            case '2': 
                Particle.publish("SetColor from index 66 to 99", colors.substring(1));
                this->SetPixelColorsInRange(66, 99, colors.substring(1));
                break;
        }
        s->setBrightness(this->brightness);
        s->show();
        return 0;
    }
    
    uint32_t GetColorByID(uint8_t id) 
    {
        uint16_t i = 0;
        for (i = 0; i < index; i++) {
            if (id == idList[i]) {
                return colorList[i];
            }
        }
        // return default color
        return s->Color(0, 0, 0);
    }

    void SetColorByID(uint8_t id, uint32_t color) 
    {
        uint16_t i = 0;
        for (i = 0; i < index; i++) {
            if (id == idList[i]) {
                colorList[i] = color;
                return;
            }
        }
        idList[index] = id;
        colorList[index] = color;
        index++;
    }
    
    void SetPixelColorsInRange(uint16_t startIndex, uint16_t lastIndex, String data)
    {
        uint16_t i = 0;
        uint16_t j = startIndex;
        for (i = 0; i < data.length(); i++) {
            s->setPixelColor(j, GetColorByID(data[i]));
            j++;
        }
    }
};

#endif // Rainbow_h

