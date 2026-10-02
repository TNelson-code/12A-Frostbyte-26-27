#pragma once

struct MotorPID {
  double kP;
  double kI;
  double kD;
  double integral = 0;
  double lastError = 0;
};

double PIDCalc(MotorPID &pid, double target, double current);
void PIDReset(MotorPID &pid);
