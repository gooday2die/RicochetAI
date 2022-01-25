//
// Created by pc on 2022-01-24.
//

#include "Algorithm.h"

/**
 * A member function for class randomAlgorithm for finding path.
 * DOES NOT WORK. Depreciated.
 */
void randomAlgorithm::findPath(){
    printf("SP : (%d, %d) --> EP : (%d, %d)\n", (spPos >> 4) & 0x0F, (spPos & 0x0F), (epPos >> 4) & 0x0F, (epPos & 0x0F));
    SMALLTYPE* distanceArr;
    distanceArr = getDistanceFrom(curField, spPos);
    if (distanceArr[16 * (epPos & 0x0F) + ((epPos >> 4) & 0x0F)] != 255) {
        findPathFromTo(curField, spPos, epPos);
    }
    else {
        printf("NOW WE HAVE TO MOVE ROBOTS\n");
        generateFieldDFS(curField, 0, 10);
        findPathFromTo(resultField, spPos, epPos);
        resultField.printField();
    }
}
/**
 * A member function for class randomAlgorithm for DFS finding
 * @param curField current field
 * @param curDepth current depth
 * @param maxDepth DFS max depth
 */
void randomAlgorithm::generateFieldDFS(Field curField, SMALLTYPE curDepth, SMALLTYPE maxDepth) {
    if(curDepth == maxDepth) return;
    else{
        SMALLTYPE* distanceArr;
        distanceArr = getDistanceFrom(curField, spPos);
        if (distanceArr[16 * (epPos & 0x0F) + ((epPos >> 4) & 0x0F)] != 255) {
            resultField = curField;
            printf("Found Solution!!!\n");
            return;
        }
        else{
            for(SMALLTYPE i = 0 ; i < 3 ; i++){
                for(SMALLTYPE j = 0 ; j < 4 ; j++){
                    generateFieldDFS((curField.moveRobot(i, 0x01 << j)), curDepth + 1, maxDepth);
                }
            }
        }
    }
}

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

SMALLTYPE Visited::getVisited(SMALLTYPE pos){
    SMALLTYPE x = (pos >> 4) & 0x0F;
    SMALLTYPE y = pos & 0x0F;

    return visited[16 * y + x];
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

void Visited::setVisited(SMALLTYPE pos){
    SMALLTYPE x = (pos >> 4) & 0x0F;
    SMALLTYPE y = pos & 0x0F;
    visited[16 * y + x] = 1;
}

void Movements::printMovements() {
    for(SMALLTYPE i = 0 ; i < curDepth ; i++){
        printf("%d) Move Robot : %d to %x. Distance : %d\n", i, mvs[i].movedRobot, mvs[i].destination, mvs[i].moveCount);
    }
    printf("Total Move Count : %d\n", totalMoves);
}

/**
 * A member function for class neighborAlgorithm to find its path.
 */
void neighborAlgorithm::findPath(){
    Movements m;
    Visited v;
    m = findWay(0, epPos, 2, v);
    printf("\n\n\n");
    m.printMovements();
    printf("Depth : %d\n", m.curDepth);
    //  m.getField(m.getDepth() - 1).printField();
    //findPathFromTo(m.getField(m.getDepth() - 1), spPos, epPos);
}


/**
 * A member function for class neighborAlgorithm to find neighborPath.
 * This member function assumes that there is a block in each direction (UP, DOWN, LEFT, RIGHT) and consider
 * if it is possible for the starting point to reach ending point
 * @param pos the position of the current ending point.
 **/
void neighborAlgorithm::findNeighborPath(SMALLTYPE pos){
    SMALLTYPE cntArr[4];

    for(SMALLTYPE i = 0 ; i < 4 ; i++) cntArr[i] = 0xFF; // clear all cntArr as 255.

    if ((pos & 0x0F) < 15) {  //  put one in down and check if it works
        printf("\nDN : (%d, %d)\n", (pos >> 4) & 0x0F, (pos & 0x0F) + 1);
        Field newField = curField.simulateRobotPos((pos & 0xF0) | ((pos & 0x0F) + 1));
        cntArr[0] = cntReachFromTo(newField, spPos, epPos);
        if(cntArr[0] != 255){ // if possible to reach ep with assist of other robots.
            printf("DN OK : CNT : %d \n", cntArr[0]);
            placeNeighbors((pos & 0xF0) | ((pos & 0x0F) + 1));
            //findPathFromTo(newField, spPos, epPos, 0);
        }
    }

    if ((pos & 0x0F) >= 1) { // put one in up and check if it works
        printf("\nUP : (%d, %d)\n", (pos >> 4) & 0x0F, (pos & 0x0F) - 1);
        Field newField = curField.simulateRobotPos((pos & 0xF0) | ((pos & 0x0F) - 1));
        cntArr[1] = cntReachFromTo(newField, spPos, epPos);
        if(cntArr[1] != 255){
            printf("UP OK : CNT : %d \n", cntArr[1]);
            placeNeighbors((pos & 0xF0) | ((pos & 0x0F) - 1));
            //findPathFromTo(newField, spPos, epPos, 0);

        }
    }

    if (((pos >> 4) & 0x0F) >= 1){ // put one in left and chceck if it works
        printf("\nLEFT : (%d, %d)\n", ((pos >> 4) & 0x0F) - 1, pos & 0x0F);
        Field newField = curField.simulateRobotPos(((((pos >> 4) & 0x0F) - 1) << 4) | (pos & 0x0F));
        cntArr[2] = cntReachFromTo(newField, spPos, epPos);
        if(cntArr[2] != 255){
            printf("LEFT OK : CNT : %d \n", cntArr[2]);
            placeNeighbors(((((pos >> 4) & 0x0F) - 1) << 4) | (pos & 0x0F));
            //findPathFromTo(newField, spPos, epPos, 0);

        }
    }

    if (((pos >> 4) & 0x0F) < 15) { // put one in right and check if it works
        printf("\nRIGHT : (%d, %d)\n", ((pos >> 4) & 0x0F) + 1, pos & 0x0F);
        Field newField = curField.simulateRobotPos(((((pos >> 4) & 0x0F) + 1) << 4) | (pos & 0x0F));
        cntArr[3] = cntReachFromTo(newField, spPos, epPos);
        if (cntArr[3] != 255){
            printf("RIGHT OK : CNT : %d \n", cntArr[3]);
            placeNeighbors(((((pos >> 4) & 0x0F) + 1) << 4) | (pos & 0x0F));
            //findPathFromTo(newField, spPos, epPos, 0);
        }
    }

    for(SMALLTYPE i = 0 ; i < 3 ; i++) {
        Field newField = curField.removeRobot(i);
        SMALLTYPE* arr = getDistanceFrom(newField, curField.robotArray[i]);
        for(SMALLTYPE j = 0 ; j < 16 ; j++){
            for(SMALLTYPE k = 0 ; k < 16 ; k++)
                printf("%3u ", arr[j * 16 + k]);
            printf("\n");
        }
        printf("\n\n\n");
    }
}

/**
 * A member function for class neighborAlgorithm to find ways to place neighbor robots.
 * Tries to put a robot at the pos with recursive features.
 * @param pos current position to set robots at
 * @return returns total count of moves to make that neighbor robot placed in needed spot.
 */
SMALLTYPE neighborAlgorithm::placeNeighbors(SMALLTYPE pos) {
    for(SMALLTYPE i = 0 ; i < 3 ; i++){
        Field tmpField = curField.removeRobot(i);
        SMALLTYPE count = cntReachFromTo(tmpField, curField.robotArray[i], pos);
        printf("RESULT : %d\n", count);
        if (count != 255){
            printf("Can place Robot %d : to %x\n", i , pos);
            for (SMALLTYPE j = 0 ; j < 3 ; j++){ // search if that pos can be reached by placing other blocks.
            }
        }
    }
    return 255;
}

Movements neighborAlgorithm::findWay(SMALLTYPE curDepth, SMALLTYPE curPos, SMALLTYPE maxDepth, Visited v) {
    if (curDepth == maxDepth) {
        Movements result = Movements();
        SMALLTYPE minCnt = 255; // minimum count.
        for (SMALLTYPE i = 0 ; i < 3; i++) {
            Field newField = curField.removeRobot(i); // have to remove robot before moving
            SMALLTYPE curCnt = cntReachFromTo(newField, curField.robotArray[i], curPos);
            if (minCnt > curCnt) { // if we found curPos is reachable, record that.
               //printf("Can visit %x with Robot %d / Distance : %d\n", curPos, i, curCnt);
                minCnt = curCnt;
                result.setMoveCount(curCnt);
                result.setCanMove(minCnt != 255);
                result.setMovedRobot(i);
                result.setDestination(curPos);
                newField.setRobot(i, curPos);
                result.setField(newField);
                result.addTotalMoves(curCnt);
                v.setVisited(curPos);
            }
        }
        printf("MAXDEPTH : visit %x with Robot %d, distance : %d\n", curPos, result.getMovedRobot(result.getDepth()),result.getMoveCount(result.getDepth()));
        result.nextDepth();
        printf("MAXDEPTH : Added Depth. Cur Depth : %d\n", result.getDepth());

        return result;

    } else { // if depth is not maxDepth
        printf("DEPTH %d\n", curDepth);
        Movements resultArr[4]; // gather all 4 directions
        if ((curPos & 0x0F) >= 1) { // if can go up, go up
            resultArr[0] = findWay(curDepth + 1, (((curPos & 0x0F) - 1) | (curPos & 0xF0)), maxDepth, v); // up
        }
        if ((curPos & 0x0F) < 15) {  // if can go dn, go dn
            resultArr[1] = findWay(curDepth + 1, (((curPos & 0x0F) + 1) | (curPos & 0xF0)), maxDepth, v); // dn
        }
        if (((curPos >> 4) & 0x0F) >= 1) {  // if can go left, go left
            resultArr[2] = findWay(curDepth + 1,
                                   ((curPos & 0x0F) | (((((curPos >> 4) & 0x0F) - 1) << 4) & 0xF0)), maxDepth, v); // left
        }
        if (((curPos >> 4) & 0x0F) < 15) {  // if can go right, go right.
            resultArr[3] = findWay(curDepth + 1,
                                   ((curPos & 0x0F) | (((((curPos >> 4) & 0x0F) + 1) << 4) & 0xF0)), maxDepth, v); // right
        }

        SMALLTYPE disableCnt = 0;
        Movements result;
        for(SMALLTYPE i = 0 ; i < 4 ; i++) disableCnt += !(resultArr[i].getCanMove());
        if (disableCnt == 4){
            result.setCanMove(0);
            printf("Cannot reach %x\n", curPos);
            return result;
        }

        for(SMALLTYPE i = 0 ; i < 4 ; i++) { // for all results
            if ((resultArr[i].getCanMove()) && !(v.getVisited(curPos))) {
                Field newField;
                //v.setVisited(curPos);
                SMALLTYPE curBest = 255;
                SMALLTYPE bestRobot = 0;
                for (SMALLTYPE j = 0; j < 3; j++) {
                    if (!resultArr[i].isUsedRobot(j)) {
                        printf("Recursion %d : Using robot :%d to %x\n", curDepth, j, resultArr[i].getDestination(resultArr[i].getDepth()- 1));
                        newField = resultArr[i].getField(resultArr[i].getDepth() - 1);
                        newField.removeRobot(j);
                        SMALLTYPE count = cntReachFromTo(newField, j, curPos);
                        if (count != 255) {
                            if (curBest > count) {
                                bestRobot = j;
                                curBest = count;
                                resultArr[i].setCanMove(count != 255);
                                resultArr[i].setDestination(curPos);
                                resultArr[i].setMoveCount(count);
                                newField.setRobot(j, curPos);
                                resultArr[i].setField(newField);

                            }
                        }
                    }
                }
                resultArr[i].addTotalMoves(resultArr[i].getMoveCount(resultArr[i].getDepth()));
                resultArr[i].setMovedRobot(bestRobot);
                resultArr[i].nextDepth();

                printf("Recursion %d : Added Depth. Cur Depth : %d\n", curDepth, resultArr[i].getDepth());
            }
        }

        SMALLTYPE smallestIndex = 0;
        SMALLTYPE smallestValue = resultArr[smallestIndex].getTotalMoves();
        for(SMALLTYPE i = 0 ; i < 4 ; i++)
            if(resultArr[i].getTotalMoves() != 0) smallestIndex = i;
        for(SMALLTYPE i = 0 ; i < 4 ; i++) {
            if((smallestValue > resultArr[i].getTotalMoves()) && resultArr[i].getCanMove()){
                smallestIndex = i;
                printf("Recursion %d : Result %d - total moves %d\n", curDepth, i, resultArr[i].getTotalMoves());

            }
        }
        printf("Choosing result %d\n", smallestIndex);
        printf("Recursion %d : Moving Robot %d to %x. Distance %d\n", curDepth, resultArr[smallestIndex].getMovedRobot(resultArr[smallestIndex].getDepth() - 1),
               resultArr[smallestIndex].getDestination(resultArr[smallestIndex].getDepth() - 1), resultArr[smallestIndex].getMoveCount(resultArr[smallestIndex].getDepth()) - 1);
        resultArr[smallestIndex].addTotalMoves(resultArr[smallestIndex].getMoveCount(resultArr[smallestIndex].getDepth() - 1));
        return resultArr[smallestIndex];
    }
}


void runAlgorithm(Algorithm* a, SMALLTYPE spPos, SMALLTYPE epPos){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,15); // distribution in range [1, 6]

    a->curField.setRobot(0, 0x1B);
    //a->curField.setRobot(1, 0x81);
    a->curField.setRobot(1, 0x45);
    a->curField.setRobot(2, 0x4E);

    a->curField.printField();

    printf("Running Algorithm\n");
    a->epPos = 0x25;
    a->spPos = 0x10;
    a->findPath();
}