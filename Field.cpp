//
// Created by pc on 2022-01-19.
//

#include "Field.h"

/**
 * A member function that gets tile information in the field
 * @param x the x pos to get info from
 * @param y the y pos to get info from
 * @return returns the Tile information
 */
SMALLTYPE Field::getTileInfo(SMALLTYPE x, SMALLTYPE y) {
    SMALLTYPE returnValue = fieldArray[8 * y + (x / 2)]; // index this one
    returnValue = returnValue >> (4 * ((x % 2)  == 0)); // if x is even, it is first tile. Else, it is second tile.
    return returnValue & 0x0F;
}

/**
 * A member function that sets tile information in the field
 * @param x the x pos to set tile info
 * @param y the y pos to set tile info
 * @param value the value to set tile info
 */
void Field::setTileInfo(SMALLTYPE x, SMALLTYPE y, SMALLTYPE value) {
    SMALLTYPE newValue;
    newValue = ((value & 0x0F) << (4 * ((x % 2) == 0)));
    fieldArray[8 * y + (x / 2)] = ((fieldArray[8 * y + (x / 2)] & (0x0F << 4 * (x % 2 == 1)) | newValue));
}

/**
 * A member function that prints out the field.
 */
void Field::printField(){
    for(SMALLTYPE i = 0 ; i < 16 ; i++){
        for(SMALLTYPE j = 0 ; j < 16 ; j++)
            printf("%2u ", getTileInfo(j, i));
        printf("\n");
    }
}

/**
 * A member function that sets the whole game field.
 * The field is included in /pic directory of the project.
 */
void Field::setField(){
    setTileInfo(1, 0, 0b1110);
    setTileInfo(2, 0, 0b1101);
    setTileInfo(4, 0, 0b1011);
    setTileInfo(8, 0, 0b1110);
    setTileInfo(9, 0, 0b1101);
    setTileInfo(11, 0, 0b1011);

    setTileInfo(4, 1, 0b0110);
    setTileInfo(5, 1, 0b1101);
    setTileInfo(10, 1, 0b1110);
    setTileInfo(11, 1, 0b0101);
    setTileInfo(15, 1, 0b1011);

    setTileInfo(14, 2, 0b1011);
    setTileInfo(15, 2, 0b0111);

    setTileInfo(0, 3, 0b1110);
    setTileInfo(1, 3, 0b1001);
    setTileInfo(14, 3, 0b0110);
    setTileInfo(15, 3, 0b1101);

    setTileInfo(1, 4, 0b0111);
    setTileInfo(5, 4, 0b1011);
    setTileInfo(9, 4, 0b1010);
    setTileInfo(10, 4, 0b1101);

    setTileInfo(0, 5, 0b1011);
    setTileInfo(4, 5, 0b1110);
    setTileInfo(5, 5, 0b0101);
    setTileInfo(9, 5, 0b0111);

    setTileInfo(0, 6, 0b0111);
    setTileInfo(3, 6, 0b1010);
    setTileInfo(4, 6, 0b1101);
    setTileInfo(7, 6, 0b1011);
    setTileInfo(8, 6, 0b1011);
    setTileInfo(11, 6, 0b1110);
    setTileInfo(12, 6, 0b1001);

    setTileInfo(2, 7, 0b1011);
    setTileInfo(3, 7, 0b0111);
    setTileInfo(6, 7, 0b1110);
    setTileInfo(7, 7, 0b0000);
    setTileInfo(8, 7, 0b0000);
    setTileInfo(9, 7, 0b1101);
    setTileInfo(12, 7, 0b0111);

    setTileInfo(2, 8, 0b0110);
    setTileInfo(3, 8, 0b1101);
    setTileInfo(6, 8, 0b1110);
    setTileInfo(7, 8, 0b0000);
    setTileInfo(8, 8, 0b0000);
    setTileInfo(9, 8, 0b1101);

    setTileInfo(4, 9, 0b1110);
    setTileInfo(5, 9, 0b1001);
    setTileInfo(7, 9, 0b0111);
    setTileInfo(8, 9, 0b0111);
    setTileInfo(11, 9, 0b1011);

    setTileInfo(1, 10, 0b1010);
    setTileInfo(2, 10, 0b1101);
    setTileInfo(5, 10, 0b0111);
    setTileInfo(10, 10, 0b1110);
    setTileInfo(11, 10, 0b0101);

    setTileInfo(1, 11, 0b0111);
    setTileInfo(4, 11, 0b1011);
    setTileInfo(10, 11, 0b1011);
    setTileInfo(12, 11, 0b1110);
    setTileInfo(13, 11, 0b1001);
    setTileInfo(15, 11, 0b1011);

    setTileInfo(0, 12, 0b1011);
    setTileInfo(4, 12, 0b0110);
    setTileInfo(5, 12, 0b1101);
    setTileInfo(10, 12, 0b0110);
    setTileInfo(11, 12, 0b1101);
    setTileInfo(13, 12, 0b0111);
    setTileInfo(15, 12, 0b0111);

    setTileInfo(0, 13, 0b0111);
    setTileInfo(6, 13, 0b1011);
    setTileInfo(12, 13, 0b1010);
    setTileInfo(13, 13, 0b1101);

    setTileInfo(5, 14, 0b1110);
    setTileInfo(6, 14, 0b0101);
    setTileInfo(12, 14, 0b0111);

    setTileInfo(3, 15, 0b1110);
    setTileInfo(4, 15, 0b1101);
    setTileInfo(9, 15, 0b1110);
    setTileInfo(10, 15, 0b0101);
}