// DIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using namespace cv;

void gammaCorrection(const Mat& src, Mat& dst, const float gamma)
{
    float invGamma = 1 / gamma;

    Mat table(1, 256, CV_8U);
    uchar* p = table.ptr();
    for (int i = 0; i < 256; ++i) {
        p[i] = (uchar)(pow(i / 255.0, invGamma) * 255);
    }

    LUT(src, table, dst);
}


int main()
{
	cv::Mat image = cv::imread("images/moon.jpg", CV_LOAD_IMAGE_COLOR); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)

	if (image.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

    Mat new_image = Mat::zeros(image.size(), image.type());
    double alpha = 1.0; //contrast
    int beta = 0;      //brightness
    cout << " Basic Linear Transforms " << endl;
    cout << "-------------------------" << endl;
    cout << "* Enter the alpha value [1.0-3.0]: "; cin >> alpha;
    cout << "* Enter the beta value [0-100]: ";    cin >> beta;
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int c = 0; c < image.channels(); c++) {
                new_image.at<Vec3b>(y, x)[c] =
                    saturate_cast<uchar>(alpha * image.at<Vec3b>(y, x)[c] + beta);
            }
        }
    }

    Mat gammaIMG;
    gammaCorrection(image, gammaIMG, 2.2);

    imshow("Original Image", image);
    imshow("New Image", new_image);
    waitKey(0);
    return 0;
}
