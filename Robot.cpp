//
// Created by pc on 2022-01-20.
//

#include "Robot.h"

SMALLTYPE Robot::getPos(void) {
    return pos;
}

void Robot::setPos(SMALLTYPE x, SMALLTYPE y){
    pos = ((x << 4) & 0xF0) | (y & (0x0F));
}

SMALLTYPE Robot::getMoveCount(void) {
    return moveCount;
}

/**
 * A function that moves a single robot into a specific direction and returns the field, robot in moveResult variable.
 * @param robot the robot to move
 * @param curField the field to move robot in
 * @param direction the direction to move robot. U : 1 / D : 2 / L : 3 / R : 4
 * @return returns the field and the robot's position in moveResult variable
 */
moveResult moveRobot(Robot robot, Field curField, SMALLTYPE direction){
    SMALLTYPE robotPos = robot.getPos();
    SMALLTYPE moveCount = 0;

    moveResult result;
    result.newField = curField;
    result.newRobot = robot;
    return result;
}