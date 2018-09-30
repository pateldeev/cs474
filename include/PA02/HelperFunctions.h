#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "Image.h"

namespace Helper {

    //function to print pixel values. Useful for debugging
    void printPixelValues(const ImageType & img);

    //function to linearly remap values in an image between
    void remapValues(ImageType & img);

    //function that applies mask to image at location specified by row and col and returns the new value. The maskCenterRow and maskCenterCol specify the center of the mask.
    int applyMask(const ImageType & img, const ImageType & mask, int row, int col, unsigned int maskCenterRow, unsigned int maskCenterCol, bool normalizeMask = true);

};

#endif
