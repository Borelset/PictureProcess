#ifndef EXP5_H
#define EXP5_H

namespace exp_5
{
#include <opencv2/core/core.hpp>
#define GUASS 0
#define PEP_SAL 1
#define PEPPER 2
#define SALT 3

#define GUASS_AVL 50
#define SALT_RAT 0.1
#define PEPPER_RAT 0.1
#define SNP_RAT 0.5
#define SDP_RAT 1

#define SIGMA 5
#define COEFF 0.4

#define INVER_HAR_Q 1.5

using namespace cv;

void pic_init(Mat pic);

int avl_filter(Mat pic, Point p, int size = 5, double arg = 1);
int geo_filter(Mat pic, Point p, int size = 5, double arg = 1);
int har_filter(Mat pic, Point p, int size = 5, double arg = 1);
int inver_har_filter(Mat pic, Point p, int size, double q);
int mid_filter(Mat pic, Point p, int size, double arg = 1);
int adapt_avl_filter(Mat pic, Point p, int size, double sigma);
int adapt_mid_filter(Mat pic, Point p, int size, double arg = 1);
Mat traversal(Mat img, int(*handle)(Mat, Point, int, double), int size = 5, double arg = 1);

class noise
{
    double p[256];
    int noise_type;
    double spratio = 1;
public:
    noise(int type, double arg, double arg2 = 1)
    {
        if (type == 0)
        {
            noise_type = 0;
            int avl = arg;

            for (int i = 0; i < 256; i++)
            {
                p[i] = COEFF / SIGMA * exp(double(-(i - avl)*(i - avl)) / (2 * SIGMA*SIGMA));
            }
        }
        if (type == 1)
        {
            noise_type = 2;
            spratio = arg2;

            for (int i = 0; i < 256; i++)
            {
                p[i] = 0;
            }
            p[0] = 1 - arg;
            p[255] = arg;
        }
        if (type == 2)
        {
            noise_type = 1;

            for (int i = 0; i < 256; i++)
            {
                p[i] = 0;
            }
            p[0] = 1 - arg;
            p[255] = arg;
        }
        if (type == 3)
        {
            noise_type = 0;

            for (int i = 0; i < 256; i++)
            {
                p[i] = 0;
            }
            p[0] = 1 - arg;
            p[255] = arg;
        }
    }
    int noise_cal()
    {
        int p_total = 0;
        for (int i = 0; i < 256; i++)
        {
            p_total += p[i] * 1000;
        }
        int random_num = rand() % p_total;
        int test_total = p[0] * 1000;
        //cout << "ramdom_num=" << random_num << endl;
        for (int i = 0; i < 255; i++)
        {
            //cout << "test_total:" << test_total << endl;
            if (random_num < test_total)
            {
                //cout << "result:" << i << endl;
                if (noise_type == 0)
                {
                    return i;
                }
                else if(noise_type == 1)
                {
                    return -i;
                }
                else
                {
                    int tag = rand() % 100;
                    if (tag < 100 * spratio / (1 + spratio))
                    {
                        return i;
                    }
                    else
                    {
                        return -i;
                    }
                }
            }
            else
            {
                test_total += p[i+1] * 1000;
            }
        }
        //cout << "result:" << 255 << endl;
        if (noise_type == 0)
        {
            return 255;
        }
        else if (noise_type == 1)
        {
            return -255;
        }
        else
        {
            int tag = rand() % 100;
            if (tag < 100 * spratio / (1 + spratio))
            {
                return 255;
            }
            else
            {
                return -255;
            }
        }
    }
    Mat polution(Mat pic)
    {
        Size pic_size = pic.size();
        Mat result = pic.clone();
        for (int i = 0; i < pic_size.height; i++)
        {
            for (int j = 0; j < pic_size.width; j++)
            {
                int temp = pic.at<uchar>(i, j) + noise_cal();
                if (temp < 0)
                {
                    result.at<uchar>(i, j) = 0;
                }
                else if (temp > 255)
                {
                    result.at<uchar>(i, j) = 255;
                }
                else
                {
                    result.at<uchar>(i, j) = temp;
                }
                //system("pause");
            }
        }
        return result;
    }
};

}
#endif // EXP5_H
