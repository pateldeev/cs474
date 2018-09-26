#include "PA02/HelperFunctions.h"

#include <assert.h>

//private helper functions
namespace {

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

void Helper::applyMask(ImageType & img, const ImageType & mask, int row, int col, unsigned int maskCenterRow, unsigned int maskCenterCol) {
    //get information about image and mask
    int imgRows, imgCols, maskRows, maskCols, levels;
    img.getImageInfo(imgRows, imgCols, levels);
    mask.getImageInfo(maskRows, maskCols, levels);
    
    assert(maskCenterRow <= imgRows && maskCenterCol <= imgCols);
    
    int currVal, newVal;
    img.getImageInfo(row, col, currVal);
    
    int weight;
    for(int i = 0; i < maskRows; ++i)
        for(int j = 0; j <maskCols; ++j){
            mask.getPixelVal(i,j,weight);
            
            
        }
    
}