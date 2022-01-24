#include <iostream>
#include "Field.h"
#include "Algorithm.h"

int main() {
    Field allField = Field();
    allField.setField();
    allField.setRobot(0, 0x45);
    allField.printField();
    //allField.printField();
    neighborAlgorithm a = neighborAlgorithm();
    a.findPath();
    a.curField.printField();

     /**
    newField = allField.moveRobot(0, 0x08);
    newField.printField();
    newField = allField.moveRobot(0, 0x01);
    newField.printField();
    newField = newField.moveRobot(0, 0x04);
    newField.printField();
    newField = newField.moveRobot(0, 0x01);
    newField.printField();
    newField = allField.moveRobot(0, 0x04);
    newField.printField();
    findPathFromTo(newField, 0xF5, 0x37);
    printf("===========================\n\n\n");
    newField = allField.moveRobot(0, 0x08);
    newField.printField();
    findPathFromTo(newField, 0xF5, 0x37);
    */

}
