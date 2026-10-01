#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

// Declare motors, but do NOT construct them here.
inline pros::Motor clawArm(12);

inline ez::Piston claw('H');
inline pros::Rotation clawRot(11);

enum ClawArmState { CLAW_ARM_DOWN, CLAW_ARM_HIGH, CLAW_ARM_LOW, CLAW_ARM_STATE_COUNT };

// Angle target in degrees for each state (tune these on the robot)
extern double clawArmTargets[CLAW_ARM_STATE_COUNT];

// Drives the arm to the state's angle target. Blocks until it arrives or timeoutMs passes.
// Returns true if the target was reached, false on timeout or invalid sensor reading.
bool ClawArmMoveToState(ClawArmState state, int timeoutMs = 3000);

void ClawContract(bool ClawState);
void ClawMove(int speed);
void ClawArmControl();
void ClawControl();
void ClawArmInit();
double _armPos();
