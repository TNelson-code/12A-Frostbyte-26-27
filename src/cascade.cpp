#include "main.h"

// Cascade positions in rotations
double cascadeTargets[CASCADE_STATE_COUNT] = {0, 2.5, 5, 7.5, 10};

double revMin = 0;
double revMax = 10;

double cascadeKp = 60;
double cascadeTolerance = 0.05;

double CascadeRevs() {
  return cascadeRot.get_position() / 36000.0; // centidegrees to rotations
}

void CascadeInit() {
  cascadeRot.set_position(0);
}

void CascadeMove(int speed) {
  cascadeLeft.move(speed);
  cascadeRight.move(speed);
}

// Used in auton, waits until the cascade gets there
void CascadeMoveToState(CascadeState state, int timeoutMs) {
  double target = cascadeTargets[state];
  int start = pros::millis();

  while (pros::millis() - start < timeoutMs) {
    double error = target - CascadeRevs();
    if (fabs(error) < cascadeTolerance) break;
    CascadeMove(error * cascadeKp);
    pros::delay(10);
  }

  CascadeMove(0);
}

void CascadeControl() {
  // No sensor plugged in means no idea where the cascade is, so skip the limits
  bool hasSensor = cascadeRot.is_installed();
  double revs = CascadeRevs();

  if (master.get_digital(DIGITAL_R1) && (!hasSensor || revs < revMax)) {
    CascadeMove(127);
  }
  else if (master.get_digital(DIGITAL_R2) && (!hasSensor || revs > revMin)) {
    CascadeMove(-127);
  }
  else {
    CascadeMove(0);
  }
}
