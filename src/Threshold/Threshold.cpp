#include <iostream>
#include <fstream>

#include "Image.h"
#include "ReadWrite.h"

int main(int argc, char * argv[]) {

    int i, j;
    int M, N, Q;
    bool type;
    int val;
    int thresh;

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array
    ImageType image(N, M, Q);

    // read image
    readImage(argv[1], image);

    std::cout << "Enter threshold: ";
    std::cin >> thresh;

    // threshold image 
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            image.getPixelVal(i, j, val);
            if (val < thresh)
                image.setPixelVal(i, j, 255);
            else
                image.setPixelVal(i, j, 0);
        }
    }

    // write image
    writeImage(argv[2], image);

    return 0;
}
