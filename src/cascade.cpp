#include "main.h"
#include <cmath>
#include <cstdint>

double revolutions = 0;
double revMax = 10;
double revMin = 0;

// Total rotations since init, or NAN if the sensor reading is invalid
double _cascadeRevs() {
  int32_t pos = cascadeRot.get_position(); // centidegrees
  if (pos == PROS_ERR || pos == INT32_MAX || pos == INT32_MIN) {
    return NAN;
  }
  double revs = pos / 36000.0; // centidegrees to rotations
  return std::isfinite(revs) ? revs : NAN;
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
    double reading = _cascadeRevs();

    // Without a valid reading the limits can't be enforced, so allow manual movement with no limits
    bool limited = std::isfinite(reading);
    if (limited) revolutions = reading;

    if (master.get_digital(DIGITAL_R1) && (!limited || revolutions < revMax)) {
        CascadeMove(127);
    }
    else if (master.get_digital(DIGITAL_R2) && (!limited || revolutions > revMin)) {
        CascadeMove(-127);
    }
    else {
        CascadeMove(0);
    }
}