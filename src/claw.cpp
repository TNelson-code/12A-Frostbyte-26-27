#include "main.h"

double armTarget = 0;
const double armMin = 0;
const double armMax = 270;
const double armKp = 1; // Tuning
const double armTolerance = 6; // degrees of slack around a target

double _armPos() {
  int32_t raw = clawRot.get_angle();
  if (raw == PROS_ERR) return NAN; // Sensor unplugged
  double a = raw / 100.0; // centridegrees to degrees
  if (a > 315) a -= 360;
  return a;
}

void ClawArmInit() {
  clawRot.set_data_rate(5); // 5 ms per reading
  double pos = _armPos();
  // get_angle() is absolute and survives power cycles, so just hold wherever the arm is
  armTarget = std::isnan(pos) ? 0 : pos;
}

// Must stay within armMin..armMax
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {0, 90, 270};

bool ClawArmMoveToState(ClawArmState state, int timeoutMs) {
  if (state < 0 || state >= CLAW_ARM_STATE_COUNT) return false;

  const double tolerance = armTolerance;
  double target = std::clamp(clawArmTargets[state], armMin, armMax);
  uint32_t start = pros::millis();

  while (pros::millis() - start < (uint32_t)timeoutMs) {
    double pos = _armPos();
    if (std::isnan(pos)) break; // can't close the loop without a sensor

    double error = target - pos;
    if (std::fabs(error) <= tolerance) {
      armTarget = target; // hold here once control resumes
      clawArm.move(0);
      return true;
    }

    clawArm.move(std::clamp(error * armKp, -127.0, 127.0));
    pros::delay(10);
  }

  clawArm.move(0);
  return false;
}

void ClawArmControl() {
  double pos = _armPos();
  bool up = master.get_digital(DIGITAL_L1);
  bool down = master.get_digital(DIGITAL_L2);

  if (std::isnan(pos)) { // No sensor: manual only, no limits or presets
    clawArm.move(up == down ? 0 : (up ? 60 : -60));
    return;
  }

  if (master.get_digital_new_press(DIGITAL_DOWN)) armTarget = clawArmTargets[CLAW_ARM_DOWN];
  if (master.get_digital_new_press(DIGITAL_LEFT)) armTarget = clawArmTargets[CLAW_ARM_HIGH];
  if (master.get_digital_new_press(DIGITAL_UP)) armTarget = clawArmTargets[CLAW_ARM_LOW];

  double out;
  if (up != down) {
    out = up ? 60 : -60; // Up or down moving 90
    armTarget = std::clamp(pos, armMin, armMax); // Readjust pos
  } else {
    double error = armTarget - pos;
    out = std::fabs(error) <= armTolerance ? 0 : std::clamp(error * armKp, -127.0, 127.0);
  }

  if (pos <= armMin && out < 0) out = 0;
  if (pos >= armMax && out > 0) out = 0;

  clawArm.move(out);
}

void ClawControl() {
    claw.button_toggle(master.get_digital(DIGITAL_X));
}

void ClawContract(bool ClawState) {
    claw.set(ClawState);
}