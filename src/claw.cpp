#include "main.h"

// Arm positions in motor degrees. 0 = arm down.
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {0, 90, 272}; // down, high, low

const double CLAW_ARM_RATIO = 3.0; // motor degrees per sensor degree (1:3 gearing)
static double armOffset = 0;       // motor encoder reading when the arm is down

void ClawInit() {
  clawArm.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  armOffset = clawArm.get_position(); // fallback if there's no sensor: down = wherever the arm starts (start it down)
  ClawArmSyncToSensor();
}

// Arm position in motor degrees from down (0 = down).
double ClawArmPosition() {
  return clawArm.get_position() - armOffset;
}

// Recomputes armOffset from the rotation sensor. Does nothing if the sensor has no reading.
// Sensor reads ~360 (or ~0) at down, so arm motor degrees = (sensor degrees - 360) * ratio.
bool ClawArmSyncToSensor() {
  if (!clawRot.is_installed()) return false;
  int32_t raw = clawRot.get_angle();
  if (raw == PROS_ERR) return false;

  double angle = raw / 100.0;   // centidegrees to degrees
  if (angle < 50) angle += 360; // arm rests at down (~0/360), so treat small readings as past 360

  double motorDeg = (angle - 360) * CLAW_ARM_RATIO;
  armOffset = clawArm.get_position() - motorDeg;
  return true;
}

static void ClawArmMoveTo(ClawArmState state) {
  clawArm.move_absolute(clawArmTargets[state] + armOffset, 20);
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
