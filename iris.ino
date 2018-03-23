#include "iris.h"

void setup()
{
    SetOperationMode(RAINBOW);
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

void SetOperationMode (OperationMode mode) {
    operationMode = mode;
    switch (operationMode) {
        case RAINBOW:
            client.publish(IRIS_SETMODE_TOPIC, "rainbow");
            break;
        case CONFIGURABLE_MODE:
            client.publish(IRIS_SETMODE_TOPIC, "configurable");
            break;
        default:
            operationMode = RAINBOW;
            client.publish(IRIS_SETMODE_TOPIC, "rainbow");
    }
}

void MQTTSubscribeCallback(char* topic, byte* payload, unsigned int length) {
    if (!strcmp(topic, IRIS_SETCOLOR_TOPIC)) {
        char p[length + 1];
        memcpy(p, payload, length);
        p[length] = '/0';
        String color(p, length);
        colorManager.SetColor (color);
    } else if (!strcmp(topic, IRIS_SET_BRIGHTNESS_TOPIC)) {
        char p[length + 1];
        memcpy(p, payload, length);
        colorManager.SetBrightnessByDirectValue(atoi(p));
    } else if (!strcmp(topic, IRIS_SETMODE_TOPIC)) {
        char p[length + 1];
        memcpy(p, payload, length);
        p[length] = '/0';
        String mode(p, length);
        if (mode == "rainbow") {
            rainbow.SetRainbowMode("");
        }
    }
    delay(1000);
}
