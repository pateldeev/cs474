#include <iostream>
#include <fstream>

#include "Image.h"
#include "ReadWrite.h"

//calculates histogram representing frequency of each gray scale value
int * computeHistogram(const ImageType & img);

//normalizes frequencies by dividing by total
double * computeNormalized(const int * histogram, int size, int samples);

//computes CDF by making each term the sum of itself and the previous one
double * computeCDF(const double * data, int size);

//equalizes the image using the given hisotgramCDF
void equalize(ImageType & img, const double * histogramCDF);

int main(int argc, char * argv[]) {

    if (argc != 3) {
        std::cerr << std::endl << "Incorrect number of arguments. Must provide name of input image and name for output image to be saved under " << std::endl;
        return -1;
    }

    std::cout << std::endl << "Reading input image: " << argv[1] << std::endl;
    int M, N, Q;
    bool type;
    readImageHeader(argv[1], N, M, Q, type); //read image header
    ImageType img(N, M, Q); //allocate memory for the image array
    readImage(argv[1], img); //read image

    std::cout << std::endl << "Done reading input. Now computing histogram" << std::endl;
    int * histogram = computeHistogram(img); //compute histogram

    std::cout << std::endl << "Now normalizing histogram and creating CDF" << std::endl;
    double * normalizedHistogram = computeNormalized(histogram, Q + 1, M * N); //create normalized histogram

    std::cout << std::endl << "Now creating CDF of normalized histogram" << std::endl;
    double * histogramCDF = computeCDF(normalizedHistogram, Q + 1); //compute CDF of normalized histogram

    std::cout << std::endl << "Now equalizing histogram" << std::endl;
    equalize(img, histogramCDF); //equalize histogram

    std::cout << std::endl << "Now saving equalized histogram as output image: " << argv[2] << std::endl;
    writeImage(argv[2], img);

    std::cout << std::endl << "Output saved! Doing memory cleanup and exiting." << std::endl;
    delete histogram;
    histogram = nullptr;
    delete normalizedHistogram;
    normalizedHistogram = nullptr;
    delete histogramCDF;
    histogramCDF = nullptr;

    return 0;
}

int * computeHistogram(const ImageType & img) {

    int numRows, numCols, maxVal;
    img.getImageInfo(numRows, numCols, maxVal);

    //create histogram array and initialize to zero
    int * histogram = new int[maxVal + 1];
    for (int i = 0; i <= maxVal; ++i) {
        histogram[i] = 0;
    }

    int tempVal;
    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < numCols; ++c) {
            img.getPixelVal(r, c, tempVal);
            ++histogram[tempVal];
        }
    }

    return histogram;
}

double * computeNormalized(const int * histogram, int size, int samples) {

    double * normalized = new double[size];

    for (int i = 0; i < size; ++i) {
        normalized[i] = (double) histogram[i] / samples;
    }

    return normalized;
}

double * computeCDF(const double * data, int size) {

    double * cdf = new double[size];

    cdf[0] = data[0];
    for (int i = 1; i < size; ++i) {
        cdf[i] = cdf[i - 1] + data[i];
    }

    return cdf;
}

void equalize(ImageType & img, const double * histogramCDF) {
    int numRows, numCols, maxVal;
    img.getImageInfo(numRows, numCols, maxVal);

    int currVal;
    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < numCols; ++c) {
            img.getPixelVal(r, c, currVal);
            img.setPixelVal(r, c, (int) (histogramCDF[currVal] * maxVal));
        }
    }
}