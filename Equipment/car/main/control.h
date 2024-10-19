#ifndef CONTROL_H
#define CONTROL_H

void initControl();
void moveForward();
void setSpeed(int newSpeed);
int getSpeed();
void moveBackward();
void turnLeft();
void turnRight();
void correctRight();
void correctLeft();
void shiftRight();
void shiftLeft();
void stop();

#endif
