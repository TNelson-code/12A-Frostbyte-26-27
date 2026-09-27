#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

// Declare motors, but do NOT construct them here.
inline pros::Motor intake(-20);

void IntakeControl();
void IntakeMove(int speed);