#include "main.h"

// Arm positions in motor encoder degrees. 0 = arm down (where the arm starts).
double clawArmTargets[CLAW_ARM_STATE_COUNT] = {0, 270, 810}; // down, high, low

const int CLAW_ARM_SPEED = 80; // rpm for preset moves

// Motor degrees per rotation sensor degree (gear ratio between the motor and the sensor).
// Make negative if the sensor counts the opposite way from the motor.
const double CLAW_ROT_RATIO = -3.0;
const double CLAW_ARM_SETTLE_DEG = 5; // motor degrees from target that counts as "arrived"

static bool clawArmSyncPending = false; // true until the arm reaches its latest preset target

void ClawInit() {
  clawArm.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  clawArm.tare_position(); // start the arm down
  clawRot.reset_position();
}

// Arm position from the rotation sensor, converted to motor degrees.
static double ClawRotMotorDegrees() {
  return clawRot.get_position() / 100.0 * CLAW_ROT_RATIO; // sensor reports centidegrees
}

// Overwrite the motor encoder with the rotation sensor's reading.
static void ClawArmSyncEncoder() {
  clawArm.set_zero_position(ClawRotMotorDegrees());
}

// Arm position in motor degrees (0 = down).
double ClawArmPosition() {
  return clawArm.get_position();
}

static void ClawArmMoveTo(ClawArmState state) {
  clawArm.move_absolute(clawArmTargets[state], CLAW_ARM_SPEED);
  clawArmSyncPending = true;
}

// Sync the encoder once the arm arrives at its target.
static void ClawArmCheckSync() {
  if (!clawArmSyncPending) return;
  if (std::abs(clawArm.get_target_position() - clawArm.get_position()) < CLAW_ARM_SETTLE_DEG) {
    ClawArmSyncEncoder();
    clawArmSyncPending = false;
  }
}

static int clawArmMoveId = 0; // bumped on every auton move so older sync tasks quit

// Used in auton. Starts the move and returns immediately; a background task syncs
// the encoder once the arm arrives (or after timeoutMs). Add a pros::delay after if you need to wait.
void ClawArmMoveToState(ClawArmState state, int timeoutMs) {
  ClawArmMoveTo(state);
  int id = ++clawArmMoveId;
  pros::Task([id, timeoutMs]() {
    int start = pros::millis();
    while (clawArmSyncPending && id == clawArmMoveId && pros::millis() - start < timeoutMs) {
      ClawArmCheckSync();
      pros::delay(10);
    }
    if (clawArmSyncPending && id == clawArmMoveId) { // timed out: sync anyway
      ClawArmSyncEncoder();
      clawArmSyncPending = false;
    }
  });
}

void ClawArmControl() {
  ClawArmCheckSync();

  if (master.get_digital(DIGITAL_DOWN)) ClawArmMoveTo(CLAW_ARM_DOWN);
  if (master.get_digital(DIGITAL_LEFT)) ClawArmMoveTo(CLAW_ARM_HIGH);
  if (master.get_digital(DIGITAL_RIGHT))   ClawArmMoveTo(CLAW_ARM_LOW);

  static bool manual = false; // true while L1/L2 is driving the arm
  if (master.get_digital(DIGITAL_L1))      { clawArm.move(40);  manual = true; clawArmSyncPending = false; }
  else if (master.get_digital(DIGITAL_L2)) { clawArm.move(-40); manual = true; clawArmSyncPending = false; }
  else if (manual)                         { clawArm.brake();   manual = false; } // let go: stop and hold
}

void ClawControl() {
  claw.button_toggle(master.get_digital(DIGITAL_Y));
}

void ClawContract(bool ClawState) {
  claw.set(ClawState);
}
