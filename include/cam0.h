#pragma once

#include <opencv2/opencv.hpp>

namespace cam0 {

typedef void (*filter)(cv::Mat& im);

}