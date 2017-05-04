#ifndef EXP3_H
#define EXP3_H

#define MAXSIZE 9

namespace exp_3
{
#define AVL 1
#define GUASS 2
#define LAP 3
#define LAP_SHARPEN 4
#define ROB_A 5
#define ROB_B 6
#define SOBEL_A 7
#define SOBEL_B 8

#define GUASS_DELTA 2

#define MAXSIZE 9

using namespace cv;
using namespace std;

int inboundary(Size s, Point p)
{
    if (p.x<0 || p.x>s.width || p.y<0 || p.y>s.height)
    {
        return 0;
    }
    return 1;
}

class filter
{
    int size = 0;
    double value[MAXSIZE][MAXSIZE];
    int tag = 0;
public:
    filter(int s, double v[], int t)
    {
        size = s;
        for (int i = 0; i < s; i++)
        {
            for (int j = 0; j < s; j++)
            {
                value[i][j] = v[i*s + j];
            }
        }
        tag = t;
    }
    int cal(Mat ipt, Point p)
    {
        Size s = ipt.size();
        double sum = 0, r = (size - 1) / 2;
        Point ref = Point(p.x - r, p.y - r);

        if (tag == 0)
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if (inboundary(s, ref + Point(i, j)))
                    {
                        sum += value[i][j];
                    }
                }
            }
            double divide = 1 / sum;

            double result = 0;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if (inboundary(s, ref + Point(i, j)))
                    {
                        result += ipt.at<uchar>(ref + Point(i, j)) * value[i][j] * divide;
                    }
                }
            }

            return (int)(result + 0.5);
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if (!inboundary(s, ref + Point(i, j)))
                    {
                        sum += value[i][j];
                    }
                }
            }

            value[1][1] += sum;
            double result = 0;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if (inboundary(s, ref + Point(i, j)))
                    {
                        result += ipt.at<uchar>(ref + Point(i, j)) * value[i][j];
                    }
                }
            }
            value[1][1] -= sum;

            if (result > 255)
            {
                return 255;
            }
            if (result < 0)
            {
                return 0;
            }
            return (int)(result + 0.5);
        }
    }
};

class gray
{
    Mat pic;
public:
    gray()
    {

    }
    gray(Mat ipt)
    {
        pic = ipt.clone();
    }
    Mat filt(filter f)
    {
        Mat result = pic.clone();
        for (int i = 0; i < result.size().height; i++)
        {
            for (int j = 0; j < result.size().width; j++)
            {
                result.at<uchar>(i, j) = f.cal(pic, Point(j, i));
            }
        }
        return result;
    }
    Mat filt(filter f1, filter f2)
    {
        Mat result = pic.clone();
        int tempadd = 0;
        for (int i = 0; i < result.size().height; i++)
        {
            for (int j = 0; j < result.size().width; j++)
            {
                tempadd = abs(f1.cal(pic, Point(j, i))) + abs(f2.cal(pic, Point(j, i)));
                result.at<uchar>(i, j) = tempadd > 255 ? 255 : tempadd;
            }
        }
        return result;
    }
    Mat filt_sharpen(filter f1, filter f2)
    {
        Mat result = pic.clone();
        int tempadd = 0;
        for (int i = 0; i < result.size().height; i++)
        {
            for (int j = 0; j < result.size().width; j++)
            {
                tempadd = (abs(f1.cal(pic, Point(j, i))) + abs(f2.cal(pic, Point(j, i)))) * 0.5 + pic.at<uchar>(i, j);
                result.at<uchar>(i, j) = tempadd > 255 ? 255 : tempadd;
            }
        }
        return result;
    }
};

class color
{
    Mat pic;
    gray r;
    gray g;
    gray b;
public:
    gray divide(Mat ipt, int tag)
    {
        int height = ipt.size().height;
        int width = ipt.size().width;

        Mat m(height, width, CV_8U);

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                m.at<uchar>(i, j) = ipt.at<Vec3b>(i, j)[tag];
            }
        }

        return gray(m);
    }
    color(Mat ipt)
    {
        pic = ipt.clone();
        b = divide(ipt, 0);
        g = divide(ipt, 1);
        r = divide(ipt, 2);
    }
    Mat filt(filter f)
    {
        Mat result(pic.size().height, pic.size().width, CV_8UC3);
        Mat bf = b.filt(f);
        Mat gf = g.filt(f);
        Mat rf = r.filt(f);
        for (int i = 0; i < pic.size().height; i++)
        {
            for (int j = 0; j < pic.size().width; j++)
            {
                result.at<Vec3b>(i, j)[0] = bf.at<uchar>(i, j);
                result.at<Vec3b>(i, j)[1] = gf.at<uchar>(i, j);
                result.at<Vec3b>(i, j)[2] = rf.at<uchar>(i, j);
            }
        }
        return result;
    }
    Mat filt(filter f1, filter f2)
    {
        Mat result(pic.size().height, pic.size().width, CV_8UC3);
        Mat bf = b.filt(f1, f2);
        Mat gf = g.filt(f1, f2);
        Mat rf = r.filt(f1, f2);
        for (int i = 0; i < pic.size().height; i++)
        {
            for (int j = 0; j < pic.size().width; j++)
            {
                result.at<Vec3b>(i, j)[0] = bf.at<uchar>(i, j);
                result.at<Vec3b>(i, j)[1] = gf.at<uchar>(i, j);
                result.at<Vec3b>(i, j)[2] = rf.at<uchar>(i, j);
            }
        }
        return result;
    }
    Mat filt_sharpen(filter f1, filter f2)
    {
        Mat result(pic.size().height, pic.size().width, CV_8UC3);
        Mat bf = b.filt_sharpen(f1, f2);
        Mat gf = g.filt_sharpen(f1, f2);
        Mat rf = r.filt_sharpen(f1, f2);
        for (int i = 0; i < pic.size().height; i++)
        {
            for (int j = 0; j < pic.size().width; j++)
            {
                result.at<Vec3b>(i, j)[0] = bf.at<uchar>(i, j);
                result.at<Vec3b>(i, j)[1] = gf.at<uchar>(i, j);
                result.at<Vec3b>(i, j)[2] = rf.at<uchar>(i, j);
            }
        }
        return result;
    }
};

filter filter_genarate(int type, int size)
{
    double temp[MAXSIZE * MAXSIZE];
    if (type == 1)
    {
        for (int i = 0; i < size*size; i++)
        {
            temp[i] = 1;
        }
        return filter(size, temp, 0);
    }
    else if (type == 2)
    {
        double center = size / 2;
        for (int i = 0; i < size*size; i++)
        {
            temp[i] = exp(-((center - i / size)*(center - i / size) + (center - i % size)*(center - i % size)) / (GUASS_DELTA*GUASS_DELTA * 2));
        }
        return filter(size, temp, 0);
    }
    else if (type == 3)
    {
        double temp1[9] = { 0,-1,0,-1,4,-1,0,-1,0 };

        return filter(3, temp1, 1);
    }
    else if (type == 4)
    {
        double temp1[9] = { 0,-1,0,-1,5,-1,0,-1,0 };

        return filter(3, temp1, 1);
    }
    else if (type == 5)
    {
        double temp1[9] = { 0,0,0,0,-1,0,0,0,1 };

        return filter(3, temp1, 1);
    }
    else if (type == 6)
    {
        double temp1[9] = { 0,0,0,0,0,-1,0,1,0 };

        return filter(3, temp1, 1);
    }
    else if (type == 7)
    {
        double temp1[9] = { -1,-2,-1,0,0,0,1,2,1 };

        return filter(3, temp1, 1);
    }
    else if (type == 8)
    {
        double temp1[9] = { -1,0,1,-2,0,2,-1,0,1 };

        return filter(3, temp1, 1);
    }
}
}



#endif // EXP3_H
