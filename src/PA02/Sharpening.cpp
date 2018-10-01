#include "PA02/HelperFunctions.h"
#include "ReadWrite.h"

#include <iostream>
#include <string>


//helper function to apply mask and save result after normalization
void applyMask(const ImageType & img, const ImageType & mask, const std::string & outputFile, bool takeAbs = false);

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

        //generate Prewitt masks
        ImageType maskX(3, 3, 255);
        ImageType maskY(3, 3, 255);
        generatePrewitt(maskX, maskY);
        

        std::cout << std::endl << "X";
        Helper::printPixelValues(maskX);
        std::cout << std::endl << "Y";
        Helper::printPixelValues(maskY);


    } else if (choice == 2) {
        std::cout << std::endl << "Applying Sobel masks. " << std::endl;

        ImageType maskX(3, 3, 255);
        ImageType maskY(3, 3, 255);

        generateSobel(maskX, maskY);

        std::cout << std::endl << "X";
        Helper::printPixelValues(maskX);
        std::cout << std::endl << "Y";
        Helper::printPixelValues(maskY);

    } else {
        std::cout << std::endl << "Applying Laplacian mask. " << std::endl;

        //generate Laplacian mask      
        ImageType mask(3, 3, 255);
        generateLaplacian(mask);

        applyMask(img, mask, outputFile); //apply Laplacian and save result        

        std::cout << std::endl << "Done output image saved!" << std::endl;
    }

    return 0;
}

//helper function to apply mask at every point and save result after normalization

void applyMask(const ImageType & img, const ImageType & mask, const std::string & outputFile, bool takeAbs) {

    //get information of image
    int imgRows, imgCols, levels;
    img.getImageInfo(imgRows, imgCols, levels);

    ImageType outputImg(imgRows, imgCols, levels); //create output image

    //get information of mask
    int maskRows, maskCols;
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

    writeImage(outputFile.c_str(), outputImg); //save output image
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
