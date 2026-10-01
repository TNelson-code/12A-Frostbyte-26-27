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

// Placeholder targets, must stay within revMin..revMax
double cascadeTargets[CASCADE_STATE_COUNT] = {0, 2.5, 5, 7.5, 10};

bool CascadeMoveToState(CascadeState state, int timeoutMs) {
    if (state < 0 || state >= CASCADE_STATE_COUNT) return false;

    const double tolerance = 0.05; // revolutions
    const double kP = 60;          // motor power per revolution of error
    double target = std::fmin(std::fmax(cascadeTargets[state], revMin), revMax);
    uint32_t start = pros::millis();

    while (pros::millis() - start < (uint32_t)timeoutMs) {
        double reading = _cascadeRevs();
        if (!std::isfinite(reading)) break; // can't close the loop without a sensor

        revolutions = reading;
        double error = target - reading;
        if (std::fabs(error) <= tolerance) {
            CascadeMove(0);
            return true;
        }

        double power = std::fmin(std::fmax(error * kP, -127), 127);
        // Keep a minimum power so the motors don't stall near the target
        if (std::fabs(power) < 25) power = power < 0 ? -25 : 25;
        CascadeMove((int)power);
        pros::delay(10);
    }

    CascadeMove(0);
    return false;
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