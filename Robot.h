//
// Created by pc on 2022-01-20.
//

#ifndef RICOCHETAI_ROBOT_H
#define RICOCHETAI_ROBOT_H

#endif //RICOCHETAI_ROBOT_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Field.h"

#define SMALLTYPE uint8_t

class Robot{
private:
    SMALLTYPE pos = 0;
    SMALLTYPE moveCount = 0;
public:
    void setPos(SMALLTYPE, SMALLTYPE);
    SMALLTYPE getPos(void);
    SMALLTYPE getMoveCount(void);
};

typedef struct moveResult_{
    Field newField;
    Robot newRobot;
} moveResult;

moveResult moveRobot(Robot, Field, SMALLTYPE);