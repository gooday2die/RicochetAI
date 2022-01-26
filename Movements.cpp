//
// Created by pc on 2022-01-26.
//

#include "Movements.h"

SMALLTYPE Movements::getMovedRobot(SMALLTYPE depth){
    return mvs[depth].movedRobot;
}
SMALLTYPE Movements::getMoveCount(SMALLTYPE depth){
    return mvs[depth].moveCount;
}
SMALLTYPE Movements::getDestination(SMALLTYPE depth){
    return mvs[depth].destination;
}

SMALLTYPE Movements::getTotalMoves() {
    return totalMoves;
}

SMALLTYPE Movements::getCanMove() {
    return canMove;
}

Field Movements::getField(SMALLTYPE depth){
    return mvs[depth].field;
}

SMALLTYPE Movements::isUsedRobot(SMALLTYPE robotIndex){
    for(SMALLTYPE i = 0 ; i < 3 ; i++){
        if(mvs[i].movedRobot == robotIndex) return 1;
    }
    return 0;
}

SMALLTYPE Movements::getDepth() {
    return curDepth;
}

SMALLTYPE Movements::getVisited(SMALLTYPE pos){
    for(SMALLTYPE i = 0 ; i < 3 ; i++){
        if(mvs[i].destination == pos) return 1;
    }
    return 0;
}

void Movements::setMovedRobot(SMALLTYPE robotIndex){
    mvs[curDepth].movedRobot = robotIndex;
}

void Movements::setMoveCount(SMALLTYPE moveCount){
    mvs[curDepth].moveCount = moveCount;
}
void Movements::setDestination(SMALLTYPE destination){
    mvs[curDepth].destination = destination;
}
void Movements::setField(Field newField){
    mvs[curDepth].field = newField;
}

void Movements::addTotalMoves(SMALLTYPE toAdd){
    totalMoves += toAdd;
}

void Movements::setCanMove(SMALLTYPE can){
    canMove = can;
}

void Movements::nextDepth() {
    curDepth++;
}

/**
 * A member function for class movements to print out how the movements would be
 * @param CurField current field
 */
void Movements::printMovements(Field CurField) {
    for(SMALLTYPE i = 0 ; i < curDepth ; i++){
        printf("%d) Move Robot : %d to %x. Distance : %d\n", i, mvs[i].movedRobot, mvs[i].destination, mvs[i].moveCount);
        /**
        if(i > 0)
            findPathFromTo(mvs[i + 1].field, CurField.robotArray[mvs[i].movedRobot], mvs[i].destination);
        else
            findPathFromTo(CurField, CurField.robotArray[mvs[i].movedRobot], mvs[i].destination);
        */
    }
    //printf("Total Move Count : %d\n", totalMoves);
}