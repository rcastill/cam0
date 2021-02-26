#include <cam0.h>

extern "C" void filter(cv::Mat& im)
{
    cv::flip(im, im, 1);
}