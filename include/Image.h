#ifndef IMAGE_H
#define IMAGE_H

// a simple example - you would need to add more funtions

class ImageType {
public:
    ImageType(void);
    ImageType(int tmpN, int tmpM, int tmpQ);
    ImageType(ImageType & oldImage);
    ~ImageType(void);

    void getImageInfo(int & rows, int & cols, int & levels) const;
    void setImageInfo(int rows, int cols, int levels);
    void setPixelVal(int i, int j, int val);
    void getPixelVal(int i, int j, int & val) const;

private:
    int N, M, Q;
    int **pixelValue;
};

#endif
