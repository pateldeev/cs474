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

//function to take ComplexImage (in frequency domain) and add Gaussian noise (in frequency domain)
//uses the function given to generate noise values
void addGaussianNoise(ImageComplex & imgFrequency, float mean, float standardDeviation);

//function to take blurred ComplexImage (in frequency domain) and restore it (to spatial domain) via inverse filtering.
//uses degradation model discussed in class and in the book.
void applyInverse(const ImageComplex & imgBlurredFrequency, ImageComplex & imgRestoredSpatial, int cutoffRadius = -1);

int main(int argc, char * argv[]) {
    //main parameters for run
    const std::string imgFile = "images/PA04/Experiment3/lenna.pgm";
    const float noiseMean = 0.f;
    const float noiseStandardDeviation = 0.f;
    const int inverseCutoffRadius = -1;

    std::cout << std::endl << "Experiment 3" << std::endl;
    std::srand(time(nullptr));

    // Read input image
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
    writeImage("images/PA04/Experiment3/blurred_spectrum.pgm", imgBlurredSpectrum);

    //compute and save real part of blurred image for visualization/verification
    ImageComplex imgBlurredSpatial(imgBlurredFrequency);
    imgBlurredSpatial.applyFFT(false);
    ImageType imgBlurredSpatialR(imgRows, imgCols, Q), imgBlurredSpatialI(imgRows, imgCols, Q);
    imgBlurredSpatial.getImageType(imgBlurredSpatialR, imgBlurredSpatialI);
    writeImage("images/PA04/Experiment3/blurred.pgm", imgBlurredSpatialR);

    //add noise to blurred image in frequency domain
    ImageComplex imgNoiseFrequency(imgBlurredFrequency);
    addGaussianNoise(imgNoiseFrequency, noiseMean, noiseStandardDeviation);

    //save spectrum of noisy image for visualization
    ImageType imgNoiseSpectrum(imgRows, imgCols, 255);
    imgNoiseFrequency.getSpectrum(imgNoiseSpectrum);
    writeImage("images/PA04/Experiment3/blurred_noisy_spectrum.pgm", imgNoiseSpectrum);

    //compute and save real part of noisy image for visualization/verification
    ImageComplex imgNoiseSpatial(imgNoiseFrequency);
    imgNoiseSpatial.applyFFT(false);
    ImageType imgNoiseSpatialR(imgRows, imgCols, Q), imgNoiseSpatialI(imgRows, imgCols, Q);
    imgNoiseSpatial.getImageType(imgNoiseSpatialR, imgNoiseSpatialI);
    writeImage("images/PA04/Experiment3/blurred_noisy.pgm", imgNoiseSpatialR);

    //get spatial domain restored image from inverse filtering
    ImageComplex imgRestoredInverse(imgRows, imgCols);
    applyInverse(imgNoiseFrequency, imgRestoredInverse, inverseCutoffRadius);

    //compute and save real part of restored image for visualization/verification
    ImageType imgRestoredInverseR(imgRows, imgCols, Q), imgRestoredInverseI(imgRows, imgCols, Q);
    imgRestoredInverse.getImageType(imgRestoredInverseR, imgRestoredInverseI);
    writeImage("images/PA04/Experiment3/restored_inverse.pgm", imgRestoredInverseR);

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
    writeImage("images/PA04/Experiment3/original_spectrum.pgm", imgSpectrum);

    //create motion blur mask in frequency domain
    ImageComplex blurMaskFrequency(imgRows, imgCols);
    createMotionBlurFilter(blurMaskFrequency);

    //apply motion blur to image in frequency domain
    img.complexMultiplation(blurMaskFrequency); //do complex multiplication
    imgBlurredFrequency = img;
}


//function to take ComplexImage (in frequency domain) and add Gaussian noise (in frequency domain)
//uses the function given to generate noise values

void addGaussianNoise(ImageComplex & imgFrequency, float mean, float standardDeviation) {
    int imgRows, imgCols;
    imgFrequency.getImageInfo(imgRows, imgCols);
    ImageComplex noiseFrequency(imgRows, imgCols);

    //generate noisy image using given values
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {
            float noiseVal = box_muller(mean, standardDeviation);
            noiseFrequency.setPixelVal(r, c, noiseVal, noiseVal);
        }

    imgFrequency += noiseFrequency; //add noise to original image
}

//function to take blurred ComplexImage (in frequency domain) and restore it (to spatial domain) via inverse filtering.
//uses degradation model discussed in class and in the book.

void applyInverse(const ImageComplex & imgBlurredFrequency, ImageComplex & imgRestoredSpatial, int cutoffRadius) {
    int imgRows, imgCols;
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
    writeImage("images/PA04/Experiment3/restored_spectrum.pgm", imgRestoredSpectrum);

    img.applyFFT(false); //apply inverse FFT to restored image to get to spatial domain
    imgRestoredSpatial = img;
}