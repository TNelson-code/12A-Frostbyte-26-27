#include "main.h"


void ClawArmMove(int IntakeSpeed) {
    clawArm.move(IntakeSpeed);
}

void ClawArmControl() {
    if (master.get_digital(DIGITAL_L1)) {
        ClawArmMove(90);
    } 

    else if (master.get_digital(DIGITAL_L2)) {
        ClawArmMove(-90);
    }
    else {
        ClawArmMove(0);
    }
}

void ClawControl() {
    claw.button_toggle(master.get_digital(DIGITAL_X));
}

void ClawContract(bool ClawState) {
    claw.set(ClawState);
}