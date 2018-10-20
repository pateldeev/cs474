#include "ReadWrite.h"

#include "PA03/fft.c"

#include <iostream>
#include <vector>
#include <string>

//function that implements part A
void partA(void);

//function that implements part B
void partB(void);

//function that implements part C
void partC(void);

//generates required signal for part A(with 0 for imaginary part)
void generateSignalPartA(std::vector<float> & signal);

//generates required signal for part A(with 0 for imaginary part)
void generateSignalPartB(std::vector<float> & signal);

//scales signal by required factor
void scaleSignal(std::vector<float> & signal, const float scaleFactor);

//function to print signal. Prints | real + j*imaginary |
void printSignal(const std::vector<float> & signal, const std::string & startLable = "signal: ");

//function to print magnitude of signal. Prints | real + j*imaginary |
void printSignalMagnitude(const std::vector<float> & signal, const std::string & startLable = "magintude of signal: ");

int main(int argc, char * argv[]) {

    std::cout << std::endl << std::endl << "PART A|||||||||||||||||||" << std::endl;
    partA();

    std::cout << std::endl << std::endl << "PART B|||||||||||||||||||" << std::endl;
    partB();

    std::cout << std::endl << std::endl << "PART C|||||||||||||||||||" << std::endl;
    partC();

    std::cout << std::endl << std::endl;
    return 0;
}

void partA(void) {
    //generate required dataset and print it for verification
    std::vector<float> signal;
    generateSignalPartA(signal);
    printSignal(signal, "Original Signal (Real + j*Imaginary): ");

    //compute forward fft with help of given function
    fft(&signal[0] - 1, signal.size() / 2, -1);
    scaleSignal(signal, float(1) / (signal.size() / 2)); //scale by 1/N factor manually

    printSignal(signal, "FFT of Signal (Real + j*Imaginary): "); //print real and imaginary parts of transformed signal for verification
    printSignalMagnitude(signal, "FFT of Signal (Magnitude): "); //print magnitude of transformed signal for verification

    //compute inverse fft with given function
    fft(&signal[0] - 1, signal.size() / 2, 1);

    printSignal(signal, "Original after inverse (Real + j*Imaginary): "); //print of original signal again for verification
}

void partB(void) {
    std::cout << "Part B stuff";
}

void partC(void) {
    std::cout << "Part C stuff";
}

void generateSignalPartA(std::vector<float> & signal) {
    signal.clear();
    signal.resize(8);

    //real part of data
    signal[0] = 2;
    signal[2] = 3;
    signal[4] = 4;
    signal[6] = 4;

    //imaginary part of data - all zero
    signal[1] = signal[3] = signal[5] = signal[7] = 0;
}

void scaleSignal(std::vector<float> & signal, const float scaleFactor) {
    for (float & val : signal)
        val *= scaleFactor;
}

void printSignal(const std::vector<float> & signal, const std::string & startLable) {
    std::cout << std::endl << startLable << " [";
    for (unsigned int i = 0; i < signal.size();)
        std::cout << ' ' << signal[i++] << "+j*" << signal[i++] << ' ';
    std::cout << ']' << std::endl;
}

void printSignalMagnitude(const std::vector<float> & signal, const std::string & startLable) {
    std::cout << std::endl << startLable << " [";
    for (unsigned int i = 0; i < signal.size(); i += 2)
        std::cout << ' ' << std::sqrt(signal[i] * signal[i] + signal[i + 1] * signal[i + 1]) << ' ';
    std::cout << ']' << std::endl;
}

void generateSignalPartB(std::vector<float> & signal) {
    //generate sign wave
}
