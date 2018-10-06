#include "PA02/HelperFunctions.h"
#include "ReadWrite.h"

#include <iostream>

int main(int argc, char * argv[]){

	if(argc != 3) {
		std::cout << "Incorrect number of arguments provided" << std::endl
		<< "Please provide the image file and output file in that order" << std::endl;
		return 0;
	}

	std::cout << "Please make a choice:\n1. 7x7 Averaging\n2. 15x15 Averaging"
	"\n3. 7x7 Gaussian\n4. 15x15 Gaussian\n5. Exit" << std::endl;

	int choice;
	std::cin >> choice;
	if(choice != 1 && choice != 2 && choice != 3 && choice != 4){
		// In case stupid stuff is entered, like a character
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

	// Hard coding in 7x7 Gaussian mask
	int mask_7x7[7][7] = {{1, 1, 2, 2, 2, 1, 1}, {1, 2, 2, 4, 2, 2, 1},
	{2, 2, 4, 8, 4, 2, 2}, {2, 4, 8, 16, 8, 4, 2}, {2, 2, 4, 8, 4, 2, 2},
	{1, 2, 2, 4, 2, 2, 1}, {1, 1, 2, 2, 2, 1, 1}};
	
	// Hard coding in 15x15 Gaussian mask
	int mask_15x15[15][15] = {{2, 2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2},
	{2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2},
	{3, 4, 6, 7, 9, 10, 10, 11, 10, 10, 9, 7, 6, 4, 3},
	{4, 5, 7, 9, 10, 12, 13, 13, 13, 12, 10, 9, 7, 5, 4},
	{5, 7, 9, 11, 13, 14, 15, 16, 15, 14, 13, 11, 9, 7, 5},
	{5, 7, 10, 12, 14, 16, 17, 18, 17, 16, 14, 12, 10, 7, 5},
	{6, 8, 10, 13, 15, 17, 19, 19, 19, 17, 15, 13, 10, 8, 6},
	{6, 8, 11, 13, 16, 18, 19, 20, 19, 18, 16, 13, 11, 8, 6},
	{6, 8, 10, 13, 15, 17, 19, 19, 19, 17, 15, 13, 10, 8, 6},
	{5, 7, 10, 12, 14, 16, 17, 18, 17, 16, 14, 12, 10, 7, 5},
	{5, 7, 9, 11, 13, 14, 15, 16, 15, 14, 13, 11, 9, 7, 5},
	{4, 5, 7, 9, 10, 12, 13, 13, 13, 12, 10, 9, 7, 5, 4},
	{3, 4, 6, 7, 9, 10, 10, 11, 10, 10, 9, 7, 6, 4, 3},
	{2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2},
	{2, 2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2}};

	int avg_7x7[7][7], avg_15x15[15][15];

	// Hard coding in 7x7 average mask
	for(int i = 0; i < 7; ++i){
		for(int j = 0; j < 7; ++j){
			avg_7x7[i][j] = 1;
		}
	}

	// Hard coding in 15x15 average mask
	for(int i = 0; i < 15; ++i){
		for(int j = 0; j < 15; ++j){
			avg_15x15[i][j] = 1;
		}
	}

	// Converting the 7x7 masks into usable format
	ImageType am_7x7(7, 7, Q), gm_7x7(7, 7, Q);
	for(int i = 0; i < 7; ++i){
		for(int j = 0; j < 7; ++j){
			am_7x7.setPixelVal(i, j, mask_7x7[i][j]);
			gm_7x7.setPixelVal(i, j, mask_7x7[i][j]);
		}
	}

	// Converting the 15x15 masks into usable format
	ImageType am_15x15(15, 15, Q), gm_15x15(15, 15, Q);
	for(int i = 0; i < 15; ++i){
		for(int j = 0; j < 15; ++j){
			am_15x15.setPixelVal(i, j, mask_15x15[i][j]);
			gm_15x15.setPixelVal(i, j, mask_15x15[i][j]);
		}
	}

	double newVal;

	if(choice == 1){
		ImageType outputImg(N, M, Q);
		for(int i = 0; i < N; ++i){
			for(int j = 0; j < M; ++j){
				// Apply mask to obtain new value
				newVal = Helper::applyMask(image, am_7x7, i, j, 3, 3);
				outputImg.setPixelVal(i, j, (int) newVal);
			}
		}
		// Remap image values to [0, 255]
		Helper::remapValues(outputImg);
		writeImage(argv[2], outputImg);
	}

	else if(choice == 2){
		ImageType outputImg(N, M, Q);
		for(int i = 0; i < N; ++i){
			for(int j = 0; j < M; ++j){
				newVal = Helper::applyMask(image, am_15x15, i, j, 7, 7);
				outputImg.setPixelVal(i, j, (int) newVal);
			}
		}
		Helper::remapValues(outputImg);
		writeImage(argv[2], outputImg);
	}

	else if(choice == 3){
		ImageType outputImg(N, M, Q);
		for(int i = 0; i < N; ++i){
			for(int j = 0; j < M; ++j){
				newVal = Helper::applyMask(image, gm_7x7, i, j, 3, 3);
				outputImg.setPixelVal(i, j, (int) newVal);
			}
		}
		Helper::remapValues(outputImg);
		writeImage(argv[2], outputImg);
	}

	else{
		ImageType outputImg(N, M, Q);
		for(int i = 0; i < N; ++i){
			for(int j = 0; j < M; ++j){
				newVal = Helper::applyMask(image, gm_15x15, i, j, 7, 7);
				outputImg.setPixelVal(i, j, (int) newVal);
			}
		}
		Helper::remapValues(outputImg);
		writeImage(argv[2], outputImg);
	}

	return 0;
}