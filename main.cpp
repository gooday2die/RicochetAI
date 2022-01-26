#include <iostream>
#include "Field.h"
#include "Algorithm.h"

int main() {
    Field allField = Field();
    allField.setField();
    neighborAlgorithm a = neighborAlgorithm();
    a.findPath();
}
