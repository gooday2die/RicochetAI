#include <iostream>
#include "Field.h"
#include "Robot.h"

int main() {
    Field allField = Field();
    allField.setField();
    allField.printField();

    findPathFromTo(allField, 0xF5, 0x37);
    //getDistanceFrom(allField, 0x36);
    //getDistanceFrom(allField, 0x94);

}
