# UNR CS 474 Fall 2018 - Image Processing
This project contains the source code for the various projects of the CS 474 Image Processing class for Fall 2018. See accompanying reports for details.

## Building with CMake
```bash
mkdir build
cd build
cmake ..
make
```

## Executables
* `./Threshold` 
  * Example threshold program given in class
* Project # 1 - Sampling, Quantization, & Equalization
  * `./PA01_Equalization`
    * For image equalization
  * `./PA01_Sampling`
    * For image sampling (changing spacial resolution)
  * `./PA01_Quantization`
    * For image quantization (changing gray levels)
* Project # 2 - Spatial Domain Filtering
  * `./PA02_Correlation`
    * For running correlation on image with mask
  * `./PA02_Median`
    * For running median filtering - for images corrupted by salt & pepper noise
  * `./PA02_Sharpening`
    * For sharpening - calculated gradient & Laplacian edge images
  * `./PA02_Spatial`
    * For smoothing - averaging and Gaussian
* Project # 3 - The Discrete Fourier Transform
  * `./PA03_Experiment1`
    * For computing the 1D DFT using the FFT
      * (a) computes DFT of test signal: `[2, 3, 4, 4]`
      * (b) computes DFT of cosine wave: `cos(2πux/N), u=8, N=128`
      * (c) computes DFT of rectangular pulse
  * `./PA03_Experiment2`
    * For computing the 2D DFT using the FFT
      * (a) computes DFT `512x512` image with `32x32` square in center
      * (b) computes DFT `512x512` image with `64x64` square in center
      * (c) computes DFT `512x512` image with `128x128` square in center
* Project # 4 - Frequency Domain Filtering
  * `./PA04_Experiment1`
    * For removing structured noise in the frequency domain using band reject filter
  * `./PA04_Experiment3`
    * For modeling motion blur and restoring blurred & noisy images using inverse filtering and Wiener filtering
  * `./PA04_Experiment4`
    * For applying homomorphic filtering to address uneven lighting
