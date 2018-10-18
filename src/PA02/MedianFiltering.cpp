#include "PA02/HelperFunctions.h"
#include "ReadWrite.h"

#include <iostream>
#include <ctime>
#include <string>
#include <algorithm>
#include <sstream>

// Corrupt the image with salt and pepper
void saltPepperCorruption(const ImageType &, ImageType &, int);

// Median filtering function
void medianMask(const ImageType &, ImageType &, int);

int main(int argc, char * argv[]){

	if (argc != 3) {
		std::cout << "Incorrect number of arguments provided" << std::endl
		<< "Please provide the image file and mask size in that order" << std::endl;
		return 0;
	}

	// Check to make sure argument is valid number
	std::istringstream ss(argv[2]);
	int medianFilterSize;
	if(!(ss >> medianFilterSize)){
		std::cerr << "Invalid number\n";
		return 0;
	}
	else if(!(ss.eof())){
		std::cerr << "Trailing characters after number\n";
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

	// Corrupt the image first
	ImageType corrupted_30(N, M, Q), corrupted_50(N, M, Q);

	saltPepperCorruption(image, corrupted_30, 30);
	std::string imageOutputName = "images/PA02/median/corrupted_30.pgm";
	writeImage(imageOutputName.c_str(), corrupted_30);

	saltPepperCorruption(image, corrupted_50, 50);
	imageOutputName = "images/PA02/median/corrupted_50.pgm";
	writeImage(imageOutputName.c_str(), corrupted_50);

	// Applying median filter to corrupted images
	ImageType medianFix_30(N, M, Q), medianFix_50(N, M, Q);

	medianMask(corrupted_30, medianFix_30, medianFilterSize);
	imageOutputName = "images/PA02/median/medianFix_30.pgm";
	writeImage(imageOutputName.c_str(), medianFix_30);

	medianMask(corrupted_50, medianFix_50, medianFilterSize);
	imageOutputName = "images/PA02/median/medianFix_50.pgm";
	writeImage(imageOutputName.c_str(), medianFix_50);

	return 0;
}

void saltPepperCorruption(const ImageType &og_image, ImageType &new_image, int percentage){
	int numRows, numCols, numLevels, roll, tmpVal;
	og_image.getImageInfo(numRows, numCols, numLevels);

	srand(time(NULL));

	for(int i = 0; i < numRows; ++i){
		for(int j = 0; j < numCols; ++j){
			roll = rand() % 100;
			if(roll < (percentage / 2)){
				// Make it white
				new_image.setPixelVal(i, j, 255);
			}
			else if(roll > (percentage / 2) && roll < percentage){
				// Make it black
				new_image.setPixelVal(i, j, 0);
			}
			else{
				og_image.getPixelVal(i, j, tmpVal);
				new_image.setPixelVal(i, j, tmpVal);
			}
		}
	}
}

void medianMask(const ImageType &og_image, ImageType &new_image, int maskSize){
	int values[maskSize * maskSize];
	int imgRows, imgCols, imgLevels, valuesCounter = 0;
	int rowOffset, colOffset, rowLoc, colLoc, sortedMedianValue;
	og_image.getImageInfo(imgRows, imgCols, imgLevels);
	for(int i = 0; i < imgRows; ++i){
		for(int j = 0; j < imgCols; ++j){
			for(int k = 0; k < maskSize; ++k){
				for(int l = 0; l < maskSize; ++l){
					rowOffset = l - (maskSize / 2);
					colOffset = k - (maskSize / 2);

					rowLoc = i + rowOffset;
					colLoc = j + colOffset;

					if(rowLoc >= 0 && rowLoc < imgRows && colLoc >= 0 && colLoc < imgCols){
						og_image.getPixelVal(rowLoc, colLoc, values[valuesCounter]);
					}
					valuesCounter++;
				}
			}
			std::sort(values, values + (maskSize * maskSize));
			sortedMedianValue = values[(maskSize * maskSize) / 2];
			new_image.setPixelVal(i, j, sortedMedianValue);
			valuesCounter = 0;
		}
	}
}