#include "PA02/HelperFunctions.h"

#include <iostream>
#include <assert.h>

//private helper functions
namespace {

    int getSumOfPixels(const ImageType & img) {
        int sum = 0, tempVal, rows, cols, levels;
        img.getImageInfo(rows, cols, levels); //get image information

        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c) {
                img.getPixelVal(r, c, tempVal);
                sum += tempVal; //add each pixel value to sum
            }

        return sum;
    }

    int findMinPixelVal(const ImageType & img) {
        int rows, cols, levels;
        img.getImageInfo(rows, cols, levels); //get image information

        int minVal, tempVal;
        img.getPixelVal(0, 0, minVal);

        //find the mininum value
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c) {
                img.getPixelVal(r, c, tempVal);
                if (tempVal < minVal)
                    minVal = tempVal;
            }

        return minVal;
    }

    int findMaxPixelVal(const ImageType & img) {
        int rows, cols, levels;
        img.getImageInfo(rows, cols, levels); //get image information

        int maxVal, tempVal;
        img.getPixelVal(0, 0, maxVal);

        //find the maximum value
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c) {
                img.getPixelVal(r, c, tempVal);
                if (tempVal > maxVal)
                    maxVal = tempVal;
            }
        return maxVal;
    }
}

void Helper::printPixelValues(const ImageType & img) {
    int rows, cols, levels;
    img.getImageInfo(rows, cols, levels); //get image information

    int tempVal;

    for (int r = 0; r < rows; ++r) {
        std::cout << std::endl << " | ";
        for (int c = 0; c < cols; ++c) {
            img.getPixelVal(r, c, tempVal);
            std::cout << tempVal << " | ";
        }
    }
    std::cout << std::endl;
}

//function to linearly remap values in an image between 0-255

void Helper::remapValues(ImageType & img) {
    int currMin = findMinPixelVal(img);
    int currMax = findMaxPixelVal(img);

    int rows, cols, levels;
    img.getImageInfo(rows, cols, levels); //get image information

    int currVal, newVal;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            img.getPixelVal(r, c, currVal);
            //find new value using linear transformation
            newVal = (((double) currVal - currMin) / (currMax - currMin)) * 255;
            img.setPixelVal(r, c, newVal);
        }
}

//function to apply mask at given location in image and return new value

int Helper::applyMask(const ImageType & img, const ImageType & mask, int row, int col, unsigned int maskCenterRow, unsigned int maskCenterCol, bool normalizeMask) {
    //get information about image and mask
    int imgRows, imgCols, maskRows, maskCols, levels;
    img.getImageInfo(imgRows, imgCols, levels);
    mask.getImageInfo(maskRows, maskCols, levels);

    assert(maskCenterRow <= maskRows && maskCenterCol <= maskCols); //ensure mask center is valid

    int currVal, newVal = 0;
    img.getPixelVal(row, col, currVal); //get current value

    //iterate through weights from mask and get corresponding value in image
    int weight, value, rowLoc, colLoc, rowOffset, colOffset;
    for (int i = 0; i < maskRows; ++i)
        for (int j = 0; j < maskCols; ++j) {
            mask.getPixelVal(i, j, weight); //get weight value

            //find offsets from location
            rowOffset = i - maskCenterRow;
            colOffset = j - maskCenterCol;

            //find location in image where corresponding value will be found
            rowLoc = row + rowOffset;
            colLoc = col + colOffset;

            //add weighed value if possible. Do not add anything if mask weight is outside image size
            if (rowLoc >= 0 && rowLoc < imgRows && colLoc >= 0 && colLoc < imgCols) {
                img.getPixelVal(rowLoc, colLoc, value);
                newVal += (value * weight);
            }
        }

    if (normalizeMask) {
        newVal /= getSumOfPixels(mask);
    }

    return newVal;
}