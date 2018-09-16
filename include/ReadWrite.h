#ifndef READWRITE_H
#define READWRITE_H

int readImageHeader(const char * fname, int & N, int & M, int & Q, bool & type);

int readImage(const char * fname, ImageType & image);

int writeImage(const char * fname, const ImageType & image);

#endif
