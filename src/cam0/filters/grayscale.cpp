#include <cam0.h>

extern "C" void filter(cv::Mat& im)
{
    cv::cvtColor(im, im, cv::COLOR_BGR2GRAY);
}