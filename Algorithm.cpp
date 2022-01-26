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


void Visited::setVisited(SMALLTYPE pos){
    SMALLTYPE x = (pos >> 4) & 0x0F;
    SMALLTYPE y = pos & 0x0F;
    visited[16 * y + x] = 1;
}

void Movements::printMovements(Field CurField) {
    for(SMALLTYPE i = 0 ; i < curDepth ; i++){
        printf("%d) Move Robot : %d to %x. Distance : %d\n", i, mvs[i].movedRobot, mvs[i].destination, mvs[i].moveCount);
        //mvs[i].field.printField();
        //if(i > 0)
        //    findPathFromTo(mvs[i - 1].field, CurField.robotArray[mvs[i].movedRobot], mvs[i].destination);
        //else
        //    findPathFromTo(CurField, CurField.robotArray[mvs[i].movedRobot], mvs[i].destination);
    }
    printf("Total Move Count : %d\n", totalMoves);
}

/**
 * A member function for class neighborAlgorithm to find its path.
 */
void neighborAlgorithm::findPath(){
    Movements m;
    Visited v;
    m = findWay(0, epPos, 2, v, epPos, spPos);
    printf("\n\n\n");
    m.printMovements(curField);
    printf("Moving from %x to %x\n", spPos, epPos);
    printf("Depth : %d\n", m.curDepth);
    printf("Canmove : %d\n", m.getCanMove());
    //m.getField(m.getDepth() - 2).printField();
    //findPathFromTo(m.getField(m.getDepth() - 2), spPos, epPos);
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

Movements neighborAlgorithm::findWay(SMALLTYPE curDepth, SMALLTYPE curPos, SMALLTYPE maxDepth, Visited v, SMALLTYPE epPos, SMALLTYPE spPos) {
    if (curDepth == maxDepth) {
        Movements result = Movements();
        SMALLTYPE minCnt = 255; // minimum count.
        SMALLTYPE curCnt;
        SMALLTYPE bestRobot = 0;
        for (SMALLTYPE i = 0 ; i < 3; i++) {
            Field newField = curField.removeRobot(i); // have to remove robot before moving
            curCnt = cntReachFromTo(newField, curField.robotArray[i], curPos);
            if (minCnt > curCnt) { // if we found curPos is reachable, record that.
               //printf("Can visit %x with Robot %d / Distance : %d\n", curPos, i, curCnt);
                minCnt = curCnt;
                bestRobot = i;
                result.setMoveCount(curCnt);
                result.setCanMove(minCnt != 255);
                result.setDestination(curPos);
                newField.setRobot(i, curPos);
                result.setField(newField);
                v.setVisited(curPos);
            }
        }
        result.addTotalMoves(curCnt);
        result.setMovedRobot(bestRobot);
        printf("MAXDEPTH : USED ROBOT %d\n", bestRobot);

        printf("MAXDEPTH : visit %x with Robot %d, distance : %d\n", curPos, result.getMovedRobot(result.getDepth()),result.getMoveCount(result.getDepth()));
        result.nextDepth();
        printf("MAXDEPTH : Added Depth. Cur Depth : %d\n", result.getDepth());

        return result;

    }
    else { // if depth is not maxDepth
        printf("DEPTH %d\n", curDepth);
        Movements resultArr[4]; // gather all 4 directions
        if ((curPos & 0x0F) >= 1) { // if can go up, go up
            resultArr[0] = findWay(curDepth + 1, (((curPos & 0x0F) - 1) | (curPos & 0xF0)), maxDepth, v, epPos,
                                   spPos); // up
        }
        if ((curPos & 0x0F) < 15) {  // if can go dn, go dn
            resultArr[1] = findWay(curDepth + 1, (((curPos & 0x0F) + 1) | (curPos & 0xF0)), maxDepth, v, epPos,
                                   spPos); // dn
        }
        if (((curPos >> 4) & 0x0F) >= 1) {  // if can go left, go left
            resultArr[2] = findWay(curDepth + 1,
                                   ((curPos & 0x0F) | (((((curPos >> 4) & 0x0F) - 1) << 4) & 0xF0)), maxDepth, v, epPos,
                                   spPos); // left
        }
        if (((curPos >> 4) & 0x0F) < 15) {  // if can go right, go right.
            resultArr[3] = findWay(curDepth + 1,
                                   ((curPos & 0x0F) | (((((curPos >> 4) & 0x0F) + 1) << 4) & 0xF0)), maxDepth, v, epPos,
                                   spPos); // right
        }

        SMALLTYPE disableCnt = 0; // count how many disabled results are
        Movements result; // result to return
        for (SMALLTYPE i = 0; i < 4; i++) disableCnt += !(resultArr[i].getCanMove());
        if (disableCnt == 4) {
            result.setCanMove(0);
            printf("Cannot reach %x\n", curPos);
            return result;
        } // totally unreachable case. Give up

        if (curDepth == 0) { // if this was depth 0. Find the best result.
            printf("CurDepth 0\n");
            SMALLTYPE bestIndex;
            SMALLTYPE bestTotal = 255;
            for(SMALLTYPE i = 0 ; i < 4 ; i++) printf("DEPTH 0 : Result %d : Total %d / depth %d / can Move : %d / POS : %x\n", i, resultArr[i].getTotalMoves(), resultArr[i].getDepth(), resultArr[i].getCanMove(), resultArr[i].getDestination(resultArr[i].getDepth() - 1));
            for (SMALLTYPE i = 0; i < 4; i++) { // for all results
                SMALLTYPE count;
                SMALLTYPE curTotal;
                printf("Result %d , Depth : %d \n", i, resultArr[i].getDepth());
                if ((resultArr[i].getDepth() != 0) && (resultArr[i].getCanMove())) {
                    count = cntReachFromTo(resultArr[i].getField(resultArr[i].getDepth() - 1), spPos, epPos);
                    curTotal = count + resultArr[i].getTotalMoves();
                    if (((bestTotal > curTotal) && resultArr[i].getCanMove()) &&
                        (count != 255)) { // if best and can move and reachable.
                        bestIndex = i;
                        bestTotal = curTotal;
                    }
                }
            }
            printf("BEST INDEX: %d\n", bestIndex);
            resultArr[bestIndex].setCanMove(bestTotal != 255); // set canMove and return
            SMALLTYPE cnt = cntReachFromTo(resultArr[bestIndex].getField(resultArr[bestIndex].getDepth() - 1), spPos, epPos);
            printf("Using Result %d , Total Robot Moves : %d ,  To Move : %d\n", bestIndex, resultArr[bestIndex].getTotalMoves(), cnt);
            return resultArr[bestIndex];

        } else { // which is internal recursions.
            for (SMALLTYPE i = 0; i < 4; i++) { // for all results
                if (resultArr[i].getCanMove()) {
                    Field newField;
                    SMALLTYPE curBest = 255;
                    SMALLTYPE bestRobot = 0;
                    for (SMALLTYPE j = 0; j < 3; j++) {
                        printf("Recursion %d //// ROBOT %d is %d\n", curDepth, j, resultArr[i].isUsedRobot(j));
                        if (!resultArr[i].isUsedRobot(j)) {
                            newField = resultArr[i].getField(resultArr[i].getDepth() - 1);
                            newField.removeRobot(j);
                            printf("RECURSION %d //// ROBOT : %x -> %x\n", curDepth, curField.robotArray[j], curPos);
                            SMALLTYPE count = cntReachFromTo(newField, curField.robotArray[j], curPos);
                            resultArr[i].setCanMove(count != 255);
                            if ((count != 255) && (curBest > count)){
                                printf("====Recursion %d : CURPOS %x / Using robot :%d to %x, Distance : %d / RESULT %d\n", curDepth, curPos, j,
                                       curPos, count , i);
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
                    resultArr[i].addTotalMoves(resultArr[i].getMoveCount(resultArr[i].getDepth()));
                    resultArr[i].setMovedRobot(bestRobot);
                    printf("RECURSION %d, USED ROBOT %d\n", curDepth, bestRobot);
                    resultArr[i].setCanMove(resultArr[i].getMoveCount(resultArr[i].getDepth()) != 255);
                    resultArr[i].nextDepth();
                    printf("=a=a Recursion %d : CURPOS %x / Using Robot %d to %x / Distance %d / canMove : %d / TotalMove %d\n", curDepth, curPos, resultArr[i].getMovedRobot(resultArr[i].getDepth() -1),
                                                                                                                                           resultArr[i].getDestination(resultArr[i].getDepth() -1 ), resultArr[i].getMoveCount(resultArr[i].getDepth() -1 ),
                                                                                                                                           resultArr[i].getCanMove(), resultArr[i].getTotalMoves());
                    printf("Recursion %d : Added Depth. Cur Depth : %d\n", curDepth, resultArr[i].getDepth());
                }
            }

            SMALLTYPE smallestIndex = 0;
            SMALLTYPE smallestValue = 255;
            for (SMALLTYPE i = 0; i < 4; i++) {
                if ((smallestValue > resultArr[i].getTotalMoves()) && resultArr[i].getCanMove()) {
                    smallestValue = resultArr[i].getTotalMoves();
                    smallestIndex = i;
                }
                printf("Recursion %d : curPOS : %x , Result %d / POS %x - total moves %d / can move %d\n", curDepth, curPos, i, resultArr[i].getDestination(resultArr[i].getDepth() - 1), resultArr[i].getTotalMoves(), resultArr[i].getCanMove());
            }

            printf("Recursion %d cURPOS %x: Choosing result %d\n", curDepth, curPos, smallestIndex);
            printf("Recursion %d : Moving Robot %d to %x. Distance %d\n", curDepth,
                   resultArr[smallestIndex].getMovedRobot(resultArr[smallestIndex].getDepth() - 1),
                   resultArr[smallestIndex].getDestination(resultArr[smallestIndex].getDepth() - 1),
                   resultArr[smallestIndex].getMoveCount(resultArr[smallestIndex].getDepth()) - 1);
            resultArr[smallestIndex].setCanMove(smallestValue != 255);
            return resultArr[smallestIndex];
        }
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