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
#include "Movements.h"

#define SMALLTYPE uint8_t

/**
 * A class for neighbor algorithm
 */
class neighborAlgorithm{
public:
    SMALLTYPE spPos;
    SMALLTYPE epPos;
    Field curField;
    neighborAlgorithm(){
        curField.setRobot(0, 0x93);
        curField.setRobot(1, 0xD9);
        //curField.setRobot(1, 0x45);
        curField.setRobot(2, 0x15);

        curField.printField();

        printf("Running Algorithm\n");
        epPos = 0xAB ;
        spPos = 0x10;
    }
    void findPath();
    Movements findWay(SMALLTYPE, SMALLTYPE, SMALLTYPE, SMALLTYPE, SMALLTYPE);
};



void runAlgorithm(neighborAlgorithm*, SMALLTYPE, SMALLTYPE);