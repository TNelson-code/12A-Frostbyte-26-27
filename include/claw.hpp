#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

// Declare motors, but do NOT construct them here.
inline pros::Motor clawArm(12);

inline ez::Piston claw('B');

void ClawContract(bool ClawState);
void ClawMove(int speed);
void ClawControl();