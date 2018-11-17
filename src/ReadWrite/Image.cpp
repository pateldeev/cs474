#include "Image.h"

ImageType::ImageType(void) {
    N = 0;
    M = 0;
    Q = 0;

    pixelValue = nullptr;
}

ImageType::ImageType(int tmpN, int tmpM, int tmpQ) {
    int i, j;

    N = tmpN;
    M = tmpM;
    Q = tmpQ;

    pixelValue = new int* [N];
    for (i = 0; i < N; i++) {
        pixelValue[i] = new int[M];
        for (j = 0; j < M; j++)
            pixelValue[i][j] = 0;
    }
}

ImageType::ImageType(const ImageType & oldImage) {
    int i, j;

    N = oldImage.N;
    M = oldImage.M;
    Q = oldImage.Q;

    pixelValue = new int* [N];
    for (i = 0; i < N; i++) {
        pixelValue[i] = new int[M];
        for (j = 0; j < M; j++)
            pixelValue[i][j] = oldImage.pixelValue[i][j];
    }
}

ImageType::~ImageType(void) {
    int i;

    for (i = 0; i < N; i++)
        delete [] pixelValue[i];
    delete [] pixelValue;
}

void ImageType::getImageInfo(int & rows, int & cols, int & levels) const{
    rows = N;
    cols = M;
    levels = Q;
}

void ImageType::setImageInfo(int rows, int cols, int levels) {
    N = rows;
    M = cols;
    Q = levels;
}

void ImageType::setPixelVal(int i, int j, int val) {
    pixelValue[i][j] = val;
}

void ImageType::getPixelVal(int i, int j, int & val) const{
    val = pixelValue[i][j];
}


