#include "ReadWrite.h"
#include "PA04/HelperFunctions.h"
#include "PA04/ImageComplex.h"

#include <iostream>
#include <string>
#include <vector>

//function to change the values in a specific row of an image over the given range of columns

void changeRow(ImageComplex & img, int row, int colMin, int colMax, int newValR = 0, int newValI = 0) {
    for (int c = colMin; c <= colMax; ++c)
        img.setPixelVal(row, c, newValR, newValI);
}

//function to change the values in a specific column of an image over the given range of rows

void changeCol(ImageComplex & img, int col, int rowMin, int rowMax, int newValR = 0, int newValI = 0) {
    for (int r = rowMin; r <= rowMax; ++r)
        img.setPixelVal(r, col, newValR, newValI);
}

int main(int argc, char * argv[]) {
    std::cout << std::endl << "Experiment 1" << std::endl;

    // Read input image    
    const std::string imgFile = "images/PA04/Experiment1/boy_noisy.pgm";
    int imgRows, imgCols, Q;
    bool type;
    readImageHeader(imgFile.c_str(), imgRows, imgCols, Q, type);
    ImageType imgSpatial(imgRows, imgCols, Q);
    readImage(imgFile.c_str(), imgSpatial);

    //apply 7x7 Gaussian mask in spatial domain to original and save result as required
    ImageType img_7x7Gaussian(imgSpatial);
    Helper::applyGaussianSpatial(img_7x7Gaussian, Helper::GaussianFilterSize::small);
    writeImage("images/PA04/Experiment1/boy_7x7Gaussian.pgm", img_7x7Gaussian);

    //apply 15x15 Gaussian mask in spatial domain to original and save result as required
    ImageType img_15x15Gaussian(imgSpatial);
    Helper::applyGaussianSpatial(img_15x15Gaussian, Helper::GaussianFilterSize::large);
    writeImage("images/PA04/Experiment1/boy_15x15Gaussian.pgm", img_15x15Gaussian);

    //make imaginary part of image (all zeros) and create complex image
    ImageType imgI(imgRows, imgCols, Q);
    ImageComplex img(imgSpatial, imgI); //use ImageComplex class for simplicity

    img.applyFFT(true); //apply forward FFT

    //save spectrum for visualization 
    ImageType spectrum(imgRows, imgCols, Q);
    img.getSpectrum(spectrum);
    writeImage("images/PA04/Experiment1/boy_noisy_spectrum.pgm", spectrum);

    //black out areas of rows were noise is prevalent
    int cutOffBottom = 235, cutOffTop = 278;
    std::vector<int> remove = {241, 240, 271, 272};
    for (int r : remove) {
        changeRow(img, r, 0, cutOffBottom);
        changeRow(img, r, cutOffTop, imgCols - 1);
    }
    cutOffBottom = 100, cutOffTop = 411;
    remove = {255, 256};
    for (int r : remove) {
        changeRow(img, r, 0, cutOffBottom);
        changeRow(img, r, cutOffTop, imgCols - 1);
    }
    //black out areas of columns were noise is prevalent
    cutOffBottom = 235, cutOffTop = 278;
    remove = {254, 255, 256, 257};
    for (int c : remove) {
        changeCol(img, c, 0, cutOffBottom);
        changeCol(img, c, cutOffTop, imgRows - 1);
    }

    //save spectrum for visualization 
    ImageType spectrumFiltered(imgRows, imgCols, Q);
    img.getSpectrum(spectrumFiltered);
    writeImage("images/PA04/Experiment1/boy_filtered_spectrum.pgm", spectrumFiltered);

    img.applyFFT(false); //apply inverse FFT

    //save real part of filtered image
    ImageType imgFilteredR(imgRows, imgCols, Q), imgFilteredI(imgRows, imgCols, Q);
    img.getImageType(imgFilteredR, imgFilteredI);
    writeImage("images/PA04/Experiment1/boy_filtered.pgm", imgFilteredR);

    return 0; //done
}