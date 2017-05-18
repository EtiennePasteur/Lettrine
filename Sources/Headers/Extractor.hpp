//
// Created by aguadoe on 18/05/17.
//

#ifndef LETTRINE_EXTRACTOR_HPP
#define LETTRINE_EXTRACTOR_HPP

#include <iostream>

// Including OpenCv and Bloblib which allow the remove small things on image like characters
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "BlobResult.h"


struct t_pos_contour {
    cv::Point max;
    cv::Point min;
};

//Entry point function that call all other functions.
void extractPics(const std::string &path, const std::string &destPath);


#endif //LETTRINE_EXTRACTOR_HPP
