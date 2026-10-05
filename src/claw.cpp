#include "main.h"

// Arm positions in motor degrees. 0 = wherever the arm is when the program starts (start it down).
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {0, -90, -272}; // down, high, low

void ClawInit() {
  clawArm.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  clawArm.tare_position();
}

// Used in auton
void ClawArmMoveToState(ClawArmState state, int timeoutMs) {
  clawArm.move_absolute(clawArmTargets[state], 20);
  pros::delay(timeoutMs);
}

void ClawArmControl() {
  if (master.get_digital(DIGITAL_DOWN)) clawArm.move_absolute(clawArmTargets[CLAW_ARM_DOWN], 20);
  if (master.get_digital(DIGITAL_LEFT)) clawArm.move_absolute(clawArmTargets[CLAW_ARM_HIGH], 20);
  if (master.get_digital(DIGITAL_UP))   clawArm.move_absolute(clawArmTargets[CLAW_ARM_LOW], 20);

  static bool manual = false; // true while L1/L2 is driving the arm
  if (master.get_digital(DIGITAL_L1))      { clawArm.move(60);  manual = true; }
  else if (master.get_digital(DIGITAL_L2)) { clawArm.move(-60); manual = true; }
  else if (manual)                         { clawArm.brake();   manual = false; } // let go: stop and hold
}

void ClawControl() {
  claw.button_toggle(master.get_digital(DIGITAL_X));
}

void ClawContract(bool ClawState) {
  claw.set(ClawState);
}
