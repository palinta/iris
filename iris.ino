#include "iris.h"

void setup()
{
    operationMode = RAINBOW;
    strip.begin();
    strip.show();
    client.connect(IRIS_MQTT_CLIENT_NAME);
    if (client.isConnected()) {
        client.subscribe(IRIS_SETCOLOR_TOPIC);
        client.subscribe(IRIS_SET_BRIGHTNESS_TOPIC);
    }
}

void loop()
{
    switch (operationMode) {
        case RAINBOW:
            rainbow.MakeRainbow();
            break;
        case CONFIGURABLE_MODE:
            strip.show();
            break;
        default:
            rainbow.MakeRainbow();
            break;
    }
    if (client.isConnected()) {
        client.loop();
    } else {
        client.connect(IRIS_MQTT_CLIENT_NAME);
        if (client.isConnected()) {
            client.subscribe(IRIS_SETCOLOR_TOPIC);
            client.subscribe(IRIS_SET_BRIGHTNESS_TOPIC);
        }
    }
    delay(500);
}

void MQTTSubscribeCallback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = '/0';

    if (!strcmp(topic, IRIS_SETCOLOR_TOPIC)) {
        String color(p, length);
        colorManager.SetColor (color);
    } else if (!strcmp(topic, IRIS_SET_BRIGHTNESS_TOPIC)) {
        Particle.publish("Set Brightness", p);
        colorManager.SetBrightnessByDirectValue(atoi(p));
    }
    delay(1000);
}
