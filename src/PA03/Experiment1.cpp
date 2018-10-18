#include "ReadWrite.h"

#include "PA03/fft.h"

#include <iostream>

void partA(void);

float * generateDataPartA(void);

int main(int argc, char * argv[]) {

    partA();

    return 0;
}

//function to implement Part A of Experiment 1

void partA(void) {
    //generate required dataset. Contains imaginary parts, all 0, as well
    float * signal = generateDataPartA();

    std::cout << std::endl << "Original Data: " << " | " << signal[0] << ' ' << signal[1] << " | " << signal[2] << ' '
            << signal[3] << " | " << signal[4] << ' ' << signal[5] << " | " << signal[6] << ' ' << signal[7] << std::endl;

    fft(signal - 1, 4, -1);

    for (int i = 0; i < 8; ++i)
        signal[i] /= 4;

    fft(signal - 1, 4, 1);

    std::cout << std::endl << "Data after inverse: " << " | " << signal[0] << ' ' << signal[1] << " | " << signal[2] << ' '
            << signal[3] << " | " << signal[4] << ' ' << signal[5] << " | " << signal[06] << ' ' << signal[7] << std::endl;

    delete signal;
}

float * generateDataPartA(void) {
    float * data = new float[8];

    //real part of data
    data[0] = 2;
    data[2] = 3;
    data[4] = 4;
    data[6] = 4;

    //imaginary part of data
    data[1] = data[3] = data[5] = data[7] = 0;

    return data;
}
