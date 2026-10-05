#include "main.h"

// Arm positions in motor degrees. 0 = arm down.
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {0, -90, -272}; // down, high, low

void ClawInit() {
  clawArm.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  clawArm.tare_position(); // fallback if there's no sensor: 0 = wherever the arm starts (start it down)
  ClawArmSyncToSensor();
}

// Resets the motor encoder to match the rotation sensor. Does nothing if the sensor has no reading.
// Sensor reads ~360 (or ~0) at down, so motor degrees = sensor degrees - 360.
bool ClawArmSyncToSensor() {
  if (!clawRot.is_installed()) return false;
  int32_t raw = clawRot.get_angle();
  if (raw == PROS_ERR) return false;

  double angle = raw / 100.0;   // centidegrees to degrees
  if (angle < 50) angle += 360; // arm rests at down (~0/360), so treat small readings as past 360

  double motorDeg = angle - 360;
  clawArm.set_zero_position(clawArm.get_position() - motorDeg); // makes get_position() read motorDeg
  return true;
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
