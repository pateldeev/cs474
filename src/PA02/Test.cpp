#include "PA02/HelperFunctions.h"
#include "ReadWrite.h"

#include <iostream>
#include <ctime>
#include <string>

void randomlyPopulateImage(ImageType & img, int numRows, int numCols, int range, int minVal) {
    int tempVal;
    std::srand(std::time(nullptr));
    for (int r = 0; r < numRows; ++r)
        for (int c = 0; c < numCols; ++c) {
            tempVal = ((double) std::rand() / RAND_MAX) * range + minVal;
            img.setPixelVal(r, c, tempVal);
        }
}

int writeMask(int argc, char * argv[]){

	ImageType img(3,3,256);
	
	int pos[] = {0,1,2,3,4};
	int neg[] = {0,-1,-2,-3,-4};

	img.setPixelVal(0,0,pos[0]);
	img.setPixelVal(0,1,pos[1]);
	img.setPixelVal(0,2,pos[0]);
	img.setPixelVal(1,0,pos[1]);
	img.setPixelVal(1,1,neg[4]);
	img.setPixelVal(1,2,pos[1]);
	img.setPixelVal(2,0,pos[0]);
	img.setPixelVal(2,1,pos[1]);
	img.setPixelVal(2,2,pos[0]);

	Helper::printPixelValues(img);


	std::string file = "/home/dp/Desktop/CS474_Workspace/images/PA02/masks/mask.pgm";
	writeImage(file.c_str(), img); //save output image

	return 0;
}

int testMask(int argc, char * argv[]) {
    ImageType img(10, 10, 100);
    randomlyPopulateImage(img, 10, 10, 90, 10);

    ImageType mask(3, 3, 100);
    randomlyPopulateImage(mask, 3, 3, 9, 1);

    Helper::printPixelValues(img);
    Helper::printPixelValues(mask);

    Helper::applyMask(img, mask, 1, 3, 2, 1);

    Helper::printPixelValues(img);

    return 0;
}

int main(int argc, char * argv[]){

	if(argc == 1){
		std::cout << std::endl << "testing mask application function" << std::endl;
		return testMask(argc, argv);
	}else{
		std::cout << std::endl << "running writeMask function" << std::endl;
		return writeMask(argc, argv);
	}

}
