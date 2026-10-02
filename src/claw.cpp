#include "main.h"

// Arm positions in rotation sensor degrees: down, high, low
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {360, 270, 88};

// Motor degrees per arm degree. 1.0 if the motor drives the arm directly,
// otherwise (driven gear teeth / motor gear teeth), e.g. 60/12 = 5.0
double armGearRatio = 1.0;
int armSpeed = 100;        // max rpm for move_absolute
double armTolerance = 5;   // arm degrees, only used by auton to know when it's done

double armTarget = 360;

// Rotation sensor is only read once at startup to find where the arm is. After that everything
// runs off the motor encoder, which never wraps, so the arm always travels 360 -> 270 -> 88 the
// long way round and never takes the short cut through 0 -> 90.
double armOffset = 0; // arm degrees when the motor encoder reads 0

void ClawInit() {
  clawArm.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

  double angle = clawRot.get_angle() / 100.0;
  if (clawRot.get_angle() == PROS_ERR) angle = clawArmTargets[CLAW_ARM_DOWN]; // sensor unplugged, assume down
  if (angle < 50) angle += 360; // arm rests at down (~0/360), so treat small readings as past 360
  armOffset = angle - clawArm.get_position() / armGearRatio;
  armTarget = angle;
}

double ArmAngle() {
  return clawArm.get_position() / armGearRatio + armOffset;
}

void ArmGoTo(double target) {
  clawArm.move_absolute((target - armOffset) * armGearRatio, armSpeed);
}

// Used in auton, waits until the arm gets there
void ClawArmMoveToState(ClawArmState state, int timeoutMs) {
  armTarget = clawArmTargets[state];
  int start = pros::millis();

  while (pros::millis() - start < timeoutMs) {
    ArmGoTo(armTarget);
    if (fabs(armTarget - ArmAngle()) < armTolerance) break;
    pros::delay(10);
  }
}

void ClawArmControl() {
  // get_digital instead of new_press: holding just keeps setting the same target, and nothing else can eat the press
  if (master.get_digital(DIGITAL_DOWN)) armTarget = clawArmTargets[CLAW_ARM_DOWN];
  if (master.get_digital(DIGITAL_LEFT)) armTarget = clawArmTargets[CLAW_ARM_HIGH];
  if (master.get_digital(DIGITAL_UP))   armTarget = clawArmTargets[CLAW_ARM_LOW];

  if (master.get_digital(DIGITAL_L1)) {
    clawArm.move(60);
    armTarget = ArmAngle(); // hold wherever it is when the button is let go
  }
  else if (master.get_digital(DIGITAL_L2)) {
    clawArm.move(-60);
    armTarget = ArmAngle();
  }
  else {
    ArmGoTo(armTarget);
  }
}

void ClawControl() {
  claw.button_toggle(master.get_digital(DIGITAL_X));
}

void ClawContract(bool ClawState) {
  claw.set(ClawState);
}
