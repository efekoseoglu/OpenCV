#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "stdafx.h"
#include "math.h"
#include <iostream>

using namespace cv;
using namespace std;

#define PI 3.14159265
class DFT_Coeff {
public:
    double real, img;
    DFT_Coeff() {
        real = 0.0;
        img = 0.0;
    }
};

int main()
{
    
    Mat image = imread("images/lena.png", CV_LOAD_IMAGE_COLOR);

    if (image.empty()) {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }
    Mat image_gray;
    cvtColor(image, image_gray, CV_BGR2GRAY);
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize(image_gray.rows);
    int n = getOptimalDFTSize(image_gray.cols); // on the border add zero values
    copyMakeBorder(image_gray, padded, 0, m - image_gray.rows, 0, n - image_gray.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix
    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))

    int M = 10;
    double a, b, c;
    cin >> a >> b >> c;
    double function[M];
    for (int i = 0; i < M; i++) {
        function[i] = (((a * (double)i) + (b * (double)i)) - c);
        //System.out.print( " "+function[i] + " ");
    }

    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;
    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
    normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).


    phase;

    Mat image_phase = atan(planes[0] /planes[1]);

    imshow("Input Image", image_gray);    // Show the result
    imshow("spectrum magnitude", magI);
    waitKey();
    return EXIT_SUCCESS;
}