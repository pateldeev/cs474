#include "ReadWrite.h"
#include "PA04/HelperFunctions.h"
#include "PA04/ImageComplex.h"
#include "PA04/boxmuller.c"

#include <iostream>
#include <string>

#include <vector>

//function to take ComplexImage (in spatial domain) and compute the blurred image (in frequency domain).
//uses degradation model discussed in class and in the book.
void getMotionBlurred(const ImageComplex & imgSpatial, ImageComplex & imgBlurredFrequency);

//function to take blurred ComplexImage (in frequency domain) and restore it (to spatial domain) via inverse filtering.
//uses degradation model discussed in class and in the book.
void applyInverse(const ImageComplex & imgBlurredFrequency, ImageComplex & imgRestoredSpatial, const int cutoffRadius = 40);

int main(int argc, char * argv[]) {

    std::cout << std::endl << "Experiment 3" << std::endl;

    // Read input image - lenna image
    const std::string imgFile = "images/PA04/Experiment3/lenna.pgm";
    int imgRows, imgCols, Q;
    bool type;
    readImageHeader(imgFile.c_str(), imgRows, imgCols, Q, type);
    ImageType imgSpatialR(imgRows, imgCols, Q);
    readImage(imgFile.c_str(), imgSpatialR);

    //make imaginary part of image (all zeros) and convert to ComplexImage for simplicity
    ImageType imgSpatialI(imgRows, imgCols, Q);
    ImageComplex imgSpatial(imgSpatialR, imgSpatialI);

    //get blurred image in frequency domain
    ImageComplex imgBlurredFrequency(imgRows, imgCols);
    getMotionBlurred(imgSpatial, imgBlurredFrequency);

    //save spectrum of blurred image for visualization
    ImageType imgBlurredSpectrum(imgRows, imgCols, Q);
    imgBlurredFrequency.getSpectrum(imgBlurredSpectrum);
    writeImage("images/PA04/Experiment3/lenna_blurred_spectrum.pgm", imgBlurredSpectrum);

    //compute and save real part of blurred image for visualization/verification
    ImageComplex imgBlurredSpatial(imgBlurredFrequency);
    imgBlurredSpatial.applyFFT(false);
    ImageType imgBlurredSpatialR(imgRows, imgCols, Q), imgBlurredSpatialI(imgRows, imgCols, Q);
    imgBlurredSpatial.getImageType(imgBlurredSpatialR, imgBlurredSpatialI);
    writeImage("images/PA04/Experiment3/lenna_blurred.pgm", imgBlurredSpatialR);

    //get spatial domain restored image from inverse filtering
    ImageComplex imgRestoredInverse(imgRows, imgCols);
    applyInverse(imgBlurredFrequency, imgRestoredInverse, -1);

    //compute and save real part of restored image for visualization/verification
    ImageType imgRestoredInverseR(imgRows, imgCols, Q), imgRestoredInverseI(imgRows, imgCols, Q);
    imgRestoredInverse.getImageType(imgRestoredInverseR, imgRestoredInverseI);
    writeImage("images/PA04/Experiment3/lenna_restored_inverse.pgm", imgRestoredInverseR);

    return 0;
}

//function that creates motion blur mask in frequency domain. Follows given formula and makes sure mask is centered

void createMotionBlurFilter(ImageComplex & blurrMaskFrequency, float a = 0.1, float b = 0.1, float T = 1.0) {
    int imgRows, imgCols;
    blurrMaskFrequency.getImageInfo(imgRows, imgCols);

    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {
            int u = r - imgRows / 2, v = c - imgCols / 2; //u and v coordinates of point
            float x = M_PI * (u * a + v * b); //repeating part of motion blur formula
            float valR = 1.f;
            float valI = 0.f;
            if (x != 0.f) { //ensure we don't divide by zero
                valR = (T / x) * std::sin(x) * std::cos(x);
                valI = -(T / x) * std::sin(x) * std::sin(x);
            }
            blurrMaskFrequency.setPixelVal(r, c, valR, valI); //set new value
        }
}

//function to take ComplexImage (in spatial domain) and compute the blurred image (in frequency domain).
//uses degradation model discussed in class and in the book.

void getMotionBlurred(const ImageComplex & imgSpatial, ImageComplex & imgBlurredFrequency) {
    int imgRows, imgCols;
    imgSpatial.getImageInfo(imgRows, imgCols);
    ImageComplex img(imgSpatial);

    img.applyFFT(true); //apply forward FFT

    //save spectrum of original image for visualization
    ImageType imgSpectrum(imgRows, imgCols, 255);
    img.getSpectrum(imgSpectrum);
    writeImage("images/PA04/Experiment3/lenna_spectrum.pgm", imgSpectrum);

    //create motion blur mask in frequency domain
    ImageComplex blurMaskFrequency(imgRows, imgCols);
    createMotionBlurFilter(blurMaskFrequency);

    //apply motion blur to image in frequency domain
    img.complexMultiplation(blurMaskFrequency); //do complex multiplication
    imgBlurredFrequency = img;
}


//function to take blurred ComplexImage (in frequency domain) and restore it (to spatial domain) via inverse filtering.
//uses degradation model discussed in class and in the book.

void applyInverse(const ImageComplex & imgBlurredFrequency, ImageComplex & imgRestoredSpatial, const int cutoffRadius) {
    int imgRows, imgCols, Q = 255;
    imgBlurredFrequency.getImageInfo(imgRows, imgCols);
    ImageComplex img(imgBlurredFrequency);

    //create motion blur mask in frequency domain
    ImageComplex blurMaskFrequency(imgRows, imgCols);
    createMotionBlurFilter(blurMaskFrequency);

    //invert motion blur mask
    blurMaskFrequency.complexInverse();

    //multiply by inverse inside cutoff
    img.complexMultiplation(blurMaskFrequency, cutoffRadius);

    //save spectrum of restored image for visualization
    ImageType imgRestoredSpectrum(imgRows, imgCols, 255);
    img.getSpectrum(imgRestoredSpectrum);
    writeImage("images/PA04/Experiment3/lenna_restored_spectrum.pgm", imgRestoredSpectrum);

    img.applyFFT(false); //apply inverse FFT to restored image to get to spatial domain
    imgRestoredSpatial = img;
}