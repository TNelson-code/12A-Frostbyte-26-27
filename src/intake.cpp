#include "main.h"

void IntakeMove(int speed) {
  intake.move(speed);
}

void IntakeControl() {
  if (master.get_digital(DIGITAL_Y)) {
    intake.move(127);
  }
  else if (master.get_digital(DIGITAL_B)) {
    intake.move(-127);
  }
  else {
    intake.move(0);
  }
}
