#include "ReadWrite.h"
#include "PA04/HelperFunctions.h"
#include "PA04/ImageComplex.h"
#include "PA04/boxmuller.c"

#include <iostream>
#include <string>

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

    img.applyFFT(true); //apply forward FFT

    img.applyFFT(false); //apply inverse FFT 

    //save real part of filtered image
    ImageType imgFilteredR(imgRows, imgCols, Q), imgFilteredI(imgRows, imgCols, Q);
    img.copytoImageType(imgFilteredR, imgFilteredI);
    writeImage("images/PA04/Experiment3/lena_filtered.pgm", imgFilteredR);

    return 0;
}