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

typedef struct movementResult_{
    SMALLTYPE robotOrder[3];
    SMALLTYPE robotDestination[3];
    SMALLTYPE movementCount[3];
    Field field[3];
}movementResult;

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
        epPos = 0x25;
        spPos = 0x10;
    }
    void findPath();
    void findNeighborPath(SMALLTYPE);
    SMALLTYPE placeNeighbors(SMALLTYPE);
    movementResult findWay(SMALLTYPE, SMALLTYPE, SMALLTYPE);
};

void runAlgorithm(Algorithm*, SMALLTYPE, SMALLTYPE);