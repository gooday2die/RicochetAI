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
    distanceArr = getDistanceFrom(curField, spPos, 0);
    if (distanceArr[16 * (epPos & 0x0F) + ((epPos >> 4) & 0x0F)] != 255) {
        findPathFromTo(curField, spPos, epPos, 0);
    }
    else {
        printf("NOW WE HAVE TO MOVE ROBOTS\n");
        generateFieldDFS(curField, 0, 10);
        findPathFromTo(resultField, spPos, epPos, 0);
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
        distanceArr = getDistanceFrom(curField, spPos, 0);
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
    printf("SP : (%d, %d) --> EP : (%d, %d)\n", (spPos >> 4) & 0x0F, (spPos & 0x0F), (epPos >> 4) & 0x0F, (epPos & 0x0F));
    if (cntReachFromTo(curField, spPos, epPos, 0) != 255) {
        findPathFromTo(curField, spPos, epPos, 0);
    }
    else {
        printf("Cannot find any ways without moving other robots.\n");
        findNeighborPath(epPos);
    }
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
        cntArr[0] = cntReachFromTo(newField, spPos, epPos, 0);
        if(cntArr[0] != 255){ // if possible to reach ep with assist of other robots.
            printf("DN OK : CNT : %d \n", cntArr[0]);
            placeNeighbors((pos & 0xF0) | ((pos & 0x0F) + 1));
            //findPathFromTo(newField, spPos, epPos, 0);
        }
    }

    if ((pos & 0x0F) >= 1) { // put one in up and check if it works
        printf("\nUP : (%d, %d)\n", (pos >> 4) & 0x0F, (pos & 0x0F) - 1);
        Field newField = curField.simulateRobotPos((pos & 0xF0) | ((pos & 0x0F) - 1));
        cntArr[1] = cntReachFromTo(newField, spPos, epPos, 0);
        if(cntArr[1] != 255){
            printf("UP OK : CNT : %d \n", cntArr[1]);
            placeNeighbors((pos & 0xF0) | ((pos & 0x0F) - 1));
            //findPathFromTo(newField, spPos, epPos, 0);

        }
    }

    if (((pos >> 4) & 0x0F) >= 1){ // put one in left and chceck if it works
        printf("\nLEFT : (%d, %d)\n", ((pos >> 4) & 0x0F) - 1, pos & 0x0F);
        Field newField = curField.simulateRobotPos(((((pos >> 4) & 0x0F) - 1) << 4) | (pos & 0x0F));
        cntArr[2] = cntReachFromTo(newField, spPos, epPos, 0);
        if(cntArr[2] != 255){
            printf("LEFT OK : CNT : %d \n", cntArr[2]);
            placeNeighbors(((((pos >> 4) & 0x0F) - 1) << 4) | (pos & 0x0F));
            //findPathFromTo(newField, spPos, epPos, 0);

        }
    }

    if (((pos >> 4) & 0x0F) < 15) { // put one in right and check if it works
        printf("\nRIGHT : (%d, %d)\n", ((pos >> 4) & 0x0F) + 1, pos & 0x0F);
        Field newField = curField.simulateRobotPos(((((pos >> 4) & 0x0F) + 1) << 4) | (pos & 0x0F));
        cntArr[3] = cntReachFromTo(newField, spPos, epPos, 0);
        if (cntArr[3] != 255){
            printf("RIGHT OK : CNT : %d \n", cntArr[3]);
            placeNeighbors(((((pos >> 4) & 0x0F) + 1) << 4) | (pos & 0x0F));
            //findPathFromTo(newField, spPos, epPos, 0);
        }
    }
}


/**
 * A member function for class neighborAlgorithm to find ways to place neighbor robots.
 * Tries to put a robot at the pos with recursive features.
 * @param pos current position to set robots at
 * @return returns total count of moves to make that neighbor robot placed in needed spot.
 */
SMALLTYPE neighborAlgorithm::placeNeighbors(SMALLTYPE pos){
    for(SMALLTYPE i = 0 ; i < 3 ; i++){
        Field tmpField = Field();
        for(SMALLTYPE j = 0 ; j < 3 ; j++){
            tmpField.setRobot(j, curField.robotArray[j]);
        }
        tmpField.printField();

        SMALLTYPE count = cntReachFromTo(tmpField, tmpField.robotArray[i], pos, 1);
        SMALLTYPE* arr = getDistanceFrom(tmpField, tmpField.robotArray[i], 1);
        printf("%x\n", pos);
        count = arr[16 * (pos & 0x0F) + ((pos >> 4) & 0x0F)];
        printf("ROBOT %d , POS : %x, CNT : %d\n", i, tmpField.robotArray[i], count);
        for(SMALLTYPE j = 0 ; j < 16 ; j++){
            for(SMALLTYPE k = 0 ; k < 16 ; k++)
                printf("%3u ", arr[16 * j + k]);
            printf("\n");
        }
        if(count != 255){
            printf("CAN SET NEIGHBORS : %d\n", count);
            printf("MOVING ROBOT %d:\n", i);
            findPathFromTo(tmpField, tmpField.robotArray[i], pos, 1);
            //return count;
        }

    }
    return 1;
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