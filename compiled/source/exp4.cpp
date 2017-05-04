#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

namespace exp_4
{
using namespace cv;

Mat ideal_highpass(Mat input, double range = 60000, int arg = 1)
{
    int midx = input.size().width / 2;
    int midy = input.size().height / 2;

    Mat result = input.clone();
    for (int i = 0; i < input.size().height; i++)
    {
        for (int j = 0; j < input.size().width; j++)
        {
            if ((i - midx)*(i - midx) + (j - midy)*(j - midy)> range)
            {
                result.at<float>(i, 2 * j) = 0;
                result.at<float>(i, 2 * j + 1) = 0;
            }
        }
    }
    return result;
}

Mat ideal_lowpass(Mat input, double range = 60000, int arg = 1)
{
    int midx = input.size().width / 2;
    int midy = input.size().height / 2;

    Mat result = input.clone();
    for (int i = 0; i < input.size().height; i++)
    {
        for (int j = 0; j < input.size().width; j++)
        {
            if ((i - midx)*(i - midx) + (j - midy)*(j - midy)< range)
            {
                result.at<float>(i, 2 * j) = 0;
                result.at<float>(i, 2 * j + 1) = 0;
            }
        }
    }
    return result;
}

Mat butterworth_highpass(Mat input, double range = 30000, int arg = 5)
{
    int midx = input.size().width / 2;
    int midy = input.size().height / 2;

    Mat result = input.clone();
    for (double i = 0; i < input.size().height; i++)
    {
        for (int j = 0; j < input.size().width; j++)
        {
            double index = 1 / (1 + pow(((i - midx)*(i - midx) + (j - midy)*(j - midy)) / range, arg));
            result.at<float>(i, 2 * j) *= index;
            result.at<float>(i, 2 * j + 1) *= index;
        }
    }
    return result;
}

Mat butterworth_lowpass(Mat input, double range = 50000, int arg = 5)
{
    int midx = input.size().width / 2;
    int midy = input.size().height / 2;

    Mat result = input.clone();
    for (int i = 0; i < input.size().height; i++)
    {
        for (int j = 0; j < input.size().width; j++)
        {
            double index = 1 / (1 + pow(range / ((i - midx)*(i - midx) + (j - midy)*(j - midy)), arg));
            result.at<float>(i, 2 * j) *= index;
            result.at<float>(i, 2 * j + 1) *= index;
        }
    }
    return result;
}

Mat spectrum(Mat img, Mat (*handle)(Mat, double, int),double range = 50000, int arg = 5)
{
    Mat planes[] = { cv::Mat_<float>(img), cv::Mat::zeros(img.size(), CV_32F) };
    Mat complexI;	//Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
    merge(planes, 2, complexI);
    dft(complexI, complexI);  // Applying DFT

    complexI = handle(complexI, range, arg);

    Mat invDFT, invDFTcvt;
    idft(complexI, invDFT, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT); // Applying IDFT
    invDFT.convertTo(invDFTcvt, CV_8U);
    return invDFTcvt;
}
}
