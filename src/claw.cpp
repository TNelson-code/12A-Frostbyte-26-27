#include "main.h"

double armTarget = 0;
const double armMin = 0;
const double armMax = 270;
const double armKp = 1; // Tuning

double _armPos() {
  double a = clawRot.get_angle() / 100.0; // centridegrees to degrees
  if (a > 315) a -= 360;
  return a;
}

void ClawArmInit() {
  clawRot.set_data_rate(5); // 5 ms per reading
  armTarget = std::clamp(_armPos(), armMin, armMax);
}

void ClawArmControl() {
  double pos = _armPos();
  bool up = master.get_digital(DIGITAL_L1);
  bool down = master.get_digital(DIGITAL_L2);

  if (master.get_digital_new_press(DIGITAL_DOWN)) armTarget = 0;
  if (master.get_digital_new_press(DIGITAL_LEFT)) armTarget = 90;
  if (master.get_digital_new_press(DIGITAL_UP)) armTarget = 270;

  double out;
  if (up != down) {
    out = up ? 90 : -90; // Up or down moving 90
    armTarget = std::clamp(pos, armMin, armMax); // Readjust pos
  } else {
    out = std::clamp((armTarget - pos) * armKp, -127.0, 127.0);
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