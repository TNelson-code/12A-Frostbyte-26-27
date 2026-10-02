#include "main.h"

// Returns motor power from -127 to 127.
// Power gets smaller as current gets closer to target, so it slows down on the way in.
double PIDCalc(MotorPID &pid, double target, double current) {
  double error = target - current;

  pid.integral += error;
  double derivative = error - pid.lastError;
  pid.lastError = error;

  double power = pid.kP * error + pid.kI * pid.integral + pid.kD * derivative;

  if (power > 127) power = 127;
  if (power < -127) power = -127;
  return power;
}

// Call this before starting a new move so old values don't carry over
void PIDReset(MotorPID &pid) {
  pid.integral = 0;
  pid.lastError = 0;
}
