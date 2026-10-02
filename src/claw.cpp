#include "main.h"

// Arm positions in rotation sensor degrees: down, high, low
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {360, 270, 88};

MotorPID armPID = {1.5, 0, 0.5}; // kP, kI, kD
double armTolerance = 20;

double armTarget = 0;
bool armGoingToTarget = false;

double ArmAngle() {
  double angle = clawRot.get_angle() / 100.0;
  if (angle < 50) angle += 360; // sensor rolls over from 360 to 0, this keeps it going past 360
  return angle;
}

// Used in auton, waits until the arm gets there
void ClawArmMoveToState(ClawArmState state, int timeoutMs) {
  double target = clawArmTargets[state];
  int start = pros::millis();
  PIDReset(armPID);

  while (pros::millis() - start < timeoutMs) {
    if (fabs(target - ArmAngle()) < armTolerance) break;
    clawArm.move(PIDCalc(armPID, target, ArmAngle()));
    pros::delay(10);
  }

  clawArm.move(0);
}

void ClawArmControl() {
  if (master.get_digital_new_press(DIGITAL_DOWN)) {
    armTarget = clawArmTargets[CLAW_ARM_DOWN];
    armGoingToTarget = true;
    PIDReset(armPID);
  }
  if (master.get_digital_new_press(DIGITAL_LEFT)) {
    armTarget = clawArmTargets[CLAW_ARM_HIGH];
    armGoingToTarget = true;
    PIDReset(armPID);
  }
  if (master.get_digital_new_press(DIGITAL_UP)) {
    armTarget = clawArmTargets[CLAW_ARM_LOW];
    armGoingToTarget = true;
    PIDReset(armPID);
  }

  if (master.get_digital(DIGITAL_L1)) {
    clawArm.move(60);
    armGoingToTarget = false;
  }
  else if (master.get_digital(DIGITAL_L2)) {
    clawArm.move(-60);
    armGoingToTarget = false;
  }
  else if (armGoingToTarget) {
    if (fabs(armTarget - ArmAngle()) < armTolerance) {
      clawArm.move(0);
      armGoingToTarget = false;
    }
    else {
      clawArm.move(PIDCalc(armPID, armTarget, ArmAngle()));
    }
  }
  else {
    clawArm.move(0); // brake mode is hold, so it stays put
  }
}

void ClawControl() {
  claw.button_toggle(master.get_digital(DIGITAL_X));
}

void ClawContract(bool ClawState) {
  claw.set(ClawState);
}
