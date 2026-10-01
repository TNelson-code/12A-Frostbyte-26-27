#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

// Declare motors, but do NOT construct them here.
inline pros::Motor clawArm(12);

inline ez::Piston claw('H');
inline pros::Rotation clawRot(11);

void ClawContract(bool ClawState);
void ClawMove(int speed);
void ClawArmControl();
void ClawControl();
void ClawArmInit();
double _armPos();
