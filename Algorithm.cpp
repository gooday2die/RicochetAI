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

/**
 * A member function for class neighborAlgorithm to find its path.
 */
void neighborAlgorithm::findPath(){
    //printf("SP : (%d, %d) --> EP : (%d, %d)\n", (spPos >> 4) & 0x0F, (spPos & 0x0F), (epPos >> 4) & 0x0F, (epPos & 0x0F));
    //if (cntReachFromTo(curField, spPos, epPos) != 255) {
    //    findPathFromTo(curField, spPos, epPos);
    //}
    //else {
    //    printf("Cannot find any ways without moving other robots.\n");
    //    findNeighborPath(epPos);
    //}
    movementResult a = findWay(0, epPos, 2);
    for(SMALLTYPE i = 0 ; i < 3 ; i++){
        printf("RESULT %d , MOVE CNT : %d, ROBOT ORDER %d, ROBOT DESTINATION %x\n", i, a.movementCount[i], a.robotOrder[i], a.robotDestination[i]);
    }
    //printf("COUNT : %d\n", a.movementCount[0]);
    //printf("ROBOT : %d to %x\n", a.robotOrder[0], a.robotDestination[0]);
    //printf("MOVING SP to EP: %d\n", cntReachFromTo(a.field[0], spPos, epPos));
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

movementResult neighborAlgorithm::findWay(SMALLTYPE curDepth, SMALLTYPE curPos, SMALLTYPE maxDepth) {
    //printf("DEPTH : %d\n", curDepth);
    if (curDepth == maxDepth) {
        movementResult result;
        SMALLTYPE minCnt = 255; // minimum count.
        result.movementCount[curDepth - maxDepth] = 255; // set it unreachable initially.
        for (SMALLTYPE i = 0; i < 3; i++) result.robotOrder[i] = 255;
        //printf("Currently trying to visit %x with robot\n", curPos);

        for (SMALLTYPE i = 0; i < 3; i++) {
            Field newField = curField.removeRobot(i); // have to remove robot before moving
            SMALLTYPE curCnt = cntReachFromTo(newField, curField.robotArray[i], curPos);
            if (minCnt > curCnt) { // if we found curPos is reachable, record that.
               printf("Can visit %x with Robot %d / Distance : %d\n", curPos, i, curCnt);

                minCnt = curCnt;
                result.movementCount[curDepth - maxDepth] = curCnt; // record min count of movements
                result.robotDestination[curDepth - maxDepth] = curPos; // check destination of robot
                result.robotOrder[curDepth - maxDepth] = i; // record current robot order
                result.field[curDepth - maxDepth] = curField.removeRobot(
                        i); // update the current field into the one that the robot moved.
                result.field[curDepth - maxDepth].setRobot(i, curPos);
            }
        }
        return result;
    } else { // if depth is not 3
        movementResult resultArr[4]; // gather all 4 directions
        for (SMALLTYPE i = 0; i < 4; i++) resultArr[i].movementCount[curDepth - 1] = 255; // init with unreachable
        if ((curPos & 0x0F) >= 1) { // if can go up, go up
            resultArr[0] = findWay(curDepth + 1, (((curPos & 0x0F) - 1) | (curPos & 0xF0)), maxDepth); // up
        }
        if ((curPos & 0x0F) < 15) {  // if can go dn, go dn
            resultArr[1] = findWay(curDepth + 1, (((curPos & 0x0F) + 1) | (curPos & 0xF0)), maxDepth); // dn
        }
        if (((curPos >> 4) & 0x0F) >= 1) {  // if can go left, go left
            resultArr[2] = findWay(curDepth + 1,
                                   ((curPos & 0x0F) | ((((curPos >> 4) & 0x0F) - 1) << 4) & 0xF0), maxDepth); // left
        }
        if (((curPos >> 4) & 0x0F) < 15) {  // if can go right, go right.
            resultArr[3] = findWay(curDepth + 1,
                                   ((curPos & 0x0F) | ((((curPos >> 4) & 0x0F) + 1) << 4) & 0xF0), maxDepth); // right
        }

        SMALLTYPE smallestIndex = 0; // set smallest index as 0
        uint16_t totalArr[4]; // to save total values and to compare
        for(SMALLTYPE i = 0 ; i < 4 ; i++){ // for all results
            if (resultArr[i].movementCount[maxDepth - curDepth - 1] != 255){ // if it is reachable
                Field newField; // make new field
                for(SMALLTYPE j = 0 ; j < 3 ; j++){ // for all robots
                    SMALLTYPE curBest = 255;
                    if (j != resultArr[i].robotOrder[j]){ // if not used before
                        newField = resultArr[i].field[maxDepth - curDepth - 1].removeRobot(j); // make new field
                        SMALLTYPE count = cntReachFromTo(newField, j, curPos); // get count
                        printf("RECUR: Can visit %x with Robot %d / Distance : %d\n", curPos, j, count);
                        if (curBest > count){ // if current count is smaller than before
                            resultArr[i].movementCount[maxDepth - curDepth] = count; // update
                            resultArr[i].robotOrder[maxDepth - curDepth] = j;
                            resultArr[i].robotDestination[maxDepth - curDepth] = curPos;
                            newField.setRobot(j, curPos);
                            resultArr[i].field[maxDepth - curDepth] = newField;
                        }
                    }
                }
            }
            for (SMALLTYPE j = 0 ; j < 3 ; j++) totalArr[i] +=  resultArr[i].movementCount[j];
            if (totalArr[smallestIndex] > totalArr[i]) smallestIndex = i;
            //printf("RESULT %d : total %d\n", i, totalArr[i]);
        }
        //printf("Smallest Index: %d, Value : %d", smallestIndex, totalArr[smallestIndex]);
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