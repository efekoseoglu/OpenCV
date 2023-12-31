// DIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;

int main()
{
    cv::Mat img_color = cv::imread("images/lena.png", CV_LOAD_IMAGE_COLOR);
  

    if (img_color.empty()) {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }

    Mat img_gray;
    cvtColor(img_color, img_gray, cv::COLOR_BGR2GRAY);

    Mat_<float>kernel(3, 3);
    kernel<<1, 1, 1, 1, 1, 1, 1, 1, 1;

    Mat dst = img_gray.clone();

    const int dx = kernel.cols / 2;
    const int dy = kernel.rows / 2;

    for (int i = 1; i < img_gray.rows - 1; i++)//starting point changed 
    {
        for (int j = 1; j < img_gray.cols - 1; j++)
        {
            float tmp = 0.0f;
            for (int k = 0; k < kernel.rows; k++)
            {
                for (int l = 0; l < kernel.cols; l++)
                {
                    int x = j - dx + l;
                    int y = i - dy + k;
                    if (x >= 0 && x < img_gray.cols && y >= 0 && y < img_gray.rows)
                        tmp += img_gray.at<uchar>(y, x) * kernel.at<float>(k, l);// not float , uchar
                }
            }
            dst.at<uchar>(i, j) = saturate_cast<float>(tmp);
        }
    }

    imshow("Original Image", img_gray);
    imshow("Blurred Image", dst);

    
    waitKey(0);
    return 0;
}
