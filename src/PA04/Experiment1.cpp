#include "PA04/HelperFunctions.h"
#include "ReadWrite.h"
#include "PA02/HelperFunctions.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

//function to change the values in a specific row of an image over the given range of columns

void changeRow(ImageType & imgR, ImageType & imgI, int row, int colMin, int colMax, int newValR = 0, int newValI = 0) {
    for (int c = colMin; c <= colMax; ++c) {
        imgR.setPixelVal(row, c, newValR);
        imgI.setPixelVal(row, c, newValI);
    }
}

//function to change the values in a specific column of an image over the given range of rows

void changeCol(ImageType & imgR, ImageType & imgI, int col, int rowMin, int rowMax, int newValR = 0, int newValI = 0) {
    for (int r = rowMin; r <= rowMax; ++r) {
        imgR.setPixelVal(r, col, newValR);
        imgI.setPixelVal(r, col, newValI);
    }
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

    //apply forward FFT and get and save spectrum
    Helper::applyFFT2D(imgR, imgI);
    ImageType imgSpectrum(imgRows, imgCols, Q);
    Helper::getSpectrum(imgSpectrum, imgR, imgI);
    ImageType spectrumOriginal(imgSpectrum);
    Helper::remapValues(spectrumOriginal);
    writeImage("images/PA04/Experiment1/boy_noisy_spectrum.pgm", spectrumOriginal);

    //black out areas of rows were noise is prevalent
    int cutOffBottom = 235, cutOffTop = 278;
    std::vector<int> remove = {241, 240, 254, 255, 256, 257, 271, 272};
    for (int r : remove) {
        changeRow(imgSpectrum, imgSpectrum, r, 0, cutOffBottom);
        changeRow(imgSpectrum, imgSpectrum, r, cutOffTop, imgRows - 1);
        changeRow(imgR, imgI, r, 0, cutOffBottom);
        changeRow(imgR, imgI, r, cutOffTop, imgRows - 1);
    }

    //black out areas of columns were noise is prevalent
    remove = {253, 254, 255, 256, 257, 258};
    for (int c : remove) {
        changeCol(imgSpectrum, imgSpectrum, c, 0, cutOffBottom);
        changeCol(imgSpectrum, imgSpectrum, c, cutOffTop, imgCols - 1);
        changeCol(imgR, imgI, c, 0, cutOffBottom);
        changeCol(imgR, imgI, c, cutOffTop, imgCols - 1);
    }

    //display filtered spectrum for visualization
    ImageType spectrumFiltered(imgSpectrum);
    Helper::remapValues(spectrumFiltered);
    writeImage("images/PA04/Experiment1/boy_filtered_spectrum.pgm", spectrumFiltered);

    //apply inverse FFT and save filtered image
    Helper::applyFFT2D(imgR, imgI, false);
    Helper::remapValues(imgR);
    writeImage("images/PA04/Experiment1/boy_filtered.pgm", imgR);

    //apply 7x7 Gaussian mask in spatial domain to original and save result as required
    Helper::GaussianFilterSize filterSize;
    ImageType img_7x7Gaussian(img);
    filterSize = Helper::GaussianFilterSize::small;
    Helper::applyGaussianSpatial(img_7x7Gaussian, filterSize);
    writeImage("images/PA04/Experiment1/boy_7x7Gaussian.pgm", img_7x7Gaussian);

    //apply 15x15 Gaussian mask in spatial domain to original and save result as required
    ImageType img_15x15Gaussian(img);
    filterSize = Helper::GaussianFilterSize::large;
    Helper::applyGaussianSpatial(img_15x15Gaussian, filterSize);
    writeImage("images/PA04/Experiment1/boy_15x15Gaussian.pgm", img_15x15Gaussian);

    return 0; //done
}