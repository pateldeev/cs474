#include "PA02/HelperFunctions.h"
#include "ReadWrite.h"

#include <math.h> 
#include <iostream>
#include <string>
#include <valarray>


//helper function to apply mask and save result to outputImg after normalization
void applyMask(const ImageType & img, const ImageType & mask, ImageType & outputImg, bool takeAbs = false);

//helper function to find magnitude image given X and Y images from gradient operators
void calculateMagnitudeImg(const ImageType & imgX, const ImageType & imgY, ImageType & imgMag);

//helper function to generate Prewitt masks
void generatePrewitt(ImageType & maskX, ImageType & maskY);

//helper function to generate Sobel masks
void generateSobel(ImageType & maskX, ImageType & maskY);

//helper function to generate Laplacian mask
void generateLaplacian(ImageType & mask);

int main(int argc, char * argv[]) {
    //get parameters
    std::string inputFile, outputFile;
    if (argc != 3) {
        std::cout << std::endl << "Incorrect number of arguments provided" << std::endl
                << "Please provide the image file and output file in that order" << std::endl;
        return 0;
    }
    inputFile = argv[1];
    outputFile = argv[2];

    //read image
    int imgRows, imgCols, Q;
    bool type;
    readImageHeader(inputFile.c_str(), imgRows, imgCols, Q, type);
    ImageType img(imgRows, imgCols, Q);
    readImage(inputFile.c_str(), img);

    //get user input for choice
    int choice = 0;
    std::string input;
    while (choice <= 0 || choice > 3) {
        std::cout << std::endl << std::endl << "Please enter: " << std::endl << "  1: Sharpening with Prewitt masks" << std::endl
                << "  2: Sharpening with Sobel masks" << std::endl << "  3: Sharpening with Laplacian" << std::endl;

        std::cin >> input;

        choice = std::atof(input.c_str());

    }

    //perform requested operation
    if (choice == 1) {
        std::cout << std::endl << "Applying Prewitt masks. " << std::endl;

        std::string outputFileX = outputFile, outputFileY = outputFile;
        outputFileX.insert(outputFileX.find('.'), "_X");
        outputFileY.insert(outputFileY.find('.'), "_Y");

        ImageType maskX(3, 3, 255);
        ImageType outputImgX(imgRows, imgCols, Q);
        ImageType maskY(3, 3, 255);
        ImageType outputImgY(imgRows, imgCols, Q);
        ImageType outputImgMag(imgRows, imgCols, Q);

        generatePrewitt(maskX, maskY); //generate Prewitt masks

        applyMask(img, maskX, outputImgX); //apply X mask
        applyMask(img, maskY, outputImgY); //apply Y mask

        calculateMagnitudeImg(outputImgX, outputImgY, outputImgMag); //calculate magnitude image

        //save output images
        writeImage(outputFileX.c_str(), outputImgX);
        writeImage(outputFileY.c_str(), outputImgY);
        writeImage(outputFile.c_str(), outputImgMag);


    } else if (choice == 2) {
        std::cout << std::endl << "Applying Sobel masks. " << std::endl;

        std::string outputFileX = outputFile, outputFileY = outputFile;
        outputFileX.insert(outputFileX.find('.'), "_X");
        outputFileY.insert(outputFileY.find('.'), "_Y");

        ImageType maskX(3, 3, 255);
        ImageType outputImgX(imgRows, imgCols, Q);
        ImageType maskY(3, 3, 255);
        ImageType outputImgY(imgRows, imgCols, Q);
        ImageType outputImgMag(imgRows, imgCols, Q);

        generateSobel(maskX, maskY); //generate Sobel masks

        applyMask(img, maskX, outputImgX); //apply X mask
        applyMask(img, maskY, outputImgY); //apply Y mask

        calculateMagnitudeImg(outputImgX, outputImgY, outputImgMag); //calculate magnitude image

        writeImage(outputFileX.c_str(), outputImgX);
        writeImage(outputFileY.c_str(), outputImgY);
        writeImage(outputFile.c_str(), outputImgMag);
    } else {
        std::cout << std::endl << "Applying Laplacian mask. " << std::endl;

        ImageType mask(3, 3, 255);
        ImageType outputImg(imgRows, imgCols, Q);
        generateLaplacian(mask); //generate Laplacian mask      

        applyMask(img, mask, outputImg); //apply Laplacian and save result        

        writeImage(outputFile.c_str(), outputImg); //save output image
    }

    std::cout << std::endl << "Done output image saved!" << std::endl;

    return 0;
}

//helper function to apply mask at every point and save result to outputImg after normalization

void applyMask(const ImageType & img, const ImageType & mask, ImageType & outputImg, bool takeAbs) {

    //get information of image and mask
    int imgRows, imgCols, maskRows, maskCols, levels;
    img.getImageInfo(imgRows, imgCols, levels);
    mask.getImageInfo(maskRows, maskCols, levels);

    //apply mask at every pixel location and store result in output image
    double newVal;
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {
            newVal = Helper::applyMask(img, mask, r, c, (int) (maskRows / 2), (int) (maskCols / 2), false);

            if (takeAbs)
                newVal = abs(newVal); //take absolute value if instructed

            outputImg.setPixelVal(r, c, (int) newVal); //store new value
        }

    Helper::remapValues(outputImg); //remap values into [0,255]
}

//helper function to find magnitude image given X and Y images from gradient operators

void calculateMagnitudeImg(const ImageType & imgX, const ImageType & imgY, ImageType & imgMag) {
    //get information of image
    int imgRows, imgCols, levels;
    imgMag.getImageInfo(imgRows, imgCols, levels);

    //calculate magnitude at every pixel value
    int xVal, yVal, magnitude;
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c) {
            imgX.getPixelVal(r, c, xVal);
            imgY.getPixelVal(r, c, yVal);
            magnitude = (int) std::sqrt(xVal * xVal + yVal * yVal);
            imgMag.setPixelVal(r, c, magnitude);
        }
}


//helper function to generate Prewitt masks

void generatePrewitt(ImageType & maskX, ImageType & maskY) {
    int val;

    val = 0;
    maskX.setPixelVal(0, 1, val);
    maskX.setPixelVal(1, 1, val);
    maskX.setPixelVal(2, 1, val);
    maskY.setPixelVal(1, 0, val);
    maskY.setPixelVal(1, 1, val);
    maskY.setPixelVal(1, 2, val);

    val = 1;
    maskX.setPixelVal(0, 2, val);
    maskX.setPixelVal(1, 2, val);
    maskX.setPixelVal(2, 2, val);
    maskY.setPixelVal(2, 0, val);
    maskY.setPixelVal(2, 1, val);
    maskY.setPixelVal(2, 2, val);

    val = -1;
    maskX.setPixelVal(0, 0, val);
    maskX.setPixelVal(1, 0, val);
    maskX.setPixelVal(2, 0, val);
    maskY.setPixelVal(0, 0, val);
    maskY.setPixelVal(0, 1, val);
    maskY.setPixelVal(0, 2, val);
}

//helper function to generate Sobel masks

void generateSobel(ImageType & maskX, ImageType & maskY) {
    int val;

    val = 0;
    maskX.setPixelVal(0, 1, val);
    maskX.setPixelVal(1, 1, val);
    maskX.setPixelVal(2, 1, val);
    maskY.setPixelVal(1, 0, val);
    maskY.setPixelVal(1, 1, val);
    maskY.setPixelVal(1, 2, val);

    val = 1;
    maskX.setPixelVal(0, 2, val);
    maskX.setPixelVal(2, 2, val);
    maskY.setPixelVal(2, 0, val);
    maskY.setPixelVal(2, 2, val);

    val = 2;
    maskX.setPixelVal(1, 2, val);
    maskY.setPixelVal(2, 1, val);

    val = -1;
    maskX.setPixelVal(0, 0, val);
    maskX.setPixelVal(2, 0, val);
    maskY.setPixelVal(0, 0, val);
    maskY.setPixelVal(0, 2, val);

    val = -2;
    maskX.setPixelVal(1, 0, val);
    maskY.setPixelVal(0, 1, val);
}

//helper function to generate Laplacian mask

void generateLaplacian(ImageType & mask) {
    int val = 0;
    mask.setPixelVal(0, 0, val);
    mask.setPixelVal(0, 2, val);
    mask.setPixelVal(2, 0, val);
    mask.setPixelVal(2, 2, val);

    val = 1;
    mask.setPixelVal(0, 1, val);
    mask.setPixelVal(1, 0, val);
    mask.setPixelVal(1, 2, val);
    mask.setPixelVal(2, 1, val);

    val = -4;
    mask.setPixelVal(1, 1, val);

}
