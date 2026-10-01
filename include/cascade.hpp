#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

// Declare motors, but do NOT construct them here.
inline pros::Motor cascadeLeft(-7);
inline pros::Motor cascadeRight(19);

inline pros::Rotation cascadeRot(20);

void CascadeMove(int speed);
void CascadeControl();
void CascadeInit();
double _cascadeRevs();

extern double revolutions; // Rotations since init
extern double revMin;
extern double revMax;
void ClawArmControl();