#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <cmath>
#include <cstdio>

#define BIN 127
#define LOG_R 2
#define MULTI_R 2

using namespace cv;

namespace exp_1
{

template<class T, class P>
Mat convert(Mat img, T(*handle)(P))
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
            result.at<T>(i, j) = handle(img.at<P>(i, j));
        }
    }
    return result;
}

/*
int main(int argc, void* argv[])
{
    if (argc != 2) return -1;
    Mat pic = imread((char*)argv[1]);
    show(pic);
    Mat gray = convert<uchar, Vec3b>(pic, gray_convert);
    show(gray);
    show(convert<uchar>(gray, bin_convert));
    show(convert<uchar>(gray, log_convert));
    show(convert<uchar>(gray, multi_convert));
    show(convert<Vec3b>(pic, opp_convert));
    show(convert<Vec3b>(pic, com_convert));
}
*/

uchar max(uchar a, uchar b, uchar c)
{
    if (a > b)
    {
        if (a > c)
        {
            return a;
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b > c)
        {
            return b;
        }
        else
        {
            return c;
        }
    }
}
uchar min(uchar a, uchar b, uchar c)
{
    if (a < b)
    {
        if (a < c)
        {
            return a;
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b < c)
        {
            return b;
        }
        else
        {
            return c;
        }
    }
}

uchar gray_convert(Vec3b ipt, double arg = 1)
{
    return (ipt[0] * 114 + ipt[1] * 587 + ipt[2] * 299 + 500) / 1000;
}
uchar bin_convert(uchar ipt, double arg = 127)
{
    return (uchar)ipt > arg ? 255 : 0;
}
uchar log_convert(uchar ipt, double arg = 2)
{
    double log_c = 255.0 / (log(255) / log(arg));
    return (uchar)(log_c * (log(ipt + 1) / log(arg)));
}
uchar multi_convert(uchar ipt, double arg = 2)
{
    double multi_c = 255.0 / (pow(255, arg));
    return (uchar)(multi_c * pow(ipt, arg));
}
Vec3b opp_convert(Vec3b ipt, double arg = 1)
{
    ipt[0] = 255 - ipt[0];
    ipt[1] = 255 - ipt[1];
    ipt[2] = 255 - ipt[2];
    return ipt;
}
Vec3b com_convert(Vec3b ipt, double arg = 2)
{
    uchar maxone = max(ipt[0], ipt[1], ipt[2]);
    uchar minone = min(ipt[0], ipt[1], ipt[2]);
    ipt[0] = maxone + minone - ipt[0];
    ipt[1] = maxone + minone - ipt[1];
    ipt[2] = maxone + minone - ipt[2];
    return ipt;
}

}
