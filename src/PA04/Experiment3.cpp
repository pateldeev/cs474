#include "ReadWrite.h"
#include "PA04/HelperFunctions.h"
#include "PA04/ImageComplex.h"
#include "PA04/boxmuller.c"

#include <iostream>
#include <string>

//function to take ComplexImage (in spatial domain) and computes the blurred image (in spatial domain).
//uses degradation model discussed in class and in the book
void getMotionBlurred(const ImageComplex & img, ImageComplex & blurredImg);

int main(int argc, char * argv[]) {

    std::cout << std::endl << "Experiment 3" << std::endl;

    // Read input image    
    const std::string imgFile = "images/PA04/Experiment3/lenna.pgm";
    int imgRows, imgCols, Q;
    bool type;
    readImageHeader(imgFile.c_str(), imgRows, imgCols, Q, type);
    ImageType imgSpatial(imgRows, imgCols, Q);
    readImage(imgFile.c_str(), imgSpatial);

    //make imaginary part of image (all zeros) and create complex image
    ImageType imgI(imgRows, imgCols, Q);
    ImageComplex img(imgSpatial, imgI); //use ImageComplex class for simplicity

    //get spatial domain representation of blurred image
    ImageComplex blurredImg(imgRows, imgCols);
    getMotionBlurred(img, blurredImg);

    return 0;
}

//function that creates motion blur mask in frequency domain. Follows given formula and makes sure mask is centered

void createMotionBlurFilter(ImageComplex & img, float a = 0.1, float b = 0.1, float T = 1.0) {
    int imgRows, imgCols;
    img.getImageInfo(imgRows, imgCols);

    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {
            int u = r - imgRows / 2, v = c - imgCols / 2; //u and v coordinates of point
            float x = M_PI * (u * a + v * b); //repeating part of motion blur formula
            float valR = 1;
            float valI = 0;
            if (x != 0) { //ensure we don't divide by zero
                valR = (T / x) * std::sin(x) * std::cos(x);
                valI = -(T / x) * std::sin(x) * std::sin(x);
            }
            img.setPixelVal(r, c, valR, valI); //set new value
        }
}

//function to take ComplexImage (in spatial domain) and computes the blurred image (in spatial domain).
//uses degradation model discussed in class and in the book

void getMotionBlurred(const ImageComplex & img, ImageComplex & blurredImg) {
    ImageComplex imgCpy(img);
    int imgRows, imgCols, Q = 255;
    imgCpy.getImageInfo(imgRows, imgCols);

    imgCpy.applyFFT(true); //apply forward FFT

    //save spectrum of original image for visualization
    ImageType imgSpectrum(imgRows, imgCols, Q);
    imgCpy.getSpectrum(imgSpectrum);
    writeImage("images/PA04/Experiment3/lenna_spectrum.pgm", imgSpectrum);

    //create motion blur mask
    ImageComplex blurMask(imgRows, imgCols);
    createMotionBlurFilter(blurMask);

    //apply motion blur to image in frequency domain
    blurredImg = imgCpy;
    blurredImg.complexMultiplation(blurMask); //do complex multiplication

    //save spectrum of blurred image for visualization
    ImageType imgBlurredSpectrum(imgRows, imgCols, Q);
    blurredImg.getSpectrum(imgBlurredSpectrum);
    writeImage("images/PA04/Experiment3/lenn_blurred_spectrum.pgm", imgBlurredSpectrum);

    blurredImg.applyFFT(false); //apply inverse FFT to blurred image to get to spatial domain

    //save real part of blurred image for visualization/verification
    ImageType imgBlurredR(imgRows, imgCols, Q), imgBlurredI(imgRows, imgCols, Q);
    blurredImg.getImageType(imgBlurredR, imgBlurredI);
    writeImage("images/PA04/Experiment3/lena_blurred.pgm", imgBlurredR);
}
