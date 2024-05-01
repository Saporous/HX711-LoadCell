#include <HX711.h>
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 6;

HX711 scale;

double highest_num = 0;
double curr_num = 0;
bool f_record = false;
byte btn;

void setup() {
    Serial.begin(115200);

    Serial.println("MFS Setup");
    Timer1.initialize();
    MFS.initialize(&Timer1);

    Serial.println("HX711 Setup");
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

    scale.set_offset(19648);
    scale.set_scale(219.754669);
    scale.tare();
    scale.set_average_mode();
}

void loop() {
    doButton();
    doScale();
    doLogic();

    delay(100);
}

void doButton() {
    btn = MFS.getButton();
}

void doScale() {
    Serial.print("Weight: ");
    Serial.print(scale.get_units());
    Serial.print(" grams or ");

    curr_num = scale.get_units();
    curr_num = curr_num / 453.6;
    Serial.print(curr_num);
    Serial.println(" pounds");
}

void doLogic() {
    if (btn) {
        byte buttonNumber = btn & B00111111;
        byte buttonAction = btn & B11000000;

        if (buttonAction == BUTTON_PRESSED_IND) {
            if (buttonNumber == 1) {
                    scale.tare();
                    Serial.println("Scale Tared");
            } else if (buttonNumber == 2) {
                // Record Mode
                if (!f_record) {
                    f_record = true;
                    MFS.writeLeds(LED_1, ON);
                    MFS.blinkLeds(LED_1, ON);
                }
            } else if (buttonNumber == 3) {
                if (f_record) {
                    f_record = false;
                    highest_num = 0;
                    MFS.blinkLeds(LED_1, OFF);
                    MFS.writeLeds(LED_1, OFF);
                }
            }
        }
    }

    if (f_record) {
        if (curr_num > highest_num) {
            highest_num = curr_num;
        }
        Serial.print("Highest Weight: ");
        Serial.println(scale.get_units());
        MFS.write(highest_num, 2);
    } else {
        MFS.write(curr_num, 2);
    }
}
