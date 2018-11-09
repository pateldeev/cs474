#include "PA04/HelperFunctions.h"

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
    int currMin = findMinPixelVal(img); //find minimum
    int currMax = findMaxPixelVal(img); //find maximum

    int rows, cols, levels;
    img.getImageInfo(rows, cols, levels); //get image information

    int currVal, newVal;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            img.getPixelVal(r, c, currVal);
            //find new value using linear transformation from class
            newVal = (((double) currVal - currMin) / (currMax - currMin)) * 255;
            img.setPixelVal(r, c, newVal); //change value
        }
}

//function to apply given mask at given location [row][col] in image and return new value
//function will treat any locations outside the image as being a 0 value

double Helper::applyMask(const ImageType & img, const ImageType & mask, int row, int col, unsigned int maskCenterRow, unsigned int maskCenterCol, bool normalizeMask) {
    //get information about image and mask
    int imgRows, imgCols, maskRows, maskCols, levels;
    img.getImageInfo(imgRows, imgCols, levels);
    mask.getImageInfo(maskRows, maskCols, levels);

    assert(maskCenterRow <= maskRows && maskCenterCol <= maskCols); //ensure mask center is valid

    int currVal;
    double newVal = 0;
    img.getPixelVal(row, col, currVal); //get current value

    //iterate through weights from mask and get corresponding value in image
    int weight, value, rowLoc, colLoc, rowOffset, colOffset;
    for (int i = 0; i < maskRows; ++i)
        for (int j = 0; j < maskCols; ++j) {
            mask.getPixelVal(i, j, weight); //get weight value

            //find offsets from location in image - used to find value corresponding to weight
            rowOffset = i - maskCenterRow;
            colOffset = j - maskCenterCol;

            //find location in image where value of weight can be found
            rowLoc = row + rowOffset;
            colLoc = col + colOffset;

            //add weighed value if possible. Do not add anything if location is outside the image
            if (rowLoc >= 0 && rowLoc < imgRows && colLoc >= 0 && colLoc < imgCols) {
                img.getPixelVal(rowLoc, colLoc, value);
                newVal += (value * weight);
            }
        }

    if (normalizeMask)
        newVal /= getSumOfPixels(mask); //divide by sum of mask if it must be normalized 

    return newVal;
}