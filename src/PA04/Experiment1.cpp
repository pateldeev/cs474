#include "PA04/HelperFunctions.h"
#include "ReadWrite.h"
#include "PA02/HelperFunctions.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

void changeRow(ImageType & img, int row, int colMin, int colMax, int newVal = 0) {
    for (int c = colMin; c <= colMax; ++c)
        if (c < 239 || c > 273)
            img.setPixelVal(row, c, newVal);
}

void changeCol(ImageType & img, int col, int rowMin, int rowMax, int newVal = 0) {
    for (int r = rowMin; r <= rowMax; ++r)
        if (r < 239 || r > 273)
            img.setPixelVal(r, col, newVal);
}

int main(int argc, char * argv[]) {
    std::cout << std::endl << "Experiment 1" << std::endl;

    // Read input image    
    const std::string imgFile = "images/PA04/Experiment1/boy_noisy.pgm";
    int imgRows, imgCols, Q;
    bool type;
    readImageHeader(imgFile.c_str(), imgRows, imgCols, Q, type);
    ImageType img(imgRows, imgCols, Q);
    readImage(imgFile.c_str(), img);

    //create imaginary and real part of image - imaginary is just 0
    ImageType imgR(img);
    ImageType imgI(imgRows, imgCols, Q);
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c)
            imgI.setPixelVal(r, c, 0);

    Helper::applyFFT2D(imgR, imgI); //apply forward FFT

    //get spectrum
    ImageType imgSpectrum(imgRows, imgCols, Q);
    Helper::getSpectrum(imgSpectrum, imgR, imgI);

    changeRow(imgSpectrum, 240, 0, 511);
    changeRow(imgR, 240, 0, 511);
    changeRow(imgI, 240, 0, 511);

    changeRow(imgSpectrum, 256, 0, 511);
    changeRow(imgR, 256, 0, 511);
    changeRow(imgI, 256, 0, 511);

    changeRow(imgSpectrum, 272, 0, 511);
    changeRow(imgR, 272, 0, 511);
    changeRow(imgI, 272, 0, 511);

    changeCol(imgSpectrum, 256, 0, 511);
    changeCol(imgR, 256, 0, 511);
    changeCol(imgI, 256, 0, 511);

    //display spectrum for visualization
    ImageType dispSpectrum(imgSpectrum);
    Helper::remapValues(dispSpectrum);
    writeImage("images/PA04/Experiment1/boy_spectrum.pgm", dispSpectrum);


    Helper::applyFFT2D(imgR, imgI, false); //apply inverse FFT
    Helper::remapValues(imgR);
    writeImage("images/PA04/Experiment1/boy_filtered.pgm", imgR);



    Helper::GaussianFilterSize filterSize;
    //apply 7x7 Gaussian mask in spatial domain and save result as required
    ImageType img_7x7Gaussian(img);
    filterSize = Helper::GaussianFilterSize::small;
    Helper::applyGaussianSpatial(img_7x7Gaussian, filterSize);
    writeImage("images/PA04/Experiment1/boy_7x7Gaussian.pgm", img_7x7Gaussian);

    //apply 15x15 Gaussian mask in spatial domain and save result as required
    ImageType img_15x15Gaussian(img);
    filterSize = Helper::GaussianFilterSize::large;
    Helper::applyGaussianSpatial(img_15x15Gaussian, filterSize);
    writeImage("images/PA04/Experiment1/boy_15x15Gaussian.pgm", img_15x15Gaussian);

    return 0;
}