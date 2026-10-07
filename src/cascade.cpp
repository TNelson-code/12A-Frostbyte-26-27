#include "main.h"

// Cascade positions in rotation sensor rotations. 0 = lift down.
double cascadeTargets[CASCADE_STATE_COUNT] = {0, 2.5, 5, 7.5, 10};

double cascadeRatio = 360; // motor degrees per sensor rotation (360 if the sensor turns with the motor)
int cascadeSpeed = 200;    // max rpm for move_absolute

void CascadeInit() {
  cascadeLeft.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  cascadeRight.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  cascadeLeft.tare_position(); // start the lift down
  cascadeRight.tare_position();
  cascadeRot.reset_position();
}

// Resets the motor encoders to match the rotation sensor. Does nothing if the sensor has no reading.
bool CascadeSyncToSensor() {
  if (!cascadeRot.is_installed()) return false;
  int32_t raw = cascadeRot.get_position();
  if (raw == PROS_ERR) return false;

  double motorDeg = raw / 36000.0 * cascadeRatio; // centidegrees to rotations to motor degrees
  cascadeLeft.set_zero_position(cascadeLeft.get_position() - motorDeg); // makes get_position() read motorDeg
  cascadeRight.set_zero_position(cascadeRight.get_position() - motorDeg);
  return true;
}

double CascadeRevs() {
  return cascadeLeft.get_position() / cascadeRatio;
}

void CascadeMove(int speed) {
  cascadeLeft.move(speed);
  cascadeRight.move(speed);
}

void CascadeMoveRelative(int degrees, int speed) {
  cascadeLeft.move_relative(degrees, speed);
  cascadeRight.move_relative(degrees, speed);
}

// Used in auton
void CascadeMoveToState(CascadeState state, int timeoutMs) {
  cascadeLeft.move_absolute(cascadeTargets[state] * cascadeRatio, cascadeSpeed);
  cascadeRight.move_absolute(cascadeTargets[state] * cascadeRatio, cascadeSpeed);
  pros::delay(timeoutMs);
}

void CascadeControl() {
  if (master.get_digital(DIGITAL_R1))      CascadeMove(127);
  else if (master.get_digital(DIGITAL_R2)) CascadeMove(-127);
  else                                     CascadeMove(0);
}
