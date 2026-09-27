#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

// Declare motors, but do NOT construct them here.
inline pros::Motor cascadeLeft(-7);
inline pros::Motor cascadeRight(19);

void CascadeMove(int speed);
void CascadeControl();
void ClawArmControl();