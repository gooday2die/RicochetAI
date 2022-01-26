//
// Created by pc on 2022-01-24.
//

#include "Algorithm.h"


/**
 * A member function that finds path for ricochet robot.
 * First it tries without moving other robots.
 * If it is possible to reach final destination without other robots, use that path.
 * Else, find with Depth Searching.
 */
void neighborAlgorithm::findPath(){
    Movements m;
    SMALLTYPE cnt = 0;
    cnt = cntReachFromTo(curField, spPos, epPos);
    if (cnt != 255){ // if it is reachable without other robots
        printf("Can Find path without moving robots : %d\n", cnt);
        findPathFromTo(curField, spPos, epPos);
    }
    else { // if it is not reachable without other robots
        for (SMALLTYPE i = 1; i < 4; i++) {
            printf("Trying with Depth %d\n", i);
            m = findWay(0, epPos, i, epPos, spPos);
            if (m.getCanMove()) {
                printf("Found Solution with Depth %d\n", i);
                printf("Moving from %x to %x\n", spPos, epPos);
                m.printMovements(curField);
                findPathFromTo(m.getField(m.getDepth() - 1), spPos, epPos);
                printf("TOTAL MOVES : %d \n",
                       m.getTotalMoves() + cntReachFromTo(m.getField(m.getDepth() - 1), spPos, epPos));
                break;
            }
        }
    }
}

/**
 * A member function that finds recursive neighbors using recursive call.
 * Has some dirty printf debugging sessions.
 * @param curDepth current dfs depth
 * @param curPos current position of tile
 * @param maxDepth max dfs depth
 * @param epPos ending position
 * @param spPos starting position
 * @return returns Movements object that contains results
 */
Movements neighborAlgorithm::findWay(SMALLTYPE curDepth, SMALLTYPE curPos, SMALLTYPE maxDepth, SMALLTYPE epPos, SMALLTYPE spPos) {
    if (curDepth == maxDepth) { // base case. When it had reached max depth
        Movements result = Movements(); // generate a object
        SMALLTYPE minCnt = 255; // minimum count.
        SMALLTYPE curCnt; // current distance of robot
        SMALLTYPE bestRobot = 0; // best robot to return
        for (SMALLTYPE i = 0 ; i < 3; i++) { // for all robots
            Field newField = curField.removeRobot(i); // have to remove robot before moving
            curCnt = cntReachFromTo(newField, curField.robotArray[i], curPos);
            if (minCnt > curCnt) { // if we found curPos is reachable, record that.
               //printf("Can visit %x with Robot %d / Distance : %d\n", curPos, i, curCnt);
                minCnt = curCnt; // dump values
                bestRobot = i;
                result.setMoveCount(curCnt);
                result.setCanMove(minCnt != 255);
                result.setDestination(curPos);
                newField.setRobot(i, curPos);
                result.setField(newField);
            }
        }
        result.addTotalMoves(curCnt); // add total moves here before moving to next depth
        result.setMovedRobot(bestRobot);
        //printf("MAXDEPTH : USED ROBOT %d\n", bestRobot);
        //printf("MAXDEPTH : visit %x with Robot %d, distance : %d\n", curPos, result.getMovedRobot(result.getDepth()),result.getMoveCount(result.getDepth()));
        result.nextDepth();
        //printf("MAXDEPTH : Added Depth. Cur Depth : %d\n", result.getDepth());

        return result; // return object
    }

    else { // if depth is not maxDepth
        Movements resultArr[4]; // gather all 4 directions
        if ((curPos & 0x0F) >= 1) { // if can go up, go up
            resultArr[0] = findWay(curDepth + 1, (((curPos & 0x0F) - 1) | (curPos & 0xF0)), maxDepth,  epPos,
                                   spPos); // up
        }
        if ((curPos & 0x0F) < 15) {  // if can go dn, go dn
            resultArr[1] = findWay(curDepth + 1, (((curPos & 0x0F) + 1) | (curPos & 0xF0)), maxDepth, epPos,
                                   spPos); // dn
        }
        if (((curPos >> 4) & 0x0F) >= 1) {  // if can go left, go left
            resultArr[2] = findWay(curDepth + 1,
                                   ((curPos & 0x0F) | (((((curPos >> 4) & 0x0F) - 1) << 4) & 0xF0)), maxDepth,  epPos,
                                   spPos); // left
        }
        if (((curPos >> 4) & 0x0F) < 15) {  // if can go right, go right.
            resultArr[3] = findWay(curDepth + 1,
                                   ((curPos & 0x0F) | (((((curPos >> 4) & 0x0F) + 1) << 4) & 0xF0)), maxDepth,  epPos,
                                   spPos); // right
        }

        SMALLTYPE disableCnt = 0; // count how many disabled results are
        Movements result; // result to return
        for (SMALLTYPE i = 0; i < 4; i++) disableCnt += !(resultArr[i].getCanMove());
        if (disableCnt == 4) {
            result.setCanMove(0);
            //printf("Cannot reach %x\n", curPos);
            return result;
        } // totally unreachable case. Give up

        if (curDepth == 0) { // if this was depth 0. Find the best result.
            //printf("CurDepth 0\n");
            SMALLTYPE bestIndex;
            SMALLTYPE bestTotal = 255;
            //for(SMALLTYPE i = 0 ; i < 4 ; i++) printf("DEPTH 0 : Result %d : Total %d / depth %d / can Move : %d / POS : %x\n", i, resultArr[i].getTotalMoves(), resultArr[i].getDepth(), resultArr[i].getCanMove(), resultArr[i].getDestination(resultArr[i].getDepth() - 1));
            for (SMALLTYPE i = 0; i < 4; i++) { // for all results
                SMALLTYPE count;
                SMALLTYPE curTotal;
                //printf("Result %d , Depth : %d \n", i, resultArr[i].getDepth());
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
            //printf("BEST INDEX: %d\n", bestIndex);
            resultArr[bestIndex].setCanMove(bestTotal != 255); // set canMove and return
            SMALLTYPE cnt = cntReachFromTo(resultArr[bestIndex].getField(resultArr[bestIndex].getDepth() - 1), spPos, epPos);
            //printf("Using Result %d , Total Robot Moves : %d ,  To Move : %d\n", bestIndex, resultArr[bestIndex].getTotalMoves(), cnt);
            return resultArr[bestIndex];

        } else { // which is internal recursions.
            for (SMALLTYPE i = 0; i < 4; i++) { // for all results
                if (resultArr[i].getCanMove()) {
                    Field newField;
                    SMALLTYPE curBest = 255;
                    SMALLTYPE bestRobot = 0;
                    for (SMALLTYPE j = 0; j < 3; j++) {
                        //printf("Recursion %d //// ROBOT %d is %d\n", curDepth, j, resultArr[i].isUsedRobot(j));
                        if (!resultArr[i].isUsedRobot(j)) {
                            newField = resultArr[i].getField(resultArr[i].getDepth() - 1);
                            newField.removeRobot(j);
                            //printf("RECURSION %d //// ROBOT : %x -> %x\n", curDepth, curField.robotArray[j], curPos);
                            SMALLTYPE count = cntReachFromTo(newField, curField.robotArray[j], curPos);
                            resultArr[i].setCanMove(count != 255);
                            if ((count != 255) && (curBest > count)){
                                //printf("====Recursion %d : CURPOS %x / Using robot :%d to %x, Distance : %d / RESULT %d\n", curDepth, curPos, j,
                                //       curPos, count , i);
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
                    //printf("RECURSION %d, USED ROBOT %d\n", curDepth, bestRobot);
                    resultArr[i].setCanMove(resultArr[i].getMoveCount(resultArr[i].getDepth()) != 255);
                    resultArr[i].nextDepth();
                    //printf("=a=a Recursion %d : CURPOS %x / Using Robot %d to %x / Distance %d / canMove : %d / TotalMove %d\n", curDepth, curPos, resultArr[i].getMovedRobot(resultArr[i].getDepth() -1),
                    //                                                                                                                       resultArr[i].getDestination(resultArr[i].getDepth() -1 ), resultArr[i].getMoveCount(resultArr[i].getDepth() -1 ),
                    //                                                                                                                       resultArr[i].getCanMove(), resultArr[i].getTotalMoves());
                    //printf("Recursion %d : Added Depth. Cur Depth : %d\n", curDepth, resultArr[i].getDepth());
                }
            }

            SMALLTYPE smallestIndex = 0;
            SMALLTYPE smallestValue = 255;
            for (SMALLTYPE i = 0; i < 4; i++) {
                if ((smallestValue > resultArr[i].getTotalMoves()) && resultArr[i].getCanMove()) {
                    smallestValue = resultArr[i].getTotalMoves();
                    smallestIndex = i;
                }
                //printf("Recursion %d : curPOS : %x , Result %d / POS %x - total moves %d / can move %d\n", curDepth, curPos, i, resultArr[i].getDestination(resultArr[i].getDepth() - 1), resultArr[i].getTotalMoves(), resultArr[i].getCanMove());
            }

            //printf("Recursion %d cURPOS %x: Choosing result %d\n", curDepth, curPos, smallestIndex);
            //printf("Recursion %d : Moving Robot %d to %x. Distance %d\n", curDepth,
            //       resultArr[smallestIndex].getMovedRobot(resultArr[smallestIndex].getDepth() - 1),
            //       resultArr[smallestIndex].getDestination(resultArr[smallestIndex].getDepth() - 1),
            //       resultArr[smallestIndex].getMoveCount(resultArr[smallestIndex].getDepth()) - 1);
            resultArr[smallestIndex].setCanMove(smallestValue != 255);
            return resultArr[smallestIndex];
        }
    }
}

/**
 * A function for running algorithm. It was supposed to be polymorphismic but it kinda broke.
 * Currently fixing this function
 * @param a the algorithm to use
 * @param spPos starting position
 * @param epPos ending position
 */
void runAlgorithm(neighborAlgorithm* a, SMALLTYPE spPos, SMALLTYPE epPos){
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