#if ! defined IRIS_H
#define IRIS_H

#include <Particle.h>
#include <neopixel.h>

#include "MQTT.h"
#include "Rainbow.h"
#include "Globals.h"
#include "ColorManager.h"
#include "OperationMode.h"

SYSTEM_MODE(AUTOMATIC);

#define PIXEL_PIN D2
#define PIXEL_COUNT 5
#define PIXEL_TYPE WS2812B

void MQTTSubscribeCallback(char* topic, byte* payload, unsigned int length);

MQTT client("iot.eclipse.org", 1883, MQTTSubscribeCallback);

// Globally defined variables
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
Rainbow rainbow(&strip);
ColorManager colorManager(&strip);

const char * IRIS_SETCOLOR_TOPIC = "palinta/iris/setColor";
const char * IRIS_SETMODE_TOPIC = "palinta/iris/mode";
const char * IRIS_SET_BRIGHTNESS_TOPIC = "palinta/iris/brightness";
const char * IRIS_MQTT_CLIENT_NAME = "palinta-iris-client";

#endif // IRIS_H
