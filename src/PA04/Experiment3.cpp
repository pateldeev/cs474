#include "ReadWrite.h"
#include "PA04/HelperFunctions.h"
#include "PA04/ImageComplex.h"
#include "PA04/boxmuller.c"
#include "PA02/HelperFunctions.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <assert.h>

//Function to take ComplexImage (in spatial domain) and compute the blurred image (in frequency domain).
//Uses degradation model discussed in class and in the book.
void getMotionBlurred(const ImageComplex & imgSpatial, ImageComplex & imgBlurredFrequency);

//Function to take ComplexImage (in frequency domain) and add Gaussian noise (in frequency domain).
//Uses the function given to generate noise values.
void addGaussianNoise(ImageComplex & imgFrequency, float mean, float standardDeviation);

//Function to take degraded ComplexImage (in frequency domain) and restore it (to spatial domain) via inverse filtering.
//Uses degradation model discussed in class and in the book.
void applyInverse(const ImageComplex & imgDegradedFrequency, ImageComplex & imgRestoredSpatial, float cutoffRadius = -1, float threshold = 0);

//Function to take degraded ComplexImage (in frequency domain) and restore it (to spatial domain) via wiener filtering.
//Uses model discussed in class and in the book with provided K.
void applyWiener(const ImageComplex & imgDegradedFrequency, ImageComplex & imgRestoredSpatial, float K);

int main2() {
    // Read input image
    int imgNRows, imgBRows, imgNCols, imgBCols, Q;
    bool type;
    std::string fileN = "/home/dp/Downloads/book.pgm";
    std::string fileB = "/home/dp/Downloads/bookBlur.pgm";
    readImageHeader(fileN.c_str(), imgNRows, imgNCols, Q, type);
    readImageHeader(fileB.c_str(), imgBRows, imgBCols, Q, type);
    ImageType tempN(imgNRows, imgNCols, Q), tempB(imgBRows, imgBCols, Q);
    readImage(fileN.c_str(), tempN);
    readImage(fileB.c_str(), tempB);

    ImageType imgN(512, 512, 255), imgB(512, 512, 255);

    int tempValN, tempValB;
    for (int r = 0; r < 512; ++r) {
        for (int c = 0; c < 512; ++c) {
            if (r < imgNRows && c < imgNCols)
                tempN.getPixelVal(r, c, tempValN);
            else
                tempValN = 0;

            if (r < imgBRows && c < imgBCols)
                tempB.getPixelVal(r, c, tempValB);
            else
                tempValB = 0;

            imgN.setPixelVal(r, c, tempValN);
            imgB.setPixelVal(r, c, tempValB);
        }
    }

    imgNRows = imgBRows = imgNCols = imgBRows = 512;
    writeImage("images/PA04/Experiment3/testN.pgm", imgN);
    writeImage("images/PA04/Experiment3/testB.pgm", imgB);

    ImageType imgImaginary(imgNRows, imgNCols, Q);
    ImageComplex imgCN(imgN, imgImaginary);
    ImageComplex imgCB(imgB, imgImaginary);

    imgCN.applyFFT(true);
    imgCB.applyFFT(true);

    ImageType imgSpec(imgNCols, imgNCols, Q);
    imgCN.getSpectrum(imgSpec);
    writeImage("images/PA04/Experiment3/testNSpec.pgm", imgSpec);
    imgCB.getSpectrum(imgSpec);
    writeImage("images/PA04/Experiment3/testBSpec.pgm", imgSpec);

    float valR, valI;
    imgCN.getPixelVal(256, 0, valR, valI);
    std::cout << "N: " << valR << " +j" << valI << std::endl;
    imgCB.getPixelVal(256, 0, valR, valI);
    std::cout << "B: " << valR << " +j" << valI << std::endl;

    imgCB.complexMultiplication(imgCN.complexInverse());
    imgCB.getSpectrum(imgSpec);
    writeImage("images/PA04/Experiment3/testHSpec.pgm", imgSpec);
}

int main(int argc, char * argv[]) {
    //main parameters for run
    const std::string imgFile = "images/PA04/Experiment3/lenna.pgm";
    const float noiseMean = 0.f;
    const float noiseStandardDeviation = 10.f;
    const int inverseCutoffRadius = 20; //21;
    const float inverseThreshold = 0; //0.005;
    const float wienerK = 0.007;

    std::srand(time(nullptr));
    std::cout << std::endl << "Experiment 3" << std::endl;

#define BOOK 0
#if BOOK
    // Read input image
    int imgRows, imgCols, Q;
    bool type;
    std::string book = "/home/dp/Downloads/book.pgm"; //"/home/dp/Downloads/bookBlur.pgm";
    readImageHeader(book.c_str(), imgRows, imgCols, Q, type);
    ImageType imgTemp(imgRows, imgCols, Q);
    readImage(book.c_str(), imgTemp);

    ImageType imgSpatialR(512, 512, 255);

    int tempVal;
    for (int r = 0; r < 512; ++r) {
        for (int c = 0; c < 512; ++c) {
            if (r < imgRows && c < imgCols)
                imgTemp.getPixelVal(r, c, tempVal);
            else
                tempVal = 0;

            imgSpatialR.setPixelVal(r, c, tempVal);
        }
    }

    writeImage("images/PA04/Experiment3/book_resized.pgm", imgSpatialR);
    imgRows = imgCols = 512;
#else
    // Read input image
    int imgRows, imgCols, Q;
    bool type;
    readImageHeader(imgFile.c_str(), imgRows, imgCols, Q, type);
    ImageType imgSpatialR(imgRows, imgCols, Q);
    readImage(imgFile.c_str(), imgSpatialR);
#endif

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
    applyInverse(imgNoiseFrequency, imgRestoredInverse, inverseCutoffRadius, inverseThreshold);

    //compute and save real part of inverse restored image for visualization/verification
    ImageType imgRestoredInverseR(imgRows, imgCols, Q), imgRestoredInverseI(imgRows, imgCols, Q);
    imgRestoredInverse.getImageType(imgRestoredInverseR, imgRestoredInverseI);
    writeImage("images/PA04/Experiment3/restored_inverse.pgm", imgRestoredInverseR);


    //get spatial domain restored image from wiener filtering
    ImageComplex imgRestoredWiener(imgRows, imgCols);
    applyWiener(imgNoiseFrequency, imgRestoredWiener, wienerK);

    //compute and save real part of wiener restored image for visualization/verification
    ImageType imgRestoredWienerR(imgRows, imgCols, Q), imgRestoredWienerI(imgRows, imgCols, Q);
    imgRestoredWiener.getImageType(imgRestoredWienerR, imgRestoredWienerI);
    writeImage("images/PA04/Experiment3/restored_wiener.pgm", imgRestoredWienerR);

    return 0;
}

//Function that creates motion blur mask in frequency domain. 
//Follows given formula and makes sure mask is centered.
#if 0

void createMotionBlurFilter(ImageComplex & blurrMaskFrequency, float a = 0.1, float b = 0.1, float T = 1.0) {
    int imgRows, imgCols;
    blurrMaskFrequency.getImageInfo(imgRows, imgCols);

    float valR, valI, si, co;
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {
            int u = r - imgRows / 2, v = c - imgCols / 2; //u and v coordinates of pixel

            float x = u * a + v * b; //repeating part of formula

            if (x != 0) { //ensure we don't divide by zero
                si = std::sin(M_PI * x), co = std::cos(M_PI * x);

                //real and imaginary value according to formula
                valR = (T / (M_PI * x)) * si * co;
                valI = -(T / (M_PI * x)) * si * si;
            } else { //value due to limits of formula
                valR = 1, valI = 0;
            }

            blurrMaskFrequency.setPixelVal(r, c, valR, valI); //set new value
        }
}
#else

void createMotionBlurFilter(ImageComplex & blurrMaskFrequency) {
    int imgRows, imgCols;
    blurrMaskFrequency.getImageInfo(imgRows, imgCols);

    const float a = 0.1, b = 0.1, T = 1.0;

    float valR, valI, si, co;
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {
            int u = c - imgCols / 2, v = imgRows / 2 - r; //u and v coordinates of pixel
            float x = u * a + v * b; //repeating part of formula

            //normalize angle to 0-2 (pi)
            float an = std::fmod(x, 2);
            if (an < 0.f)
                an = 2 - std::abs(an);

            //manually set sin and cosine values for special values to prevent rounding errors
            if (an == 0.f)
                si = 0, co = 1;
            else if (an == 0.5)
                si = 1, co = 0;
            else if (an == 1.f)
                si = 0, co = -1;
            else if (an == 1.5)
                si = -1, co = 0;
            else
                si = std::sin(M_PI * an), co = std::cos(M_PI * an);

            if (x != 0.f) { //ensure we don't divide by zero
                //real and imaginary value according to formula
                valR = (T / (M_PI * x)) * si * co;
                valI = -(T / (M_PI * x)) * si * si;
            } else { //value due to limits of formula
                valR = T * co, valI = T * si;
            }

            blurrMaskFrequency.setPixelVal(r, c, valR, valI); //set new value
        }
}
#endif

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

#if 1

    int u = 5, v = 6;
    int c = u + imgCols / 2, r = imgRows / 2 - v; //u and v coordinates of pixels
    std::cout << r << "," << c;

    float valR, valI;
    blurMaskFrequency.getPixelVal(r, c, valR, valI);
    std::cout << std::endl << valR << "," << valI;

    ImageComplex test(blurMaskFrequency);
    ImageType testSpec(imgRows, imgCols, 255);
    test.test(testSpec);
    writeImage("images/PA04/Experiment3/maskSpec.pgm", testSpec);
#endif

    //apply motion blur to image in frequency domain
    img.complexMultiplication(blurMaskFrequency); //do complex multiplication
    imgBlurredFrequency = img;
}


//function to take ComplexImage (in frequency domain) and add Gaussian noise (in frequency domain)
//uses the function given to generate noise values

void addGaussianNoise(ImageComplex & imgFrequency, float mean, float standardDeviation) {
    int imgRows, imgCols;
    imgFrequency.getImageInfo(imgRows, imgCols);
    ImageComplex noiseFrequency(imgRows, imgCols);

#if 1
    ImageComplex img(imgFrequency);
    img.applyFFT(false);
    ImageType imgR(imgRows, imgCols, 255), imgI(imgRows, imgCols, 255);
    img.getImageType(imgR, imgI);
    writeImage("images/PA04/Experiment3/Noise_input.pgm", imgR);

    int tempVal;
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {
            imgR.getPixelVal(r, c, tempVal);
            tempVal += box_muller(mean, standardDeviation);
            imgR.setPixelVal(r, c, tempVal);
        }
    Helper::remapValues(imgR);

    writeImage("images/PA04/Experiment3/Noise_output.pgm", imgR);

#endif

    //generate noisy image using given values
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {

            float noiseValR = box_muller(mean, standardDeviation);
            float noiseValI = box_muller(mean, standardDeviation);
            noiseFrequency.setPixelVal(r, c, noiseValR, noiseValI);
        }

    imgFrequency += noiseFrequency; //add noise to original image
}

//function to take degraded ComplexImage (in frequency domain) and restore it (to spatial domain) via inverse filtering.
//uses degradation model discussed in class and in the book.

void applyInverse(const ImageComplex & imgDegradedFrequency, ImageComplex & imgRestoredSpatial, float cutoffRadius, float threshold) {
    int imgRows, imgCols;
    imgDegradedFrequency.getImageInfo(imgRows, imgCols);
    ImageComplex img(imgDegradedFrequency);

    //create motion blur mask in frequency domain
    ImageComplex blurMaskFrequency(imgRows, imgCols);
    createMotionBlurFilter(blurMaskFrequency);

    //divide by motion degradation mask
    img.complexMultiplication(blurMaskFrequency.complexInverse(threshold), cutoffRadius);

    //save spectrum of restored image for visualization
    ImageType imgRestoredSpectrum(imgRows, imgCols, 255);
    img.getSpectrum(imgRestoredSpectrum);
    writeImage("images/PA04/Experiment3/restored_spectrum_inverse.pgm", imgRestoredSpectrum);

    img.applyFFT(false); //apply inverse FFT to restored image to get to spatial domain
    imgRestoredSpatial = img;
}

//function to take degraded ComplexImage (in frequency domain) and restore it (to spatial domain) via wiener filtering.
//uses model discussed in class and in the book with provided K.

void applyWiener(const ImageComplex & imgDegradedFrequency, ImageComplex & imgRestoredSpatial, float K) {
    int imgRows, imgCols;
    imgDegradedFrequency.getImageInfo(imgRows, imgCols);
    ImageComplex img(imgDegradedFrequency);

    //create motion blur mask in frequency domain
    ImageComplex blurMaskFrequency(imgRows, imgCols);
    createMotionBlurFilter(blurMaskFrequency);

    //compute necessary power images
    ImageComplex blurPower(blurMaskFrequency);
    blurPower.powerSpectrum();
    ImageComplex blurPowerPlusK = blurPower + K;

    //use wiener formula as given in notes/book
    blurPower.complexMultiplication(blurPowerPlusK.complexInverse());
    img.complexMultiplication(blurPower);
    img.complexMultiplication(blurMaskFrequency.complexInverse());

    //save spectrum of restored image for visualization
    ImageType imgRestoredSpectrum(imgRows, imgCols, 255);
    img.getSpectrum(imgRestoredSpectrum);
    writeImage("images/PA04/Experiment3/restored_spectrum_wiener.pgm", imgRestoredSpectrum);

    img.applyFFT(false); //apply inverse FFT to restored image to get to spatial domain
    imgRestoredSpatial = img;
}