#include <iostream>
#include <fstream>

#include "Image.h"
#include "ReadWrite.h"

int main(int argc, char * argv[]){

	std::cout << "What do you want to resize the image to?\n1. 128 x 128\n"
				<< "2. 64 x 64\n3. 32 x 32" << std::endl;

	int choice;
	std::cin >> choice;
	int loopSkipper, myValue;
	if(choice == 1){
		// 128 x 128
		loopSkipper = 2;
	}
	else if(choice == 2){
		// 64 x 64
		loopSkipper = 4;
	}
	else if(choice == 3){
		loopSkipper = 8;
	}
	else{
		std::cout << "Could not read input" << std::endl;
		return 0;
	}

	//----------------------------------------------------------------------------
	// Start reading image in
	int M, N, Q;	// M = Columns, N = Rows, Q = Levels
	bool type;		// P5 or P6

	// Read header for image information
	readImageHeader(argv[1], N, M, Q, type);

	// Allocate memory for image
	ImageType image(N, M, Q);

	// Read in image information
	readImage(argv[1], image);
	//----------------------------------------------------------------------------

	for(int i = 0; i < N; i += loopSkipper){
		for(int j = 0; j < M; j += loopSkipper){
			 image.getPixelVal(i, j, myValue);
			 for(int k = 0; k < loopSkipper; k++){
			 	for(int l = 0; l < loopSkipper; l++){
			 		image.setPixelVal(i + k, j + l, myValue);
			 	}
			 }
		}
	}

	// Output image
	writeImage(argv[2], image);

	return 0;
}