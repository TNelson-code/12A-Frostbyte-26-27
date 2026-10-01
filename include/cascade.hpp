#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

// Declare motors, but do NOT construct them here.
inline pros::Motor cascadeLeft(-7);
inline pros::Motor cascadeRight(19);

inline pros::Rotation cascadeRot(6);

enum CascadeState { CASCADE_STATE_0, CASCADE_STATE_1, CASCADE_STATE_2, CASCADE_STATE_3, CASCADE_STATE_4, CASCADE_STATE_COUNT };

// Revolution target for each state (tune these on the robot)
extern double cascadeTargets[CASCADE_STATE_COUNT];

// Drives the cascade to the state's revolution target. Blocks until it arrives or timeoutMs passes.
// Returns true if the target was reached, false on timeout or invalid sensor reading.
bool CascadeMoveToState(CascadeState state, int timeoutMs = 3000);

void CascadeMove(int speed);
void CascadeControl();
void CascadeInit();
double _cascadeRevs();

extern double revolutions; // Rotations since init
extern double revMin;
extern double revMax;
void ClawArmControl();