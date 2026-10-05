#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

inline pros::Motor clawArm(12);
inline ez::Piston claw('H');
inline pros::Rotation clawRot(11);

enum ClawArmState { CLAW_ARM_DOWN, CLAW_ARM_HIGH, CLAW_ARM_LOW, CLAW_ARM_STATE_COUNT };

extern double clawArmTargets[CLAW_ARM_STATE_COUNT];

void ClawInit();
bool ClawArmSyncToSensor();
double ClawArmPosition();
void ClawArmMoveToState(ClawArmState state, int timeoutMs = 1000);
void ClawContract(bool ClawState);
void ClawArmControl();
void ClawControl();
