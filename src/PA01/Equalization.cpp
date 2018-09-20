#include <iostream>
#include <fstream>

#include <iomanip>

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

//calculates and returns unnormalized histogram representing the frequency of each gray scale value

int * computeHistogram(const ImageType & img) {

    int numRows, numCols, maxVal;
    img.getImageInfo(numRows, numCols, maxVal); //get details of image

    //create histogram array and initialize to zero
    int * histogram = new int[maxVal + 1];
    for (int i = 0; i <= maxVal; ++i)
        histogram[i] = 0;

    int tempVal;
    //iterate through every pixel
    for (int r = 0; r < numRows; ++r)
        for (int c = 0; c < numCols; ++c) {
            img.getPixelVal(r, c, tempVal); //read pizel value
            ++histogram[tempVal]; //increment corresponding frequency in histogram
        }

    return histogram;
}

//normalizes histogram to get the pdf of the image - probability density funnction

double * computeNormalized(const int * histogram, int size, int samples) {

    double * normalized = new double[size]; //create array to hold cdf

    for (int i = 0; i < size; ++i)
        normalized[i] = (double) histogram[i] / samples; //normalize each value by dividing by number of samples

    return normalized;
}

//computes cdf of image by making each term the sum of itself and the previous one - probability distribution function

double * computeCDF(const double * data, int size) {

    double * cdf = new double[size];

    cdf[0] = data[0];
    for (int i = 1; i < size; ++i)
        cdf[i] = cdf[i - 1] + data[i];

    return cdf;
}

//equalizes the given image using the given cdf

void equalize(ImageType & img, const double * histogramCDF) {
    int numRows, numCols, maxVal;
    img.getImageInfo(numRows, numCols, maxVal); //get details of image

    int currVal;
    //iterate through every pixel
    for (int r = 0; r < numRows; ++r)
        for (int c = 0; c < numCols; ++c) {
            img.getPixelVal(r, c, currVal); //read each pixel value
            img.setPixelVal(r, c, (int) (histogramCDF[currVal] * maxVal)); //change each pixel value using the cdf as the mapping
        }
}
