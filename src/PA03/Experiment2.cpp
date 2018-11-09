#include "ReadWrite.h"

#include "FFT/fft.c"
#include "FFT/fft2D.c"

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

// Function to save magnitude or phase of signal to external file
void outputMagnitude(const std::vector<float> & signal, const std::string & fileName);

// Generate the black image with white square in center
void generate_image(ImageType &, int, int, int);

// Function for doing the 2D FFT transformation
void FFT2D_Main(const std::string &, const std::string &, ImageType &, bool);

int main(int argc, char * argv[]) {

	// Part A, 32x32 white square in center
	ImageType generated_image_32x32(512, 512, 255);
	generate_image(generated_image_32x32, 512, 512, 32);
	writeImage("images/PA03/Experiment2/partA_original.pgm", generated_image_32x32);

	// Unshifted magnitude
	FFT2D_Main("images/PA03/Experiment2/partA_unshifted_magnitude.dat", "images/PA03/Experiment2/partA_unshifted.pgm", generated_image_32x32, 0);

	// Magnitude shifted to center of frequency domain
	FFT2D_Main("images/PA03/Experiment2/partA_shifted_magnitude.dat", "images/PA03/Experiment2/partA_shifted.pgm", generated_image_32x32, 1);



	// Part B, 64x64 white square in center
	ImageType generated_image_64x64(512, 512, 255);
	generate_image(generated_image_64x64, 512, 512, 64);
	writeImage("images/PA03/Experiment2/partB_original.pgm", generated_image_64x64);

	// Unshifted magnitude
	FFT2D_Main("images/PA03/Experiment2/partB_unshifted_magnitude.dat", "images/PA03/Experiment2/partB_unshifted.pgm", generated_image_64x64, 0);

	// Magnitude shifted to center of frequency domain
	FFT2D_Main("images/PA03/Experiment2/partB_shifted_magnitude.dat", "images/PA03/Experiment2/partB_shifted.pgm", generated_image_64x64, 1);



	// Part C, 128x128 white square in center
	ImageType generated_image_128x128(512, 512, 255);
	generate_image(generated_image_128x128, 512, 512, 128);
	writeImage("images/PA03/Experiment2/partC_original.pgm", generated_image_128x128);

	// Unshifted magnitude
	FFT2D_Main("images/PA03/Experiment2/partC_unshifted_magnitude.dat", "images/PA03/Experiment2/partC_unshifted.pgm", generated_image_128x128, 0);

	// Magnitude shifted to center of frequency domain
	FFT2D_Main("images/PA03/Experiment2/partC_shifted_magnitude.dat", "images/PA03/Experiment2/partC_shifted.pgm", generated_image_128x128, 1);

	return 0;
}

void outputMagnitude(const std::vector<float> & signal, const std::string & fileName){
	std::ofstream outputFile(fileName);

	for(unsigned int i = 0; i < signal.size(); ++i){
		outputFile << signal[i] << std::endl;
	}

	outputFile.close();
}

void generate_image(ImageType &image, int row, int col, int middle){
	
	// Set everything 0 initially
	for(int i = 0; i < row; ++i){
		for(int j = 0; j < col; ++j){
			image.setPixelVal(i, j, 0);
		}
	}

	// Set middle square to 255
	for(int i = (row / 2) - (middle / 2); i < (row / 2) + (middle / 2); ++i){
		for(int j = (col / 2) - (middle / 2); j < (col / 2) + (middle / 2); ++j){
			image.setPixelVal(i, j, 255);
		}
	}
}

void FFT2D_Main(const std::string &dataFile, const std::string &imageFile, ImageType &image, bool shifted){

	int N, M, Q; // M = Columns, N = Rows, Q = Levels
	image.getImageInfo(N, M, Q);

	std::vector<float> imgDataReal;
	std::vector<float> imgDataImag;
	std::vector<float> imgDataCombined;

	// Obtain real values and set imaginary to 0
	int px_val;
	for(int i = 0; i < N; ++i){
		for(int j = 0; j < M; ++j){
			image.getPixelVal(i, j, px_val);
			imgDataReal.push_back(px_val);
			imgDataImag.push_back(0);
		}
	}

	if(shifted){
		/* If the magnitude should be shifted to center of freq. domain
		 * 
		 * 
		 * By inverting every other value in 1D, the FFT will be
		 * shifted to the center of the frequency domain. In 2D
		 * the shifting needs to be done in a checkerboard way
		 */
		for(int i = 0; i < N; ++i){
			if(!(i % 2)){
				for(int j = 0; j < M; j += 2){
					imgDataReal[i * M + j] *= -1;
				}
			}
			else{
				for(int j = 1; j < M; j += 2){
					imgDataReal[i * M + j] *= -1;
				}
			}
		}
	}

	fft2D(N, M, &imgDataReal[0], &imgDataImag[0], -1); // Forward 2D FFT

	// Combines real and imaginary into one vector
	for(int i = 0; i < N * M; ++i){
		px_val = std::sqrt(imgDataReal[i] * imgDataReal[i] + imgDataImag[i] * imgDataImag[i]);
		imgDataCombined.push_back(log(1 + px_val));
	}

	// Output magnitude data to file for reference
	outputMagnitude(imgDataCombined, dataFile);

	// 
	ImageType FFT2D_Magnitude_Image(N, M, Q);
	double max = *max_element(imgDataCombined.begin(), imgDataCombined.end());
	for(int i = 0; i < N; ++i){
		for(int j = 0; j < M; ++j){
			// Rescales to 256 levels for better viewing
			FFT2D_Magnitude_Image.setPixelVal(i, j, (imgDataCombined[i * M + j]) / max * 255);
		}
	}

	// Output of magnitude to image
	char* c = const_cast<char*>(imageFile.c_str());
	writeImage(c, FFT2D_Magnitude_Image);
}