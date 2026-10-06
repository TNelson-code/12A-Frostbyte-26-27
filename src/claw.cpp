#include "main.h"

// Arm positions in motor encoder degrees. 0 = arm down (where the arm starts).
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {0, 255, 793}; // down, high, low

const int CLAW_ARM_SPEED = 40; // rpm for preset moves

void ClawInit() {
  clawArm.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  clawArm.tare_position(); // start the arm down
}

// Arm position in motor degrees (0 = down).
double ClawArmPosition() {
  return clawArm.get_position();
}

static void ClawArmMoveTo(ClawArmState state) {
  clawArm.move_absolute(clawArmTargets[state], CLAW_ARM_SPEED);
}

// Used in auton
void ClawArmMoveToState(ClawArmState state, int timeoutMs) {
  ClawArmMoveTo(state);
  pros::delay(timeoutMs);
}

void ClawArmControl() {
  if (master.get_digital(DIGITAL_DOWN)) ClawArmMoveTo(CLAW_ARM_DOWN);
  if (master.get_digital(DIGITAL_LEFT)) ClawArmMoveTo(CLAW_ARM_HIGH);
  if (master.get_digital(DIGITAL_UP))   ClawArmMoveTo(CLAW_ARM_LOW);

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
