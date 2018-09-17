#include <iostream>
#include <fstream>
#include <cmath>

#include "Image.h"
#include "ReadWrite.h"

int main(int argc, char * argv[]){

	std::cout << "What gray levels do you want?\n1. 128\n"
				<< "2. 32\n3. 8\n4. 2" << std::endl;

	int choice, myValue;
	double roundedAnswer;
	std::cin >> choice;

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

	for(int i = 0; i < N; i++){
		for(int j = 0; j < M; j++){
			image.getPixelVal(i, j, myValue);
			if(choice == 1){
				if(myValue % 2 == 1){
					image.setPixelVal(i, j, myValue - 1);
				}
			}
			else if(choice == 2){
				myValue /= 4.113;
				if(myValue % 2 == 1){
					myValue++;
				}
				roundedAnswer = myValue * 4.113;
				myValue = std::round(roundedAnswer);
				image.setPixelVal(i, j, myValue);
				/*
				 * In future implementations, use formula to
				 * obtain the "magic" number. Number is found
				 * by dividing 255 by (number of sections - 1) * 2
				 * Further details are explained in comment below
				 */
			}
			else if(choice == 3){
				myValue /= 18.2;
				if(myValue %2 == 1){
					myValue++;
				}
				roundedAnswer = myValue * 18.2;
				myValue = std::round(roundedAnswer);
				image.setPixelVal(i, j, myValue);

				/*
				 * Values obtained by dividing 255 into 7 sections
				 * 0, 36, 73, 109, 146, 182, 219, 255
				 * 
				 * Dividing by 14 finds the values midway between
				 * the above values. 18.2 is the size of each 1/14
				 * section, so dividing by it will give us which
				 * 1/14 section the number is in. Rounding up odd
				 * numbers to the next even number puts them in
				 * the correct gray value when multiplied by 18.2.
				 * Not rounding up puts it exactly at the numbers
				 * midway between the 7 gray numbers
				*/
			}
			else if(choice == 4){
				if(myValue < 128){
					image.setPixelVal(i, j, 0);
				}
				else{
					image.setPixelVal(i, j, 255);
				}
			}
			else{
				std::cout << "Invalid choice" << std::endl;
				return -1;
			}
		}
	}

	// Output image
	writeImage(argv[2], image);

	return 0;
}