//
// Created by pc on 2022-01-26.
//

#ifndef RICOCHETAI_MOVEMENTS_H
#define RICOCHETAI_MOVEMENTS_H

#endif //RICOCHETAI_MOVEMENTS_H


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <random>


#include "Field.h"

#define SMALLTYPE uint8_t


/**
 * A class for recording movements for algorithms
 */
class Movements{
public:
    struct robotMovements{
        SMALLTYPE movedRobot = 4; // set it to 4 which is invalid robot id
        SMALLTYPE moveCount = 255; // set it to max to avoid being considered movable
        SMALLTYPE destination;
        Field field;
    };

    robotMovements mvs[3];
    SMALLTYPE curDepth = 0;
    SMALLTYPE totalMoves = 0;
    SMALLTYPE canMove = 0;

    SMALLTYPE getMovedRobot(SMALLTYPE); // getter functions
    SMALLTYPE getMoveCount(SMALLTYPE);
    SMALLTYPE getDestination(SMALLTYPE);
    SMALLTYPE getTotalMoves();
    SMALLTYPE getCanMove();
    SMALLTYPE isUsedRobot(SMALLTYPE);
    SMALLTYPE getDepth();
    SMALLTYPE getVisited(SMALLTYPE);

    Field getField(SMALLTYPE);


    void setMovedRobot(SMALLTYPE); // setter functions
    void setMoveCount(SMALLTYPE);
    void setDestination(SMALLTYPE);
    void addTotalMoves(SMALLTYPE);
    void setCanMove(SMALLTYPE);
    void setField(Field);

    void nextDepth();
    void printMovements(Field);
};
