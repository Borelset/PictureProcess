#ifndef EXP2_H
#define EXP2_H
#include <opencv2/core/core.hpp>

using namespace cv;

namespace exp_2
{
void init(Mat ipt)
{
    int height = ipt.size().height;
    int width = ipt.size().width;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            ipt.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
        }
    }
}

class channel
{
private:
    Mat img;
    int width;
    int height;
    int n[256] = { 0 };
    double p[256] = { 0.0 };
    double c[256] = { 0.0 };
    double maxp = 0.0;
public:
    channel()
    {

    }
    channel(Mat ipt)
    {
        img = ipt.clone();
        width = ipt.size().width;
        height = ipt.size().height;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                n[ipt.at<uchar>(i, j)]++;
            }
        }

        for (int i = 0; i < 256; i++)
        {
            p[i] = (double)n[i] / (double)(width * height);
            if (p[i] > maxp) maxp = p[i];
        }

        for (int i = 0; i < 256; i++)
        {
            for (int j = 0; j < i; j++)
            {
                c[i] += p[j];
            }
        }
    }
    Mat hist_graph(Scalar color = Scalar(255, 255, 255))
    {
        Mat result = Mat(200, 256, CV_8UC3);
        init(result);
        for (int i = 0; i < 256; i++)
        {
            line(result, Point(i, 100), Point(i, 100 - c[i] * 100), color, 1);
            line(result, Point(i, 200), Point(i, 200 - (p[i]/maxp) * 100), color, 1);
        }
        return result;
    }
    Mat hist_balance()
    {
        Mat result = img.clone();
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                result.at<uchar>(i, j) = int(c[img.at<uchar>(i, j)] * 255);
            }
        }
        return result;
    }
    Mat origin()
    {
        return img;
    }
};

class multichannel
{
private:
    Mat img;
    int height;
    int width;
    channel cr;
    channel cg;
    channel cb;
public:
    channel divide(Mat img, int cn)
    {
        Mat temp(height, width, CV_8U);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                temp.at<uchar>(i, j) = img.at<Vec3b>(i, j)[cn];
            }
        }
        return channel(temp);
    }
    multichannel(Mat ipt)
    {
        img = ipt.clone();
        height = img.size().height;
        width = img.size().width;
        cb = divide(img, 0);
        cg = divide(img, 1);
        cr = divide(img, 2);
    }
    Mat fusion(channel cr, channel cg, channel cb)
    {
        Mat mr = cr.hist_balance();
        Mat mg = cg.hist_balance();
        Mat mb = cb.hist_balance();
        Mat result = img.clone();
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                result.at<Vec3b>(i, j)[0] = mb.at<uchar>(i, j);
                result.at<Vec3b>(i, j)[1] = mg.at<uchar>(i, j);
                result.at<Vec3b>(i, j)[2] = mr.at<uchar>(i, j);
            }
        }
        return result;
    }
    Mat hist_balance()
    {
        return fusion(cr, cg, cb);
    }
    Mat hist_graph()
    {
        Mat temp1 = cr.hist_graph(Scalar(0, 0, 255));
        Mat temp2 = cg.hist_graph(Scalar(0, 255, 0));
        Mat temp3 = cb.hist_graph(Scalar(255, 0, 0));

        Mat result(600, 256, CV_8UC3);

        for (int i = 0; i < 200; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                result.at<Vec3b>(i, j) = temp1.at<Vec3b>(i, j);
            }
        }

        for (int i = 0; i < 200; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                result.at<Vec3b>(i+200, j) = temp2.at<Vec3b>(i, j);
            }
        }

        for (int i = 0; i < 200; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                result.at<Vec3b>(i+400, j) = temp3.at<Vec3b>(i, j);
            }
        }

        return result;
    }
    Mat origin()
    {
        return img;
    }
};
}

#endif // EXP2_H
