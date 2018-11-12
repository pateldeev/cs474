#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "Image.h"
#include "FFT/fft2D.c"

namespace Helper {

    //function to print pixel values. Useful for debugging
    void printPixelValues(const ImageType & imgR, const ImageType & imgI);

    //function to linearly remap values in an image to 8-bit range (0-255)
    void remapValues(ImageType & img);

    //function that applies mask to image at location specified by row and col and returns the new value. The maskCenterRow and maskCenterCol specify the center of the mask.
    double spatialConvolution(const ImageType & img, const ImageType & mask, int row, int col, unsigned int maskCenterRow, unsigned int maskCenterCol, bool normalizeMask = true);

    enum GaussianFilterSize {
        small = 7,
        large = 15
    };

    //function to apply Gaussian filtering in the spatial domain
    void applyGaussianSpatial(ImageType & img, const GaussianFilterSize & size);

    //function to apply 2D fft to ImageType variables. 
    //Note the function internal shifts the magnitude to center and back
    void applyFFT2D(ImageType & imgR, ImageType & imgI, bool forward = true);

    //function to return spectrum of image - applies log transformation: log(1+val)
    void getSpectrum(ImageType & spectrum, const ImageType & imgR, const ImageType & imgI);
};

#endif
