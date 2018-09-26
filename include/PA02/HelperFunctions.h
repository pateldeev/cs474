#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "Image.h"

namespace Helper{

//function to linearly remap values in an image between
void remapValues(ImageType & img);

//function that applies mask to image at location specfied by row and col. The maskCenterRow and maskCenterCol specify the center of the mask.
void applyMask(ImageType & img, const ImageType & mask, int row, int col, unsigned int maskCenterRow, unsigned int maskCenterCol);

};

#endif
