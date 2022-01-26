//
// Created by pc on 2022-01-24.
//

#ifndef RICOCHETAI_ALGORITHM_H
#define RICOCHETAI_ALGORITHM_H

#endif //RICOCHETAI_ALGORITHM_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <random>


#include "Field.h"

#define SMALLTYPE uint8_t



/**
 * An algorithm class for polymorphism
 */
class Algorithm{
public:
    SMALLTYPE spPos;
    SMALLTYPE epPos;
    Field curField;
    Field resultField;
    Algorithm(){
        curField = Field();
        resultField = curField;
    };
    virtual void findPath(){};
};

class randomAlgorithm : public Algorithm{
public:
    SMALLTYPE spPos;
    SMALLTYPE epPos;
    randomAlgorithm() : Algorithm(){}
    void findPath() override;
    void generateFieldDFS(Field, SMALLTYPE, SMALLTYPE);
};

class Movements{
public:
    struct robotMovements{
        SMALLTYPE movedRobot = 4;
        SMALLTYPE moveCount = 255;
        SMALLTYPE destination;
        Field field;
    };

    robotMovements mvs[3];
    SMALLTYPE curDepth = 0;
    SMALLTYPE totalMoves = 0;
    SMALLTYPE canMove = 0;

    SMALLTYPE getMovedRobot(SMALLTYPE);
    SMALLTYPE getMoveCount(SMALLTYPE);
    SMALLTYPE getDestination(SMALLTYPE);
    SMALLTYPE getTotalMoves();
    SMALLTYPE getCanMove();
    SMALLTYPE isUsedRobot(SMALLTYPE);
    SMALLTYPE getDepth();
    SMALLTYPE getVisited(SMALLTYPE);

    Field getField(SMALLTYPE);


    void setMovedRobot(SMALLTYPE);
    void setMoveCount(SMALLTYPE);
    void setDestination(SMALLTYPE);
    void addTotalMoves(SMALLTYPE);
    void setCanMove(SMALLTYPE);
    void setField(Field);

    void nextDepth();
    void printMovements(Field);
};

class Visited{
private:
    SMALLTYPE* visited;
public:
    Visited(){
        visited = (SMALLTYPE*) malloc((sizeof(SMALLTYPE)) * 256);
        for(uint16_t i = 0 ; i < 256 ; i++) visited[i] = 0;
    }
    void setVisited(SMALLTYPE);
    SMALLTYPE getVisited(SMALLTYPE);
};

class neighborAlgorithm{
public:
    SMALLTYPE spPos;
    SMALLTYPE epPos;
    Field curField;
    neighborAlgorithm(){
        curField.setRobot(0, 0xB1);
        curField.setRobot(1, 0x81);
        //curField.setRobot(1, 0x45);
        curField.setRobot(2, 0xE4);

        curField.printField();

        printf("Running Algorithm\n");
        epPos = 0x25 ;
        spPos = 0x10;
    }
    void findPath();
    void findNeighborPath(SMALLTYPE);
    SMALLTYPE placeNeighbors(SMALLTYPE);
    Movements findWay(SMALLTYPE, SMALLTYPE, SMALLTYPE, Visited, SMALLTYPE, SMALLTYPE);
};



void runAlgorithm(Algorithm*, SMALLTYPE, SMALLTYPE);