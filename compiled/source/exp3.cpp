#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>


/*
template<class T>
void demo(Mat ipt)
{
    show(ipt);
    T obj(ipt);

    filter f_avl_3 = filter_genarate(AVL, 3);
    f_avl_3.view();
    show(obj.filt(f_avl_3));

    filter f_avl_5 = filter_genarate(AVL, 5);
    f_avl_5.view();
    show(obj.filt(f_avl_5));

    filter f_avl_9 = filter_genarate(AVL, 9);
    f_avl_9.view();
    show(obj.filt(f_avl_9));

    filter f_guass_3 = filter_genarate(GUASS, 3);
    f_guass_3.view();
    show(obj.filt(f_guass_3));

    filter f_guass_5 = filter_genarate(GUASS, 5);
    f_guass_5.view();
    show(obj.filt(f_guass_5));

    filter f_guass_9 = filter_genarate(GUASS, 9);
    f_guass_9.view();
    show(obj.filt(f_guass_9));

    filter f_lap = filter_genarate(LAP, 3);
    f_lap.view();
    show(obj.filt(f_lap));
    filter f_lap_sharpen = filter_genarate(LAP_SHARPEN, 3);
    f_lap_sharpen.view();
    show(obj.filt(f_lap_sharpen));

    filter f_rob_A = filter_genarate(ROB_A, 3);
    f_rob_A.view();
    filter f_rob_B = filter_genarate(ROB_B, 3);
    f_rob_B.view();
    show(obj.filt(f_rob_A, f_rob_B));
    show(obj.filt_sharpen(f_rob_A, f_rob_B));

    filter f_sobel_A = filter_genarate(SOBEL_A, 3);
    f_sobel_A.view();
    filter f_sobel_B = filter_genarate(SOBEL_B, 3);
    f_sobel_B.view();
    show(obj.filt(f_sobel_A, f_sobel_B));
    show(obj.filt_sharpen(f_sobel_A, f_sobel_B));

    if (sizeof(T) == sizeof(gray))
    {
        Mat a = obj.filt(f_guass_9);
        Mat result = ipt.clone();
        int temp = 0;
        for (int i = 0; i < a.size().height; i++)
        {
            for (int j = 0; j < a.size().width; j++)
            {
                temp = ipt.at<uchar>(i, j) - a.at<uchar>(i, j);
                if (temp > 255)
                {
                    result.at<uchar>(i, j) = 255;
                }
                else if (temp < 0)
                {
                    result.at<uchar>(i, j) = 0;
                }
                else
                {
                    result.at<uchar>(i, j) = temp;
                }
            }
        }
        cout << "m=f-s\n";
        show(result);

        for (int i = 0; i < a.size().height; i++)
        {
            for (int j = 0; j < a.size().width; j++)
            {
                temp = result.at<uchar>(i, j) * 2 + ipt.at<uchar>(i, j);
                if (temp > 255)
                {
                    result.at<uchar>(i, j) = 255;
                }
                else if (temp < 0)
                {
                    result.at<uchar>(i, j) = 0;
                }
                else
                {
                    result.at<uchar>(i, j) = temp;
                }
            }
        }
        cout << "g=f+m*2\n";
        show(result);
    }
}

/*
int main(int argc, void* argv[])
{
    Mat a = imread((char*)argv[1]);
    demo<color>(a);

    cvtColor(a, a, COLOR_BGR2GRAY);
    demo<gray>(a);

    return 0;
}
*/

//*/
