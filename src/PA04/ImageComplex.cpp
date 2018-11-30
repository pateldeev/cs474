#include "PA04/ImageComplex.h"
#include "PA04/HelperFunctions.h"
#include "FFT/fft2D.c"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>

//Constructs image to hold specified number of values

ImageComplex::ImageComplex(int rows, int cols) : m_rows(rows), m_cols(cols), m_dataR(nullptr), m_dataI(nullptr) {
    //allocate space for real and imaginary parts of data
    m_dataR = new float* [m_rows];
    m_dataI = new float* [m_rows];
    for (int r = 0; r < m_rows; ++r) {
        m_dataR[r] = new float[m_cols]();
        m_dataI[r] = new float[m_cols]();
    }
}

//Constructs image by copying over values from existing image.

ImageComplex::ImageComplex(const ImageComplex & other) : ImageComplex(other.m_rows, other.m_cols) {
    //copy over data values
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            m_dataR[r][c] = other.m_dataR[r][c];
            m_dataI[r][c] = other.m_dataI[r][c];
        }
}

//Constructs image from pixel values from ImageType variables.
//imgR and imgI must have same size.

ImageComplex::ImageComplex(const ImageType & imgR, const ImageType & imgI) : m_rows(0), m_cols(0), m_dataR(nullptr), m_dataI(nullptr) {
    //get image info and ensure it is good
    int rowsR, rowsI, colsR, colsI, q;
    imgR.getImageInfo(rowsR, colsR, q);
    imgI.getImageInfo(rowsI, colsI, q);
    assert(rowsR == rowsI && colsR == colsI);
    m_rows = rowsR, m_cols = colsR;

    //allocate space for real and imaginary parts of data
    m_dataR = new float* [m_rows];
    m_dataI = new float* [m_rows];
    for (int r = 0; r < m_rows; ++r) {
        m_dataR[r] = new float[m_cols];
        m_dataI[r] = new float[m_cols];
    }

    //copy over values
    int tempR, tempI;
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            imgR.getPixelVal(r, c, tempR);
            imgI.getPixelVal(r, c, tempI);
            m_dataR[r][c] = float(tempR);
            m_dataI[r][c] = float(tempI);
        }
}

//Deallocate memory holding pixel data.

ImageComplex::~ImageComplex(void) {
    //free up used space
    for (int r = 0; r < m_rows; ++r) {
        delete[] m_dataR[r];
        delete[] m_dataI[r];
    }
    delete[] m_dataR;
    delete[] m_dataI;
}

//Copies over pixel values - rhs must have same size of image.

ImageComplex& ImageComplex::operator=(const ImageComplex & rhs) {
    //only defined if two images have same size
    assert(m_rows == rhs.m_rows && m_cols == rhs.m_cols);

    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            m_dataR[r][c] = rhs.m_dataR[r][c];
            m_dataI[r][c] = rhs.m_dataI[r][c];
        }

    return *this;
}

//Adds pixel values of other image - other must have same size.

void ImageComplex::operator+=(const ImageComplex & other) {
    //only defined if two images have same size
    assert(m_rows == other.m_rows && m_cols == other.m_cols);

    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            m_dataR[r][c] += other.m_dataR[r][c];
            m_dataI[r][c] += other.m_dataI[r][c];
        }
}


//Adds constant to real parts - needed for Wiener filtering.

ImageComplex ImageComplex::operator+(float valReal) const {
    ImageComplex sum(*this);

    //add value to real part of each pixel
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c)
            sum.m_dataR[r][c] += valReal;

    return sum;
}

//Get size of image.

void ImageComplex::getImageInfo(int & rows, int & cols) const {
    rows = m_rows;
    cols = m_cols;
}

//Copy pixel data to ImageType variables. 
//All non integer values are rounded down.
//imgR and imgI must have same size as image.

void ImageComplex::getImageType(ImageType & imgR, ImageType & imgI, bool normalize) const {
    //get image info and ensure it is valid
    int rowsR, rowsI, colsR, colsI, q;
    imgR.getImageInfo(rowsR, colsR, q);
    imgI.getImageInfo(rowsI, colsI, q);
    assert(rowsR == rowsI && m_rows == rowsR && colsR == colsI && m_cols == colsR);

    //move over values. Round down to nearest integer
    int tempR, tempI;
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            tempR = (int) m_dataR[r][c];
            tempI = (int) m_dataI[r][c];
            imgR.setPixelVal(r, c, tempR);
            imgI.setPixelVal(r, c, tempI);
        }

    //renormalized values if requested
    if (normalize) {
        Helper::remapValues(imgR);
        Helper::remapValues(imgI);
    }
}

//Get spectrum of image as ImageType variable.
//Applies log transformation: log(1+val).
//spectrum must have same size as image.

void ImageComplex::getSpectrum(ImageType & spectrum, bool normalize) const {
    //get image info and ensure it is valid
    int spectrumR, spectrumC, q;
    spectrum.getImageInfo(spectrumR, spectrumC, q);
    assert(spectrumR == m_rows && spectrumC == m_cols);

    //function to get spectrum value - applies log transformation
    auto getNewVal = [](float real, float imaginary) ->int {
        float val = std::sqrt((double) real * real + (double) imaginary * imaginary);
        return ((int) std::log(1 + val));
    };

    //find each value of spectrum
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c)
            spectrum.setPixelVal(r, c, getNewVal(m_dataR[r][c], m_dataI[r][c]));

    //renormalized values if requested
    if (normalize)
        Helper::remapValues(spectrum);
}

//Get real and imaginary value at specific pixel.

void ImageComplex::getPixelVal(int row, int col, float & re, float & im) const {
    re = m_dataR[row][col];
    im = m_dataI[row][col];
}


//Set real and imaginary value at specific pixel.

void ImageComplex::setPixelVal(int row, int col, float re, float im) {
    if (re == -0.f) re = 0.0;
    if (im == -0.f) im = 0.0;
    m_dataR[row][col] = re;
    m_dataI[row][col] = im;
}

//Apply 2D FFT to image. Note the function internally shifts the magnitude.

void ImageComplex::applyFFT(bool forward) {
    //move data into usable form - make sure to extend data to power of 2 for FFT
    int extendedR = std::pow(2, std::ceil(log(m_rows) / std::log(2)));
    int extendedC = std::pow(2, std::ceil(log(m_cols) / std::log(2)));
    std::vector<float> dataR(extendedR * extendedC, 0.f), dataI(extendedR * extendedC, 0.f);
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            //copy over data. Invert signs as necessary for shifting
            dataR[r * extendedR + c] = m_dataR[r][c] * (((r + c) % 2) ? 1 : -1);
            dataI[r * extendedR + c] = m_dataI[r][c] * (((r + c) % 2) ? 1 : -1);
        }

    //apply FFT
    fft2D(extendedR, extendedC, &dataR[0], &dataI[0], ((forward) ? -1 : 1));

    //save new values. Make sure to invert shifting transformation
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            m_dataR[r][c] = dataR[r * extendedR + c] * (((r + c) % 2) ? 1 : -1);
            m_dataI[r][c] = dataI[r * extendedR + c] * (((r + c) % 2) ? 1 : -1);
        }
}

//Compute power spectrum of image. Useful for Wiener filtering.

void ImageComplex::powerSpectrum(void) {
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            m_dataR[r][c] = m_dataR[r][c] * m_dataR[r][c] + m_dataI[r][c] * m_dataI[r][c];
            m_dataI[r][c] = 0;
        }
}

//Apply point-by-point complex multiplication.
//rhs must have same size as image.
//Negative cutOffRadius means to multiply entire spectrum. 

void ImageComplex::complexMultiplication(const ImageComplex & rhs, const float cutoffR) {
    //only defined if two images have same size
    assert(m_rows == rhs.m_rows && m_cols == rhs.m_cols);

    float tempR, tempI;
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            int u = r - m_rows / 2, v = c - m_cols / 2; //u and v coordinates of pixel; //u and v coordinates of pixel
            if (cutoffR < 0 || std::sqrt(u * u + v * v) <= cutoffR) {
                tempR = m_dataR[r][c] * rhs.m_dataR[r][c] - m_dataI[r][c] * rhs.m_dataI[r][c];
                tempI = m_dataR[r][c] * rhs.m_dataI[r][c] + m_dataI[r][c] * rhs.m_dataR[r][c];
                m_dataR[r][c] = tempR, m_dataI[r][c] = tempI;
            }
        }
}


//Compute complex multiplicative inverse. Needed to do division.
//Returns calling image for chaining with multiplication.

ImageComplex& ImageComplex::complexInverse(void) {
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c) {
            float x = m_dataR[r][c] * m_dataR[r][c] + m_dataI[r][c] * m_dataI[r][c];
            if (x != 0.f)
                m_dataR[r][c] /= x, m_dataI[r][c] /= -x;
        }
    return *this;
}

//Print pixel values to screen. Useful for debugging.
//Will print to screen if fileName is empty.

void ImageComplex::printPixelValues(const char * fileName) const {
    if (!fileName) { //print to terminal
        for (int r = 0; r < m_rows; ++r) {
            std::cout << std::endl << " | ";
            for (int c = 0; c < m_cols; ++c)
                std::cout << m_dataR[r][c] << "+j" << m_dataI[r][c] << " | ";
        }
        std::cout << std::endl;
    } else { //print to text file
        std::ofstream file(fileName);

        for (int r = 0; r < m_rows; ++r) {
            file << std::endl << " | ";
            for (int c = 0; c < m_cols; ++c)
                file << m_dataR[r][c] << "+j" << m_dataI[r][c] << " | ";
        }
        file << std::endl;

        file.close();
    }
}
