#ifndef FFT2D_C
#define FFT2D_C

#include "PA03/fft.c"

#include <cstdio>
#include <vector>

/* Function to implement 2D fft via given 1D fft function.
 * Used to calculate the DFT of an NxM image with N rows and M cols.
 * N and M must be a a power of 2. Otherwise function will not work.
 * The data arrays must be of length N*M with the data stored sequentially.
 * ie: first row will be stored in the range [0,M-1]. The next row will be at [M,2M-1] and so on.
 * isign: use -1 for forward DFT and 1 for inverse DFT 
 */
inline void fft2D(unsigned long N, unsigned long M, float real_Fuv[], float imag_Fuv[], int isign) {
    //helpers to get and set values by row and column number
    auto getReal = [&M, real_Fuv](unsigned int r, unsigned int c) ->float {
        return real_Fuv[r * M + c];
    };
    auto getImaginary = [&M, imag_Fuv](unsigned int r, unsigned int c) ->float {
        return imag_Fuv[r * M + c];
    };
    auto setValue = [&M, real_Fuv, imag_Fuv](unsigned int r, unsigned int c, float realVal, float imagVal) ->void {
        real_Fuv[r * M + c] = realVal;
        imag_Fuv[r * M + c] = imagVal;
    };

    //transform each row using 1D fft
    std::vector<float> tempArr(2 * M);
    for (unsigned int r = 0; r < N; ++r) {
        //copy over real and imaginary part to temporary array
        unsigned int i = 0;
        for (unsigned int c = 0; c < M; ++c) {
            tempArr[i++] = getReal(r, c);
            tempArr[i++] = getImaginary(r, c);
        }

        //compute fft with help of given function
        fft(&tempArr[0] - 1, M, isign);

        //store result in original arrays
        i = 0;
        for (unsigned int c = 0; c < M; ++c, i += 2)
            setValue(r, c, tempArr[i], tempArr[i + 1]);
    }

    //transform each column using 1D fft
    tempArr.resize(2 * N);
    for (unsigned int c = 0; c < M; ++c) {
        //copy over real and imaginary part to temporary array
        unsigned int i = 0;
        for (unsigned int r = 0; r < N; ++r) {
            tempArr[i++] = getReal(r, c);
            tempArr[i++] = getImaginary(r, c);
        }

        //compute fft with help of given function. Make sure to normalize after
        fft(&tempArr[0] - 1, N, isign);
        for (float & tempVal : tempArr)
            tempVal /= N; //normalize by 1/N

        //store result in original arrays
        i = 0;
        for (unsigned int r = 0; r < N; ++r, i += 2)
            setValue(r, c, tempArr[i], tempArr[i + 1]);
    }
}

#endif
