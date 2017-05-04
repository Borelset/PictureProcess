#ifndef EXP4_H
#define EXP4_H

namespace exp_4
{
#include <opencv2/core/core.hpp>
using namespace cv;

Mat ideal_highpass(Mat input, double range = 60000, int arg = 1);
Mat ideal_lowpass(Mat input, double range = 60000, int arg = 1);
Mat butterworth_highpass(Mat input, double range = 30000, int arg = 5);
Mat butterworth_lowpass(Mat input, double range = 50000, int arg = 5);
Mat spectrum(Mat img, Mat (*handle)(Mat, double, int),double range = 50000, int arg = 5);
}

#endif // EXP4_H
