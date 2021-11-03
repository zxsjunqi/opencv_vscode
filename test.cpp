#include"opencv2/opencv.hpp"
#include<opencv2/highgui.hpp>
#include<iostream>
using namespace cv;
using namespace std;
int main()
{
    Mat img = imread("D:\\png\\whitegirl.png");
    cv::imshow("image", img);
    cv::waitKey();
    return 0;
}
