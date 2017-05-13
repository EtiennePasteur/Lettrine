#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "BlobResult.h"

using namespace cv;
using namespace std;


void bwareaopen(Mat &img, int size) {
    CBlobResult blobs;
    blobs = CBlobResult(img, Mat(), 4);
    blobs.Filter(blobs, B_INCLUDE, CBlobGetLength(), B_GREATER, size);

    Mat newimg(img.size(), img.type());
    newimg.setTo(0);
    for (int i = 0; i < blobs.GetNumBlobs(); i++) {
        blobs.GetBlob(i)->FillBlob(newimg, CV_RGB(255, 255, 255), 0, 0, true);
    }
    img = newimg;
}

static void newFind(const Mat &image, vector<vector<Point> > &squares) {
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    //Extract the contours so that
    vector<vector<Point> > contours0;
    findContours(image, contours0, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    contours.resize(contours0.size());
    for (size_t k = 0; k < contours0.size(); k++)
        approxPolyDP(Mat(contours0[k]), contours[k], 3, true);

    Mat drawing = Mat::zeros(image.size().height, image.size().width, CV_8UC3);





    drawContours(drawing, contours, 3,  Scalar(0,255,0), 1, 8, vector<Vec4i>(), 0, Point());

    for (int i = 0; i < contours.size(); i++)
    {
        std::cout << i << std::endl;
    }
    imshow("Lettrine", drawing);

}

void extractPics(const std::string &path) {
    namedWindow("Lettrine", 1);
    vector<vector<Point>> squares;

    Mat imageP = imread(path, 1);

    cvtColor(imageP, imageP, CV_RGB2GRAY);
    threshold(imageP, imageP, 75.0, 255.0, THRESH_BINARY_INV);

    bwareaopen(imageP, 800);

    resize(imageP, imageP, Size(0, 0), 0.4, 0.4);

    newFind(imageP, squares);

    for (;;) {
        int c = waitKey();
        if (c == 13 || c == 10)
            break;
        if (c == 27)
            exit(0);
    }
}
