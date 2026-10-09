#include "main.h"

// Arm positions in motor degrees. 0 = arm down.
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {0, 270, 810}; // down, high, low

const int CLAW_ARM_SPEED = 80; // rpm for preset moves

void ClawInit() {
  clawArm.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  clawArm.tare_position(); // start the arm down
}

// Arm position in motor degrees (0 = down).
double ClawArmPosition() {
  return clawArm.get_position();
}

// Starts the move and returns immediately. Add a pros::delay after if you need to wait.
void ClawArmMoveTo(ClawArmState state) {
  clawArm.move_absolute(clawArmTargets[state], CLAW_ARM_SPEED);
}

void ClawArmControl() {
  if (master.get_digital(DIGITAL_DOWN)) ClawArmMoveTo(CLAW_ARM_DOWN);
  if (master.get_digital(DIGITAL_LEFT)) ClawArmMoveTo(CLAW_ARM_HIGH);
  if (master.get_digital(DIGITAL_RIGHT))   ClawArmMoveTo(CLAW_ARM_LOW);

  static bool manual = false; // true while L1/L2 is driving the arm
  if (master.get_digital(DIGITAL_L1))      { clawArm.move(40);  manual = true; }
  else if (master.get_digital(DIGITAL_L2)) { clawArm.move(-40); manual = true; }
  else if (manual)                         { clawArm.brake();   manual = false; } // let go: stop and hold
}

void ClawControl() {
  claw.button_toggle(master.get_digital(DIGITAL_Y));
}

void ClawContract(bool ClawState) {
  claw.set(ClawState);
}
