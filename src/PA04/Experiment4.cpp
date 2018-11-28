#include "ReadWrite.h"
#include "PA04/HelperFunctions.h"
#include "PA04/ImageComplex.h"
#include "FFT/fft2D.c"

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

int main(int argc, char * argv[]) {

    const std::string imgFile = "images/PA04/Experiment4/girl.pgm";
    const std::string exp_image_real = "images/PA04/Experiment4/homomorphic_girl.pgm";
    //----------------------------------------------------------------------------
    // Start reading image in
    int M, N, Q;
    bool type;
    readImageHeader(imgFile.c_str(), N, M, Q, type);
    ImageType og_image(N, M, Q);
    readImage(imgFile.c_str(), og_image);
    //----------------------------------------------------------------------------

    // Create imaginary part of image - zero by default
    ImageType og_imaginary(N, M, Q);

    // Use ImageComplex for simplicity since
    // logarithmic functions will work on it
    ImageComplex ln_image(og_image, og_imaginary);

    // Natural log the original image
    float tempR, tempI;
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; j++) {
            ln_image.getPixelVal(i, j, tempR, tempI);
            ln_image.setPixelVal(i, j, std::log(tempR), tempI);
        }

    // Forward Fourier Transform
    ln_image.applyFFT(true);

    //display spectrum
    ImageType imgSpec(N, M, Q);
    ln_image.getSpectrum(imgSpec);
    writeImage("images/PA04/Experiment4/ln_spectrum.pgm", imgSpec);

    // High Pass Filter
    const float y_low = 0.5, y_high = 1.5, d_cutoff = 1.8, c = 1;
    ImageComplex filter(N, M);
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; j++) {
            int u = j - M / 2, v = N / 2 - i; //u and v coordinates of pixel
            tempR = 1 - std::exp(-c * ((u * u + v * v) / (d_cutoff * d_cutoff)));
            tempR *= (y_high - y_low);
            tempR += y_low;
            filter.setPixelVal(i, j, tempR, 0);
        }

    //multiply by filter
    ln_image.complexMultiplication(filter);

    //display spectrum
    ln_image.getSpectrum(imgSpec);
    writeImage("images/PA04/Experiment4/filtered_spectrum.pgm", imgSpec);


    // Inverse Fourier Transform
    ln_image.applyFFT(false);

    // Apply exponential function
    ImageType modified_real(N, M, Q), modified_imaginary(N, M, Q);
    ImageComplex exp_image(N, M);
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; j++) {
            ln_image.getPixelVal(i, j, tempR, tempI);
            exp_image.setPixelVal(i, j, std::exp(tempR), tempI);
        }

    // Change type back to ImageType
    exp_image.getImageType(modified_real, modified_imaginary);

    writeImage(exp_image_real.c_str(), modified_real);

    return 0;
}



/* Useless Code

        // Normalize
        float minVal, maxVal, tempVal, tempVal_I;
        ln_image.getPixelVal(0, 0, minVal, tempVal_I);
        maxVal = minVal;
        for(int i = 0; i < M; ++i){
                for(int j = 0; j < N; j++){
                        ln_image.getPixelVal(i, j, tempVal, tempVal_I);
                        if(tempVal < minVal){
                                minVal = tempVal;
                        }
                        if(tempVal > maxVal){
                                maxVal = tempVal;
                        }
                }
        }
        std::cout << "Max value: " << maxVal << "\nMin value: " << minVal << std::endl;

        for(int i = 0; i < M; ++i){
                for(int j = 0; j < N; j++){
                        ln_image.getPixelVal(i, j, tempVal, tempVal_I);
                        tempVal = ((tempVal - minVal) / (maxVal - minVal) * log(255));
                        ln_image.setPixelVal(i, j, tempVal, tempVal_I);
                        if(i < 3 && j < 3){
                                std::cout << tempVal << " ";
                        }
                }
        }
        std::cout << std::endl;

 */