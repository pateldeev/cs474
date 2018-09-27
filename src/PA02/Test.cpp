#include "PA02/HelperFunctions.h"
#include "ReadWrite.h"

#include <iostream>
#include <ctime>

void randomlyPopulateImage(ImageType & img, int numRows, int numCols, int range, int minVal) {
    int tempVal;
    std::srand(std::time(nullptr));
    for (int r = 0; r < numRows; ++r)
        for (int c = 0; c < numCols; ++c) {
            tempVal = ((double) std::rand() / RAND_MAX) * range + minVal;
            img.setPixelVal(r, c, tempVal);
        }
}

int main(int argc, char * argv[]) {
    ImageType img(10, 10, 100);
    randomlyPopulateImage(img, 10, 10, 90, 10);

    ImageType mask(3, 3, 100);
    randomlyPopulateImage(mask, 3, 3, 9, 1);

    Helper::printPixelValues(img);
    Helper::printPixelValues(mask);

    Helper::applyMask(img, mask, 1, 3, 2, 1);

    Helper::printPixelValues(img);
}