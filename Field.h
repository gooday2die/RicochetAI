//
// Created by pc on 2022-01-19.
//

#ifndef RICOCHETAI_FIELD_H
#define RICOCHETAI_FIELD_H

#endif //RICOCHETAI_FIELD_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SMALLTYPE uint8_t

/**
 * Each tile will be represented as a uint8_t with following format.
 * 0000 0000
 * Each 4 bits will represent a single tile.
 * And each bits will represent UDLR movements.
 * EX) 0x01 = 1st tile can't move anywhere.
 * EX) 0x01 = 2nd tile can only move right.
 * Also, we are going to store these 256 tiles into a single array called fieldArray.
 */
class Field{
private:
    SMALLTYPE* fieldArray;
public:
    Field(){
        fieldArray = (SMALLTYPE*) malloc(sizeof (SMALLTYPE) * 128);
        for (SMALLTYPE i = 0 ; i < 128 ; i++){
            fieldArray[i] = 0xFF;
        }
    }
    SMALLTYPE getTileInfo(SMALLTYPE, SMALLTYPE);
    void setTileInfo(SMALLTYPE, SMALLTYPE, SMALLTYPE);
    void printField();
    void setField();
};

SMALLTYPE* getDistanceFrom(Field, SMALLTYPE);
void getDistance(Field, SMALLTYPE, SMALLTYPE, SMALLTYPE*, SMALLTYPE);
void goDirection(Field, SMALLTYPE, SMALLTYPE, SMALLTYPE*, SMALLTYPE, SMALLTYPE);
