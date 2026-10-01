#include "main.h"

double revolutions = 0;
double revMax = 20;
double revMin = 0;

// Total rotations since init 
double _cascadeRevs() {
  return cascadeRot.get_position() / 36000.0; // centidegrees to rotations
}

void CascadeInit() {
  cascadeRot.set_data_rate(5); // 5 ms per reading
  cascadeRot.set_position(0);
  revolutions = 0;
}

void CascadeMove(int IntakeSpeed) {
    cascadeLeft.move(IntakeSpeed);
    cascadeRight.move(IntakeSpeed);
}

void CascadeControl() {
    revolutions = _cascadeRevs();

    if (master.get_digital(DIGITAL_R1) && revolutions < revMax) {
        CascadeMove(127);
    }
    else if (master.get_digital(DIGITAL_R2) && revolutions > revMin) {
        CascadeMove(-127);
    }
    else {
        CascadeMove(0);
    }
}