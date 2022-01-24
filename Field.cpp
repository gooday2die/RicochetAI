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
 * A member function that moves a specific robot into a direction
 * Directions:
 * 0x08 -> UP
 * 0x04 -> DN
 * 0x02 -> LEFT
 * 0x01 -> RIGHT
 * @param robotIndex the robot index to move
 * @param direction the direction.
 * @return returns field after robot has been moved;
 */
Field Field::moveRobot(SMALLTYPE robotIndex, SMALLTYPE direction) {
    //printf("MOVING ROBOT %d into %u direction\n", robotIndex, direction);
    SMALLTYPE curPOS = robotArray[robotIndex];
    Field newField; // An empty new Field.
    while(((getTileInfo((curPOS >> 4) & 0x0F, curPOS & 0x0F) & direction) == direction) || (robotArray[robotIndex] == curPOS)){
        // keep moving towards that direction when robot can.
        switch (direction) {
            case 0x08: // go up
                curPOS = ((curPOS & 0xF0) | (0x0F & ((curPOS & 0x0F) - 1)));
                break;
            case 0x04: //  go down
                curPOS = ((curPOS & 0xF0) | (0x0F & ((curPOS & 0x0F) + 1)));
                break;
            case 0x02: // go left
                curPOS = (((((curPOS >> 4) & 0x0F) - 1) << 4) & 0xF0) | (curPOS & 0x0F);
                break;
            case 0x01: // go right
                curPOS = (((((curPOS >> 4) & 0x0F) + 1) << 4) & 0xF0) | (curPOS & 0x0F);
                break;
        }
    }
    //printf("ROBOT %d : CANNOT MOVE ANYMORE , CURPOS : (%u, %u)\n", robotIndex, (curPOS >> 4) & 0x0F, curPOS & 0x0F);
    newField.setRobot(robotIndex, curPOS); // set the robot at that position using setRobot

    return newField;
}

Field Field::simulateRobotPos(SMALLTYPE pos){
    Field newField = Field();
    for(SMALLTYPE i = 0 ; i < 3 ; i++){
        newField.fieldArray[i] = this->fieldArray[i];
        newField.setRobot(i, this->robotArray[i]);
    }

    //newField.setTileInfo((pos >> 4) & 0x0F, pos & 0x0F, 0x00); // set that tile not movable.

    if ((pos & 0x0F) < 15)  // check boundary and disable down if possible
        newField.setTileInfo((pos >> 4) & 0x0F, (pos & 0x0F) + 1,
                    this->getTileInfo((pos >> 4) & 0x0F, (pos & 0x0F) + 1) & 0x07);

    if ((pos & 0x0F) >= 1)  // check boundary and disable up if possible
        newField.setTileInfo((pos >> 4) & 0x0F, (pos & 0x0F) - 1,
                    this->getTileInfo((pos >> 4) & 0x0F, (pos & 0x0F) - 1) & 0x0B);

    if (((pos >> 4) & 0x0F) >= 1)  // check boundary and disable right if possible
        newField.setTileInfo(((pos >> 4) & 0x0F) - 1, pos & 0x0F,
                    this->getTileInfo(((pos >> 4) & 0x0F) - 1, pos & 0x0F) & 0x0E);

    if (((pos >> 4) & 0x0F) < 15)  // check boundary and disable left if possible
        newField.setTileInfo(((pos >> 4) & 0x0F) + 1, pos & 0x0F,
                    this->getTileInfo(((pos >> 4) & 0x0F) + 1, pos & 0x0F) & 0x0D);

    return newField;
}

/**
 * A member function that sets a robot's position at pos
 * @param robotIndex the robot index to set pos
 * @param pos the position to set robot at
 */
void Field::setRobot(SMALLTYPE robotIndex, SMALLTYPE pos){
    //setTileInfo((pos >> 4) & 0x0F, pos & 0x0F, 0x00); // set that tile not movable.
    robotArray[robotIndex] = pos;
    if ((pos & 0x0F) < 15)  // check boundary and disable down if possible
        setTileInfo((pos >> 4) & 0x0F, (pos & 0x0F) + 1,
                             getTileInfo((pos >> 4) & 0x0F, (pos & 0x0F) + 1) & 0x07);

    if ((pos & 0x0F) >= 1)  // check boundary and disable up if possible
        setTileInfo((pos >> 4) & 0x0F, (pos & 0x0F) - 1,
                             getTileInfo((pos >> 4) & 0x0F, (pos & 0x0F) - 1) & 0x0B);

    if (((pos >> 4) & 0x0F) >= 1)  // check boundary and disable right if possible
        setTileInfo(((pos >> 4) & 0x0F) - 1, pos & 0x0F,
                             getTileInfo(((pos >> 4) & 0x0F) - 1, pos & 0x0F) & 0x0E);

    if (((pos >> 4) & 0x0F) < 15)  // check boundary and disable left if possible
        setTileInfo(((pos >> 4) & 0x0F) + 1, pos & 0x0F,
                             getTileInfo(((pos >> 4) & 0x0F) + 1, pos & 0x0F) & 0x0D);
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

    for(SMALLTYPE i = 0 ; i < 16 ; i++){
        setTileInfo(i, 0, (getTileInfo(i, 0) & 0x07)); // disable UP for y = 0
        setTileInfo(i, 15, (getTileInfo(i, 15) & 0x0B)); // disable DN for y = 15
        setTileInfo(0, i, (getTileInfo(0, i) & 0x0D)); // disable LEFT for x = 0
        setTileInfo(15, i, (getTileInfo(15, i) & 0xE)); // disable RIGHT for y = 15
    }
}

/**
 * A function that finds route from starting point to end point.
 * Used too much bitwise operation so it's kind of messy
 * @param curField current field
 * @param SPpos the starting position coordinate
 * @param EPpos the ending position coordinate
 * @param isRobot if this is is for robot finding ways
 */
void findPathFromTo(Field curField, SMALLTYPE SPpos, SMALLTYPE EPpos, bool isRobot){
    SMALLTYPE* distanceArr;
    distanceArr = getDistanceFrom(curField, SPpos, isRobot);
    printf("SP : (%d, %d) --> EP : (%d, %d)\n", (SPpos >> 4) & 0x0F, (SPpos & 0x0F), (EPpos >> 4) & 0x0F, (EPpos & 0x0F));
    if(distanceArr[(EPpos & 0x0F) * 16 + ((EPpos >> 4) & 0x0F)] != 255){
        printf("Can reach path. Distance : %d\n", distanceArr[(EPpos & 0x0F) * 16 + ((EPpos >> 4) & 0x0F)]);
        SMALLTYPE curPOS = EPpos;
        SMALLTYPE curDistance = distanceArr[(EPpos & 0x0F) * 16 + ((EPpos >> 4) & 0x0F)];

        while (distanceArr[(curPOS & 0x0F) * 16 + ((curPOS >> 4) & 0x0F)] != 0){
            nextIter: // used a very bad expression, goto expression.
            if ((curField.getTileInfo(((curPOS >> 4) & 0x0F), curPOS & 0x0F) & 0x08) == 0x08) // go up if you can
                for (SMALLTYPE i = (curPOS & 0x0F) ; i >= 0 ; i--) {
                    if (((curField.getTileInfo((curPOS >> 4) & 0x0F, i) & 0x08) == 0) || (distanceArr[i * 16 + ((curPOS >> 4) & 0x0F)] == (curDistance - 1))){
                        if (distanceArr[i * 16 + ((curPOS >> 4) & 0x0F)] == (curDistance - 1)) {
                            curPOS = ((curPOS & 0xF0) | (i & 0x0F));
                            curDistance--;
                            printf("CUR POS : (%d, %d) , Distance : %d\n", ((curPOS >> 4) & 0x0F), curPOS & 0x0F,
                                   distanceArr[(curPOS & 0x0F) * 16 + ((curPOS >> 4) & 0x0F)]);
                            goto nextIter;
                        }
                        else break;
                    }
                }
            if ((curField.getTileInfo(((curPOS >> 4) & 0x0F), curPOS & 0x0F) & 0x04) == 0x04) // go dn if you can
                for (SMALLTYPE i = (curPOS & 0x0F) ; i < 16 ; i++) {
                    if (((curField.getTileInfo((curPOS >> 4) & 0x0F, i) & 0x04) == 0) || (distanceArr[i * 16 + ((curPOS >> 4) & 0x0F)] == (curDistance - 1))){
                        if (distanceArr[i * 16 + ((curPOS >> 4) & 0x0F)] == (curDistance - 1)) {
                            curPOS = ((curPOS & 0xF0) | (i & 0x0F));
                            curDistance--;
                            printf("CUR POS : (%d, %d) , Distance : %d\n", ((curPOS >> 4) & 0x0F), curPOS & 0x0F,
                                   distanceArr[(curPOS & 0x0F) * 16 + ((curPOS >> 4) & 0x0F)]);
                            goto nextIter;

                        }
                        else break;
                    }
                }

            if ((curField.getTileInfo(((curPOS >> 4) & 0x0F), curPOS & 0x0F) & 0x02) == 0x02) // go l if you can
                for (SMALLTYPE i = ((curPOS >> 4) & 0x0F) ; i >= 0 ; i--) {
                    if ((((curField.getTileInfo(i, curPOS & 0x0F)) & 0x02) == 0) || distanceArr[i + 16 * (curPOS & 0x0F)] == (curDistance - 1)) {
                        if (distanceArr[i + 16 * (curPOS & 0x0F)] == (curDistance - 1)) {
                            curPOS = (((i << 4) & 0xF0) | curPOS & 0x0F);
                            curDistance--;
                            printf("CUR POS : (%d, %d) , Distance : %d\n", ((curPOS >> 4) & 0x0F), curPOS & 0x0F,
                                   distanceArr[(curPOS & 0x0F) * 16 + ((curPOS >> 4) & 0x0F)]);
                            goto nextIter;
                        } else {
                            break;
                        }
                    }
                }

            if ((curField.getTileInfo(((curPOS >> 4) & 0x0F), curPOS & 0x0F) & 0x01) == 0x01) // go r if you can
                for (SMALLTYPE i = ((curPOS >> 4) & 0x0F) ; i < 16 ; i++) {
                    if (((curField.getTileInfo(i, curPOS & 0x0F) & 0x01) == 0) || (distanceArr[i + 16 * (curPOS & 0x0F)] == (curDistance - 1))){
                        if (distanceArr[i + 16 * (curPOS & 0x0F)] == (curDistance - 1)) {
                            curPOS = (((i << 4) & 0xF0) | curPOS & 0x0F);
                            curDistance--;
                            printf("CUR POS : (%d, %d) , Distance : %d\n", ((curPOS >> 4) & 0x0F), curPOS & 0x0F,
                                   distanceArr[(curPOS & 0x0F) * 16 + ((curPOS >> 4) & 0x0F)]);
                            goto nextIter;
                        }
                        else break;
                    }
                }
        }
    }
    else{
        printf("Cannot reach path.\n");
    }
}

/**
 * A function that gets distance from a point
 * @param curField the field to find distance
 * @param pos the starting point
 * @return returns SMALLTYPE array of field that represents the distance. If value is 255, that means unreachable.
 * @param isRobot if this is is for robot finding ways
 */
SMALLTYPE* getDistanceFrom(Field curField, SMALLTYPE pos, bool isRobot){
    SMALLTYPE* distanceArr;
    distanceArr = (SMALLTYPE*) malloc(sizeof(SMALLTYPE) * 256);
    for(uint16_t i = 0 ; i < 256 ; i++) distanceArr[i] = 0xFF;
    //printf("GETTING DISTANCE\n");
    getDistance(curField, ((pos >> 4) & 0x0F), (pos & 0x0F), distanceArr, 0, isRobot);
    //printf("POS : (%u, %u)\n",((pos >> 4) & 0x0F), (pos & 0x0F));
    distanceArr[(((pos >> 4) & 0x0F) + (16 * (pos & 0x0F)))] = 0;

    /**
    for(SMALLTYPE i = 0 ; i < 16 ; i++){
        for(SMALLTYPE j = 0 ; j < 16 ; j++)
            printf("%3u ", distanceArr[16 * i + j]);
        printf("\n");
    }
    */

    return distanceArr;
}

/**
 * A function that calls goDirection function to every direction when hitting a wall.
 * Checks in UP, DN, LEFT, RIGHT order
 * @param curField current field
 * @param x current x pos
 * @param y current y pos
 * @param distanceARR distance array
 * @param distance current distance
 * @param isRobot if this is is for robot finding ways
 */
void getDistance(Field curField, SMALLTYPE x, SMALLTYPE y, SMALLTYPE* distanceARR, SMALLTYPE distance, bool isRobot){
    if (isRobot){
        Field orgField = Field();
        curField.setTileInfo(x, y, orgField.getTileInfo(x, y));
    }
    if ((curField.getTileInfo(x, y) & 0x08) == 0x08) goDirection(curField, x, y - 1, distanceARR, distance + 1, 0x08, isRobot);
    if ((curField.getTileInfo(x, y) & 0x04) == 0x04) goDirection(curField, x, y + 1, distanceARR, distance + 1, 0x04, isRobot);
    if ((curField.getTileInfo(x, y) & 0x02) == 0x02) goDirection(curField, x - 1, y, distanceARR, distance + 1, 0x02, isRobot);
    if ((curField.getTileInfo(x, y) & 0x01) == 0x01) goDirection(curField, x + 1, y, distanceARR, distance + 1, 0x01, isRobot);
}

/**
 * A function that goes into a specific direction.
 * 0x08 -> UP
 * 0x04 -> DN
 * 0x02 -> LEFT
 * 0x01 -> RIGHT
 * @param curField current Field
 * @param x current x pos
 * @param y current y pos
 * @param distanceARR distance array
 * @param distance current distance
 * @param direction direction in hex format
 * @param isRobot if this is is for robot finding ways
 */
void goDirection(Field curField, SMALLTYPE x, SMALLTYPE y, SMALLTYPE* distanceARR, SMALLTYPE distance, SMALLTYPE direction, bool isRobot){
    if ((curField.getTileInfo(x, y) & direction) != direction) { // if not able to move into the direction anymore;
        if (distanceARR[16 * y + x] > distance){
            distanceARR[16 * y + x] = distance; // do relaxation
            getDistance(curField, x, y, distanceARR, distance, isRobot);
        }
        else{
            return;
        }
    }
    else{
        switch (direction) {
            case 0x08: // up
                goDirection(curField, x, y - 1, distanceARR, distance, direction, isRobot);
                break;
            case 0x04: // dn
                goDirection(curField, x, y + 1, distanceARR, distance, direction, isRobot);
                break;
            case 0x02: // left
                goDirection(curField, x - 1, y, distanceARR, distance, direction, isRobot);
                break;
            case 0x01: // right
                goDirection(curField, x + 1, y, distanceARR, distance, direction, isRobot);
        }
    }
}

/**
 * A function that returns total counts that is required to visit epPos from spPos
 * @param curField current field to look at
 * @param spPos starting position
 * @param epPos ending position
 * @param isRobot if this is is for robot finding ways
 * @return returns true if can reach, else false.
 */
SMALLTYPE cntReachFromTo(Field curField, SMALLTYPE spPos, SMALLTYPE epPos, bool isRobot){
    SMALLTYPE* distanceArr = getDistanceFrom(curField, spPos, isRobot);
    return distanceArr[16 * (epPos & 0x0F) + ((epPos >> 4) & 0x0F)];
}
