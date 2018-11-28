#ifndef IMAGECOMPLEX_H
#define IMAGECOMPLEX_H

#include "Image.h"

//Like ImageType, but able to hold real and imaginary parts. Also stores floating point values instead of integers. Useful for frequency domain image representations

class ImageComplex {
public:
    ImageComplex(int rows, int cols);
    ImageComplex(const ImageComplex & other);
    ImageComplex& operator=(const ImageComplex & rhs);
    ImageComplex(const ImageType & imgR, const ImageType & imgI); //creates complex image from ImageType variables
    ~ImageComplex(void);

    void getImageInfo(int & rows, int & cols) const;

    void setPixelVal(int row, int col, float valR, float valI);
    void getPixelVal(int row, int col, float & valR, float & valI) const;

    //adds pixel values of two images
    void operator+=(const ImageComplex & other);

    //adds constant value to image - needed for wiener filtering
    ImageComplex operator+(float val) const;

    //function to copy data to ImageType variable. All non integer values are rounded down
    void getImageType(ImageType & imgR, ImageType & imgI, bool normalize = true) const;

    //function to get spectrum of image as ImageType variable - applies log transformation: log(1+val)
    void getSpectrum(ImageType & spectrum, bool normalize = true) const;

    //function to apply 2D FFT to image. Note the function internally shifts the magnitude
    void applyFFT(bool forward = true);

    //function to compute power spectrum of image - useful for Wiener filtering
    void powerSpectrum(void);

    //function to apply point by point complex multiplication
    //negative cutOffRadius means to multiple entire spectrum
    void complexMultiplication(const ImageComplex & rhs, const float cutoffRadius = -1);

    //function to compute complex multiplicative inverse - needed to do division for inverse filtering
    ImageComplex& complexInverse(const float threshold = 0.f);

    //function to print pixel values. Useful for debugging
    void printPixelValues(void) const;

    void test(ImageType & spectrum) const;

private:
    int m_rows;
    int m_cols;

    float ** m_dataR; //real values
    float ** m_dataI; //imaginary value
};

#endif
