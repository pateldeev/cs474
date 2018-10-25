#include "ReadWrite.h"

#include "PA03/fft.c"
#include "PA03/fft2D.c"

#include <iostream>
#include <vector>
#include <string>

int main(int argc, char * argv[]) {

    //test of 2D FFT function with 2x2 image from midterm question
    std::vector<float> imgDataReal = {0.f, 1.f, 1.f, 0.f};
    std::vector<float> imgDataImag = {0.f, 0.f, 0.f, 0.f};

    std::cout << std::endl << "Orignal Data: " << std::endl << imgDataReal[0] << "+j*" << imgDataImag[0] << "|"
            << imgDataReal[1] << "+j*" << imgDataImag[1] << std::endl << imgDataReal[2] << "+j*" << imgDataImag[2]
            << "|" << imgDataReal[3] << "+j*" << imgDataImag[3] << std::endl << std::endl;
    
    fft2D(2,2,&imgDataReal[0], &imgDataImag[0], -1); //forward 2D FFT
    
    std::cout << std::endl << "Data after FFT: " << std::endl << imgDataReal[0] << "+j*" << imgDataImag[0] << "|"
            << imgDataReal[1] << "+j*" << imgDataImag[1] << std::endl << imgDataReal[2] << "+j*" << imgDataImag[2]
            << "|" << imgDataReal[3] << "+j*" << imgDataImag[3] << std::endl << std::endl;
    
    fft2D(2,2,&imgDataReal[0], &imgDataImag[0], 1); //inverse 2D FFT
    
    std::cout << std::endl << "Data after inverse FFT: " << std::endl << imgDataReal[0] << "+j*" << imgDataImag[0] << "|"
            << imgDataReal[1] << "+j*" << imgDataImag[1] << std::endl << imgDataReal[2] << "+j*" << imgDataImag[2]
            << "|" << imgDataReal[3] << "+j*" << imgDataImag[3] << std::endl << std::endl;

    return 0;
}
