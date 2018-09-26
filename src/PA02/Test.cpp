#include <iostream>

#include "PA02/HelperFunctions.h"

#include "ReadWrite.h"

int main(int argc, char * argv[]) {

    int M, N, Q;
    bool type;
    readImageHeader("images/original/sf.pgm", N, M, Q, type); //read image header
    ImageType img(N, M, Q); //allocate memory for the image array
    readImage("images/original/sf.pgm", img); //read image

    Helper::remapValues(img);

    writeImage("/home/dp/Desktop/sf2.pgm", img);

    return 0;
}
