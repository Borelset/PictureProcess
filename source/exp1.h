#ifndef EXP1_H
#define EXP1_H
#include <opencv2/core/core.hpp>

namespace exp_1
{

using namespace cv;

void show(Mat img);

uchar gray_convert(Vec3b ipt, double arg = 1);
uchar bin_convert(uchar ipt, double arg = 127);
uchar log_convert(uchar ipt, double arg = 2);
uchar multi_convert(uchar ipt, double arg = 2);
Vec3b opp_convert(Vec3b ipt, double arg = 1);
Vec3b com_convert(Vec3b ipt, double arg = 1);

template<class T, class P>
Mat convert(Mat img, T(*handle)(P, double), double arg = 1)
{
    int width = img.size().width;
    int height = img.size().height;

    Mat result;
    if (sizeof(T) == sizeof(P))
    {
        result = img.clone();
    }
    else
    {
        result = Mat(height, width, CV_8U);
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            result.at<T>(i, j) = handle(img.at<P>(i, j), arg);
        }
    }
    return result;
}
}

#endif // EXP1_H
