#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>

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

using namespace std;
using namespace cv;

namespace exp_5
{

void init(Mat pic)
{
    Size pic_size = pic.size();
    for (int i = 0; i < pic_size.height; i++)
    {
        for (int j = 0;j < pic_size.width; j++)
        {
            pic.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
        }
    }
}
void pic_init(Mat pic)
{
    for (int i = 0; i < pic.size().height; i++)
    {
        for (int j = 0; j < pic.size().width; j++)
        {
            pic.at<uchar>(i, j) = 127;
        }
    }
}
int inboundary(Size s, Point p, int size = 5, double arg = 1)
{
    if (p.x<0 || p.x>s.width || p.y<0 || p.y>s.height)
    {
        return 0;
    }
    return 1;
}

int avl_filter(Mat pic, Point p, int size = 5, double arg = 1)
{
    int count = 0;
    int add = 0;
    Point ref(p.x - 2, p.y - 2);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (inboundary(pic.size(), Point(ref.x + i, ref.y + j)))
            {
                count++;
                add += pic.at<uchar>(Point(ref.x + i, ref.y + j));
            }
        }
    }
    return add / count;
}

int geo_filter(Mat pic, Point p, int size = 5, double arg = 1)
{
    int count_in = 0;
    int count_out = 0;
    double result = 1;
    double round_multi = 1;
    Point ref(p.x - 2, p.y - 2);

    for (int i = 0; i < 5; i++)
    {
        round_multi = 1;
        count_in = 0;
        for (int j = 0; j < 5; j++)
        {
            if (inboundary(pic.size(), Point(ref.x + i, ref.y + j)))
            {
                count_in++;
                //cout << "a:" << (int)pic.at<uchar>(Point(ref.x + i, ref.y + j)) << endl;
                round_multi *= pic.at<uchar>(Point(ref.x + i, ref.y + j)) < 1 ? 1 : pic.at<uchar>(Point(ref.x + i, ref.y + j));
            }
        }
        count_out++;
        if (count_in != 0)
        {
            result *= pow(round_multi, (1.0 / count_in));
            //cout << "pow(" << round_multi << ", " << 1.0 / count_in << "):" << pow(round_multi, 1.0 / count_in) << endl;
        }
        //cout << "result:" << result << endl;
    }
    //cout << "final:" << pow(result, 1.0 / count_out) << endl;
    return (int)pow(result, (1.0 / count_out));
}

int har_filter(Mat pic, Point p, int size = 5, double arg = 1)
{
    double count = 0;
    double add = 0;
    Point ref(p.x - 2, p.y - 2);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (inboundary(pic.size(), Point(ref.x + i, ref.y + j)))
            {
                count++;
                int temp = pic.at<uchar>(Point(ref.x + i, ref.y + j));
                temp = temp < 1 ? 1 : temp;
                add += 1.0 / temp;
            }
        }
    }
    //int result_log = log(count) - log(add);
    //return exp(result_log);
    return int(count / add);
}

int inver_har_filter(Mat pic, Point p, int size, double q)
{
    int add_q = 0;
    int add_q1 = 0;
    int r = (size - 1) / 2;
    Point ref(p.x - r, p.y - r);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (inboundary(pic.size(), Point(ref.x + i, ref.y + j)))
            {
                int temp = pic.at<uchar>(Point(ref.x + i, ref.y + j));
                temp = temp < 0 ? 1 : temp;
                add_q += pow(temp, q);
                add_q1 += pow(temp, q + 1);
            }
        }
    }
    return int(add_q1 / add_q);
}

int mid_filter(Mat pic, Point p, int size, double arg = 1)
{
    int count = 0;
    int record[81];
    int r = (size - 1) / 2;
    Point ref(p.x - r, p.y - r);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (inboundary(pic.size(), Point(ref.x + i, ref.y + j)))
            {
                record[count] = pic.at<uchar>(Point(ref.x + i, ref.y + j));
                count++;
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count-1; j++)
        {
            if (record[j] < record[j + 1])
            {
                int temp = record[j];
                record[j] = record[j + 1];
                record[j + 1] = temp;
            }
        }
    }
    return record[size*r + r];
}

int adapt_avl_filter(Mat pic, Point p, int size, double sigma)
{
    int avl = avl_filter(pic, p);
    int count = 0;
    int add = 0;
    int r = (size - 1) / 2;
    Point ref(p.x - r, p.y - r);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (inboundary(pic.size(), Point(ref.x + i, ref.y + j)))
            {
                count++;
                add += pow(pic.at<uchar>(Point(ref.x + i, ref.y + j)) - avl, 2);
            }
        }
    }

    int sigma_l = add / count;
    int result = pic.at<uchar>(p) - sigma / sigma_l * (pic.at<uchar>(p) - avl);
    if(result < 0)
    {
        return 0;
    }
    if(result > 255)
    {
        return 255;
    }
    return result;
}

int adapt_mid_filter(Mat pic, Point p, int size, double arg = 1)
{
    int count = 0;
    int record[169];
    int r = (size - 1) / 2;
    Point ref(p.x - r, p.y - r);

    int max = 0;
    int min = 255;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (inboundary(pic.size(), Point(ref.x + i, ref.y + j)))
            {
                count++;
                record[count] = pic.at<uchar>(Point(ref.x + i, ref.y + j));
                if (record[count] > max)
                {
                    max = record[count];
                }
                else if (record[count] < min)
                {
                    min = record[count];
                }
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count - 1; j++)
        {
            if (record[j] < record[j + 1])
            {
                int temp = record[j];
                record[j] = record[j + 1];
                record[j + 1] = temp;
            }
        }
    }

    if (record[size*r + r] > min && record[size*r + r] < max)
    {
        int value = pic.at<uchar>(p);
        if (value > min && value < max)
        {
            return value;
        }
        else
        {
            return record[size*r + r];
        }
    }
    else
    {
        if (size + 2 <= 13)
        {
            adapt_mid_filter(pic, p, size + 2);
        }
        else
        {
            return record[size*r + r];
        }
    }
}

Mat traversal(Mat img, int(*handle)(Mat, Point, int, double), int size = 5, double arg = 1)
{
    Mat result(img.size().height, img.size().width, CV_8U);
    for (int i = 0; i < img.size().height; i++)
    {
        for (int j = 0; j < img.size().width; j++)
        {
            result.at<uchar>(i, j) = handle(img, Point(j, i), size, arg);
        }
    }
    return result;
}

class noise
{
    double p[256];
    int noise_type;
    double spratio = 1;
public:
    noise(int type, double arg = 30, double arg2 = 5)
    {
        if (type == 0)
        {
            noise_type = 0;
            int avl = arg;
            int sigma = arg2;

            for (int i = 0; i < 256; i++)
            {
                p[i] = COEFF / sigma * exp(-(i - avl)*(i - avl) / (2 * sigma*sigma));
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
    void view()
    {
        for (int i = 0; i < 256; i++)
        {
            cout << "p[" << i << "]=" << p[i] << endl;
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
        for (int i = 0; i < pic_size.height; i++)
        {
            for (int j = 0; j < pic_size.width; j++)
            {
                int temp = pic.at<uchar>(i, j) + noise_cal();
                if (temp < 0)
                {
                    pic.at<uchar>(i, j) = 0;
                }
                else if (temp > 255)
                {
                    pic.at<uchar>(i, j) = 255;
                }
                else
                {
                    pic.at<uchar>(i, j) = temp;
                }
                //system("pause");
            }
        }
        return pic;
    }
};

/*
void noise_test(noise a, noise b, noise c, noise d)
{
    Mat noise_test_pic(200, 200, CV_8U);
    pic_init(noise_test_pic);

    Mat temp = noise_test_pic.clone();
    show(temp);

    channel origin(temp);
    show(origin.hist_graph());

    show(a.polution(temp));
    channel a_pol(a.polution(temp));
    show(a_pol.hist_graph());

    temp = noise_test_pic.clone();

    show(b.polution(temp));
    channel b_pol(b.polution(temp));
    show(b_pol.hist_graph());

    temp = noise_test_pic.clone();

    show(c.polution(temp));
    channel c_pol(c.polution(temp));
    show(c_pol.hist_graph());

    temp = noise_test_pic.clone();

    show(d.polution(temp));
    channel d_pol(d.polution(temp));
    show(d_pol.hist_graph());
}

/*
int main()
{
    srand(time(NULL));

    noise guass(GUASS, GUASS_AVL);
    noise pep_sal(PEP_SAL, SNP_RAT, SDP_RAT);
    noise pepper(PEPPER, PEPPER_RAT);
    noise salt(SALT, SALT_RAT);

    //noise_test(guass, pep_sal, pepper, salt);
    Mat img = imread("d:/test0.jpg");
    cvtColor(img, img, COLOR_BGR2GRAY);

    Mat poluted = pep_sal.polution(img);
    channel pic(poluted);


    show(pic.origin());
    //show(pic.traversal(avl_filter));
    //show(pic.traversal(geo_filter));
    //show(pic.traversal(har_filter));
    //show(pic.traversal(inver_har_filter, 5, 1.5));
    show(pic.traversal(mid_filter, 5));
    //show(pic.traversal(adapt_avl_filter, 3, 1000));
    show(pic.traversal(adapt_mid_filter, 3));
}
*/

}
