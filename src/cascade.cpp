#include "main.h"


void CascadeMove(int IntakeSpeed) {
    cascadeLeft.move(IntakeSpeed);
    cascadeRight.move(IntakeSpeed);
}


void CascadeControl() {

    // If L1 is pressed, spin bottom intake forwards (top intake spins backwards to keep blocks from falling out)
    if (master.get_digital(DIGITAL_R1)) {
        CascadeMove(127);
    } 

    else if (master.get_digital(DIGITAL_R2)) {
        CascadeMove(-127);
    }
    else {
        CascadeMove(0);
    }
}