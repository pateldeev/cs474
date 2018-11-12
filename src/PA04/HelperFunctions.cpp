#include "PA04/HelperFunctions.h"

#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>

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

        //find the minimum value
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

    void generateGaussianSpatialMask(ImageType & mask, const Helper::GaussianFilterSize & size) {
        int rows, cols, levels;
        mask.getImageInfo(rows, cols, levels);
        assert(rows == cols && rows == size); //ensure mask is properly size

        std::vector<std::vector<int>> maskVals;
        if (size == Helper::GaussianFilterSize::small)
            // Hard coding in 7x7 Gaussian mask
            maskVals = {
                {1, 1, 2, 2, 2, 1, 1},
                {1, 2, 2, 4, 2, 2, 1},
                {2, 2, 4, 8, 4, 2, 2},
                {2, 4, 8, 16, 8, 4, 2},
                {2, 2, 4, 8, 4, 2, 2},
                {1, 2, 2, 4, 2, 2, 1},
                {1, 1, 2, 2, 2, 1, 1}
            };
        else
            // Hard coding in 15x15 Gaussian mask
            maskVals = {
                {2, 2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2},
                {2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2},
                {3, 4, 6, 7, 9, 10, 10, 11, 10, 10, 9, 7, 6, 4, 3},
                {4, 5, 7, 9, 10, 12, 13, 13, 13, 12, 10, 9, 7, 5, 4},
                {5, 7, 9, 11, 13, 14, 15, 16, 15, 14, 13, 11, 9, 7, 5},
                {5, 7, 10, 12, 14, 16, 17, 18, 17, 16, 14, 12, 10, 7, 5},
                {6, 8, 10, 13, 15, 17, 19, 19, 19, 17, 15, 13, 10, 8, 6},
                {6, 8, 11, 13, 16, 18, 19, 20, 19, 18, 16, 13, 11, 8, 6},
                {6, 8, 10, 13, 15, 17, 19, 19, 19, 17, 15, 13, 10, 8, 6},
                {5, 7, 10, 12, 14, 16, 17, 18, 17, 16, 14, 12, 10, 7, 5},
                {5, 7, 9, 11, 13, 14, 15, 16, 15, 14, 13, 11, 9, 7, 5},
                {4, 5, 7, 9, 10, 12, 13, 13, 13, 12, 10, 9, 7, 5, 4},
                {3, 4, 6, 7, 9, 10, 10, 11, 10, 10, 9, 7, 6, 4, 3},
                {2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2},
                {2, 2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2}
            };

        //set mask valuess
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                mask.setPixelVal(r, c, maskVals[r][c]);

    }
};

//function to print pixel values. Useful for debugging

void Helper::printPixelValues(const ImageType & imgR, const ImageType & imgI) {
    //get image information an assure everything is in order
    int rowsR, rowsI, colsI, colsR, levels;
    imgR.getImageInfo(rowsR, colsR, levels);
    imgI.getImageInfo(rowsI, colsI, levels);
    assert(rowsR == rowsI && colsR == colsI);

    int tempR, tempI;
    for (int r = 0; r < rowsR; ++r) {
        std::cout << std::endl << " | ";
        for (int c = 0; c < colsR; ++c) {
            imgR.getPixelVal(r, c, tempR);
            imgI.getPixelVal(r, c, tempI);
            std::cout << tempR << "+j" << tempI << " | ";
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

double Helper::spatialConvolution(const ImageType & img, const ImageType & mask, int row, int col, unsigned int maskCenterRow, unsigned int maskCenterCol, bool normalizeMask) {
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

//function to apply Gaussian filtering in the spatial domain

void Helper::applyGaussianSpatial(ImageType & img, const GaussianFilterSize & size) {
    int imgRows, imgCols, levels;
    img.getImageInfo(imgRows, imgCols, levels); //get image info

    //create Gaussian mask
    ImageType mask(size, size, levels);
    generateGaussianSpatialMask(mask, size);
    int maskCenter = size / 2;

    //apply mask at each location
    double newVal;
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {
            newVal = spatialConvolution(img, mask, r, c, maskCenter, maskCenter);
            img.setPixelVal(r, c, (int) newVal);
        }

    remapValues(img); //remap value for visualization
}

//function to apply 2D fft to ImageType variables 
//Note the function internal shifts the magnitude to center and back

void Helper::applyFFT2D(ImageType & imgR, ImageType & imgI, bool forward) {
    //get image information an assure everything is in order
    int rowsR, rowsI, colsI, colsR, levels;
    imgR.getImageInfo(rowsR, colsR, levels);
    imgI.getImageInfo(rowsI, colsI, levels);
    assert(rowsR == rowsI && colsR == colsI);

    //move data into usable form - make sure to extend data to power of 2 for FFT
    int extendedR = pow(2, ceil(log(rowsR) / log(2)));
    int extendedC = pow(2, ceil(log(colsR) / log(2)));
    std::vector<float> dataR(extendedR * extendedC), dataI(extendedR * extendedC);
    int tempR, tempI;
    for (int r = 0; r < rowsR; ++r)
        for (int c = 0; c < colsR; ++c) {
            imgR.getPixelVal(r, c, tempR);
            dataR[r * extendedR + c] = (float) tempR * (((r + c) % 2) ? 1 : -1);
            imgI.getPixelVal(r, c, tempI);
            dataI[r * extendedR + c] = (float) tempI * (((r + c) % 2) ? 1 : -1);
        }

    //apply FFT
    fft2D(extendedR, extendedC, &dataR[0], &dataI[0], (forward) ? -1 : 1);

    //save new frequency domain values in original images. Make sure to invert shifting transformation
    int newValR, newValI;
    for (int r = 0; r < rowsR; ++r)
        for (int c = 0; c < colsR; ++c) {
            newValR = (int) dataR[r * extendedR + c] * (((r + c) % 2) ? 1 : -1);
            newValI = (int) dataI[r * extendedR + c] * (((r + c) % 2) ? 1 : -1);
            imgR.setPixelVal(r, c, newValR);
            imgI.setPixelVal(r, c, newValI);
        }
}

//function to return spectrum of image - applies log transformation: log(1+val)

void Helper::getSpectrum(ImageType & spectrum, const ImageType & imgR, const ImageType & imgI) {
    //get image information an assure everything is in order
    int rowsR, rowsI, rowsS, colsI, colsR, colsS, levels;
    imgR.getImageInfo(rowsR, colsR, levels);
    imgI.getImageInfo(rowsI, colsI, levels);
    spectrum.getImageInfo(rowsS, colsS, levels);
    assert(rowsR == rowsI && colsR == colsI && rowsR == rowsS && colsR == colsS);

    //function to get spectrum value - applies log transformation
    auto getNewVal = [](int real, int imaginary) ->int {
        int val = (int) std::sqrt((long) real * real + (long) imaginary * imaginary);
        return (int) log(1 + val);
        return val;
    };

    //find each value of spectrum
    int currentR, currentI, newVal;
    for (int r = 0; r < rowsR; ++r)
        for (int c = 0; c < colsR; ++c) {
            imgR.getPixelVal(r, c, currentR);
            imgI.getPixelVal(r, c, currentI);
            spectrum.setPixelVal(r, c, getNewVal(currentR, currentI));
        }
}
