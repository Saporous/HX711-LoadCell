#include <HX711.h>
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 6;

HX711 scale;

void setup() {
    Serial.begin(115200);

    Serial.println("MFS Setup");
    Timer1.initialize();
    MFS.initialize(&Timer1);

    Serial.println("HX711 Setup");
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

    scale.set_scale(100469); 
    scale.tare();
}

void loop() {
    doScale();
    doButton();

    delay(100);
}

void doButton() {
    byte btn = MFS.getButton();

    if (btn) {
        byte buttonNumber = btn & B00111111;
        byte buttonAction = btn & B11000000;

        if (buttonNumber == 1) {
            if (buttonAction == BUTTON_PRESSED_IND) {
                scale.tare();
                Serial.println("Scale Tared");
            }
        }
    }
}

void doScale() {
    Serial.print("one reading: ");
    Serial.println(scale.get_units(), 1);

    MFS.write(scale.get_units());
}