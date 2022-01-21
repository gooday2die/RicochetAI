#include <iostream>
#include "Field.h"
#include "Robot.h"

int main() {
    Field allField = Field();
    allField.setField();
    //allField.printField();

    getDistanceFrom(allField, 0x36);
    getDistanceFrom(allField, 0x94);

}
