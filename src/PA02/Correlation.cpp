#include "PA02/HelperFunctions.h"
#include "ReadWrite.h"

#include <iostream>
#include <string>

int main(int argc, char * argv[]) {

    std::string imgFile, maskFile, outputFile;

    if (argc == 1)
        std::cout << std::endl << "Arguments not provided. Using default values" << std::endl;
    else if (argc != 4)
        std::cout << std::endl << "Incorrect number of arguments provided" << std::endl;

    std::cout << "To run with arguments, please provide the image file, mask file, and output file in that order" << std::endl;

    if (argc == 4) {
        imgFile = argv[1];
        maskFile = argv[2];
        outputFile = argv[3];
    } else {
        imgFile = "images/PA02/Correlation/Image.pgm";
        maskFile = "images/PA02/Correlation/Pattern.pgm";
        outputFile = "images/PA02/Correlation/Output.pgm";
    }

    std::cout << std::endl << "Image File: " << imgFile << std::endl << "Mask File: " << maskFile << std::endl << "Output File: " << outputFile << std::endl << std::endl;

    //read image file
    int imgRows, imgCols, Q;
    bool type;
    readImageHeader(imgFile.c_str(), imgRows, imgCols, Q, type);
    ImageType img(imgRows, imgCols, Q);
    readImage(imgFile.c_str(), img);

    //read mask file
    int maskRows, maskCols;
    readImageHeader(maskFile.c_str(), maskRows, maskCols, Q, type);
    ImageType mask(maskRows, maskCols, Q);
    readImage(maskFile.c_str(), mask);

    //find center of mask - will be uneven if there are even number of cols or rows
    int maskCenterRow = maskRows / 2;
    int maskCenterCol = maskCols / 2;

    std::cout << "Now applying mask to image. Note that a 0 will be used for any part of the mask that corresponds to areas outside the image boundaries " << std::endl;

    //apply mask to every pixel of image
    for (int r = 0; r < imgRows; ++r)
        for (int c = 0; c < imgCols; ++c)
            Helper::applyMask(img, mask, r, c, maskCenterRow, maskCenterCol);

    std::cout << std::endl << "Done applying mask. Now remapping values to [0 255] and saving as output image! " << std::endl;

    Helper::remapValues(img); //remap image values to [0 255] range

    writeImage(outputFile.c_str(), img); //save output image

    std::cout << std::endl << "Program Done! See output image '" << outputFile << "' for results" << std::endl;

}