#include "main.h"

// Arm positions in motor degrees. 0 = arm down.
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {0, 505, 810}; // down, high, low


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
  clawArm.move_absolute(clawArmTargets[state], 127);
}

void ClawArmControl() {
  if (master.get_digital(DIGITAL_LEFT)) ClawArmMoveTo(CLAW_ARM_DOWN);
  if (master.get_digital_new_press(DIGITAL_DOWN)) {
    ClawArmMoveTo(CLAW_ARM_HIGH);
    pros::delay(400); 
    clawTilt.set(true);
  }
  if (master.get_digital(DIGITAL_RIGHT))   ClawArmMoveTo(CLAW_ARM_LOW);

  static bool manual = false; // true while L1/L2 is driving the arm
  if (master.get_digital(DIGITAL_L1))      { clawArm.move(70);  manual = true; }
  else if (master.get_digital(DIGITAL_L2)) { clawArm.move(-70); manual = true; }
  else if (manual)                         { clawArm.brake();   manual = false; } // let go: stop and hold
}

void ClawControl() {
  claw.button_toggle(master.get_digital(DIGITAL_Y));
}

void ClawContract(bool ClawState) {
  claw.set(ClawState);
}

void ClawTilt(bool ClawTiltState) {
  clawTilt.set(ClawTiltState);
}

void ClawTiltControl() {
  clawTilt.button_toggle(master.get_digital(DIGITAL_B));
}
