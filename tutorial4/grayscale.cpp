#include "grayscale.hpp"

using namespace cv;

#define CCIR601_R_COEF 0.299
#define CCIR601_G_COEF 0.587 
#define CCIR601_B_COEF 0.114

inline uchar computeGamma(const uchar* bgrPixel) {
    double b = (double)bgrPixel[0];
    double g = (double)bgrPixel[1];
    double r = (double)bgrPixel[2];
    return (uchar)(CCIR601_B_COEF * b + CCIR601_G_COEF * g + CCIR601_R_COEF * r);
}

void toGrayscale(Mat &input, Mat &output) {
    /* Converts to grayscale following CCIR 601 */
    CV_Assert(input.type() == CV_8UC3);

    int channels = input.channels();
    int nRows = input.rows;
    int nCols = input.cols;
    output.create(nRows, nCols, CV_8UC1);

    if (input.isContinuous() && output.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    for(int i = 0; i < nRows; i++) {
        uchar *inputRow_p = input.ptr(i);
        uchar *outputRow_p = output.ptr(i);
        for (int j = 0; j < nCols; j++) {
            outputRow_p[j] = computeGamma(&(inputRow_p[j * channels]));
        }
    }
}

void
toGrayscale_threaded(int matStartingIdx, int matEndingIdx, Mat &input, Mat &output) {
    int channels = input.channels();
    int nCols = input.cols;
    int i, j;   // Loop variables

#pragma omp simd private(j) collapse(2)
    for(i = matStartingIdx; i < matEndingIdx; i++) {
        for (j = 0; j < nCols; j++) {
            output.ptr(i)[j] = computeGamma(&(input.ptr(i)[j * channels]));
        }
    }
}
