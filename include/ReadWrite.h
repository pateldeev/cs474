#ifndef READWRITE_H
#define READWRITE_H

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

#endif
