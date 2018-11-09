# UNR CS 474 - Image Processing

## Building with CMake
```bash
mkdir build
cd build
cmake ..
make
```

## Executables
* ./Threshold 
  * Example threshold program given in class
* Project # 1
  * ./PA01_Equalization
    *  For image equalization
  * ./PA01_Sampling
    *  For image sampling (changing spacial resolution)
  * ./PA01_Quantization
    *  For image quantization (reducing gray levels)
* Project # 2
  * ./PA02_Correlation
    *  For running correlation with a given mask
  * ./PA02_Spatial
    *  For running spatial filtering with a given mask
  * ./PA02_Median
    *  For running median filtering with a given mask
* Project # 3
  * ./PA03_Experiment1
    * 3 part program that computes 1D DFT
  * ./PA03_Experiment2
    * 3 part program that computes 2D DFT
* Project # 4
  * ./PA04_Experiment1
    * Removes noise in the frequency domain
  * ./PA04_Experiment3
    * Restores motion blurred & noisy images
  * ./PA04_Experiment4
    * Applys homomorphic filtering
