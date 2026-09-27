#include "main.h"


void IntakeMove(int IntakeSpeed) {
    intake.move(IntakeSpeed);
}

void IntakeControl() {
    // If A is pressed, spin intake forwards
    if (master.get_digital(DIGITAL_A)) {
        IntakeMove(127);
    } 

    // If B is pressed, spin intake backwards
    else if (master.get_digital(DIGITAL_B)) {
        IntakeMove(-127);
    }

    // If no button is pressed, stop intake from spinning
    else {
        IntakeMove(0);
    }
}

