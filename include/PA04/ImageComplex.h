#ifndef IMAGECOMPLEX_H
#define IMAGECOMPLEX_H

#include "Image.h"

class ImageComplex {
public:

    //Constructs image to hold specified number of values
    ImageComplex(int rows, int cols);

    //Constructs image by copying over values from existing image.
    ImageComplex(const ImageComplex & other);

    //Constructs image from pixel values from ImageType variables.
    //imgR and imgI must have same size.
    ImageComplex(const ImageType & imgR, const ImageType & imgI);

    //Deallocate memory holding pixel data.
    ~ImageComplex(void);

    //Copies over pixel values - rhs must have same size of image.
    ImageComplex& operator=(const ImageComplex & rhs);

    //Adds pixel values of other image - other must have same size.
    void operator+=(const ImageComplex & other);

    //Adds constant to real parts - needed for Wiener filtering.
    ImageComplex operator+(float valReal) const;

    //Get size of image.
    void getImageInfo(int & rows, int & cols) const;

    //Copy pixel data to ImageType variables. 
    //All non integer values are rounded down.
    //imgR and imgI must have same size as image.
    void getImageType(ImageType & imgR, ImageType & imgI, bool normalize = true) const;

    //Get spectrum of image as ImageType variable.
    //Applies log transformation: log(1+val).
    //spectrum must have same size as image.
    void getSpectrum(ImageType & spectrum, bool normalize = true) const;

    //Get real and imaginary value at specific pixel.
    void getPixelVal(int row, int col, float & re, float & im) const;

    //Set real and imaginary value at specific pixel.
    void setPixelVal(int row, int col, float re, float im);

    //Apply 2D FFT to image. Note the function internally shifts the magnitude.
    void applyFFT(bool forward = true);

    //Compute power spectrum of image. Useful for Wiener filtering.
    void powerSpectrum(void);

    //Apply point-by-point complex multiplication.
    //rhs must have same size as image.
    //Negative cutOffRadius means to multiply entire spectrum.
    void complexMultiplication(const ImageComplex & rhs, const float cutoffR = -1);

    //Compute complex multiplicative inverse. Needed to do division.
    //Returns calling image for chaining with multiplication.
    ImageComplex& complexInverse(void);

    //Print pixel values to screen. Useful for debugging.
    //Will print to screen if fileName is empty.
    void printPixelValues(const char * fileName = nullptr) const;

private:

    int m_rows;
    int m_cols;

    float ** m_dataR; //real values
    float ** m_dataI; //imaginary value
};

#endif
