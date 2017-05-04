#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "exp1.h"
#include "exp2.h"
#include "exp3.h"
#include "exp4.h"
#include "exp5.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DisplayMat(cv::Mat image)
{
    cv::Mat rgb;
    QImage img;
    if(image.channels() == 3)
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_Indexed8);
    }
    ui->labelimage->setPixmap(QPixmap::fromImage(img).scaled(ui->labelimage->size()));//setPixelmap(QPixmap::fromImage(img));
    ui->labelimage->resize(ui->labelimage->pixmap()->size());//resize(ui->label->pixmap()->size());
}

void MainWindow::DisplayMat_2(cv::Mat image)
{
    cv::Mat rgb;
    QImage img;
    if(image.channels() == 3)
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_Indexed8);
    }
    ui->labelimage_2->setPixmap(QPixmap::fromImage(img).scaled(ui->labelimage_2->size()));//setPixelmap(QPixmap::fromImage(img));
    ui->labelimage_2->resize(ui->labelimage_2->pixmap()->size());//resize(ui->label->pixmap()->size());
}

void MainWindow::DisplayMat_hist(cv::Mat image)
{
    cv::Mat rgb;
    QImage img;
    if(image.channels() == 3)
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_Indexed8);
    }
    ui->labelhist->setPixmap(QPixmap::fromImage(img).scaled(ui->labelhist->size()));//setPixelmap(QPixmap::fromImage(img));
    ui->labelhist->resize(ui->labelhist->pixmap()->size());//resize(ui->label->pixmap()->size());
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Open Image"),".",tr("Image File (*.jpg *.png *.bmp)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()
    image = cv::imread(name);
    gray = exp_1::convert(image, exp_1::gray_convert).clone();
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image Data Is Null"));
        msgBox.exec();
    }
    else {
       DisplayMat(image);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(image.empty()) return;
    DisplayMat(gray);
}

void MainWindow::on_pushButton_3_clicked()
{
    if(image.empty()) return;
    DisplayMat(image);
}

void MainWindow::on_pushButton_4_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_bin->text();
    double val = str.toDouble();
    DisplayMat(exp_1::convert(gray, exp_1::bin_convert, val));
}

void MainWindow::on_pushButton_5_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_log->text();
    double val = str.toDouble();
    DisplayMat(exp_1::convert(gray, exp_1::log_convert, val));
}

void MainWindow::on_pushButton_6_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_power->text();
    double val = str.toDouble();
    DisplayMat(exp_1::convert(gray, exp_1::multi_convert, val));
}

void MainWindow::on_pushButton_7_clicked()
{
    if(image.empty()) return;
    DisplayMat(exp_1::convert(image, exp_1::opp_convert));
}

void MainWindow::on_pushButton_8_clicked()
{
    if(image.empty()) return;
    DisplayMat(exp_1::convert(image, exp_1::com_convert));
}

void MainWindow::on_pushButton_9_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Open Image"),".",tr("Image File (*.jpg *.png *.bmp)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()
    image = cv::imread(name);
    exp_2::multichannel pic(image);
    hist = pic.hist_graph();
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image Data Is Null"));
        msgBox.exec();
    }
    else {
       DisplayMat_2(image);
       DisplayMat_hist(hist);
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    if(image.empty()) return;
    exp_2::multichannel pic(image);
    DisplayMat_2(pic.hist_balance());

    exp_2::multichannel pic_balanced(pic.hist_balance());
    DisplayMat_hist(pic_balanced.hist_graph());
}

void MainWindow::on_pushButton_11_clicked()
{
    if(image.empty()) return;
    DisplayMat_2(image);
    DisplayMat_hist(hist);
}

void MainWindow::DisplayMat_3(cv::Mat image)
{
    cv::Mat rgb;
    QImage img;
    if(image.channels() == 3)
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_Indexed8);
    }
    ui->labelimage_3->setPixmap(QPixmap::fromImage(img).scaled(ui->labelimage_3->size()));//setPixelmap(QPixmap::fromImage(img));
    ui->labelimage_3->resize(ui->labelimage_3->pixmap()->size());//resize(ui->label->pixmap()->size());
}

void MainWindow::on_pushButton_12_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Open Image"),".",tr("Image File (*.jpg *.png *.bmp)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()
    image = cv::imread(name);
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image Data Is Null"));
        msgBox.exec();
    }
    else {
       DisplayMat_3(image);
    }
}

void MainWindow::on_pushButton_13_clicked()
{
    if(image.empty()) return;
    DisplayMat_3(image);
}

void MainWindow::on_pushButton_14_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_avl_3 = exp_3::filter_genarate(1, 3);
    DisplayMat_3(pic.filt(f_avl_3));
}

void MainWindow::on_pushButton_15_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_avl_5 = exp_3::filter_genarate(1, 5);
    DisplayMat_3(pic.filt(f_avl_5));
}

void MainWindow::on_pushButton_16_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_avl_9 = exp_3::filter_genarate(1, 9);
    DisplayMat_3(pic.filt(f_avl_9));
}

void MainWindow::on_pushButton_17_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_guass_3 = exp_3::filter_genarate(2, 3);
    DisplayMat_3(pic.filt(f_guass_3));
}

void MainWindow::on_pushButton_18_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_guass_5 = exp_3::filter_genarate(2, 5);
    DisplayMat_3(pic.filt(f_guass_5));
}

void MainWindow::on_pushButton_19_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_guass_9 = exp_3::filter_genarate(2, 9);
    DisplayMat_3(pic.filt(f_guass_9));
}

void MainWindow::on_pushButton_20_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_lap = exp_3::filter_genarate(3, 3);
    DisplayMat_3(pic.filt(f_lap));
}

void MainWindow::on_pushButton_21_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_lap_sharpen = exp_3::filter_genarate(4, 3);
    DisplayMat_3(pic.filt(f_lap_sharpen));
}

void MainWindow::on_pushButton_22_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_rob_A = exp_3::filter_genarate(5, 3);
    exp_3::filter f_rob_B = exp_3::filter_genarate(6, 3);
    DisplayMat_3(pic.filt(f_rob_A, f_rob_B));
}

void MainWindow::on_pushButton_23_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_rob_A = exp_3::filter_genarate(5, 3);
    exp_3::filter f_rob_B = exp_3::filter_genarate(6, 3);
    DisplayMat_3(pic.filt_sharpen(f_rob_A, f_rob_B));
}

void MainWindow::on_pushButton_24_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_sobel_A = exp_3::filter_genarate(7, 3);
    exp_3::filter f_sobel_B = exp_3::filter_genarate(8, 3);
    DisplayMat_3(pic.filt(f_sobel_A, f_sobel_B));
}

void MainWindow::on_pushButton_25_clicked()
{
    if(image.empty()) return;
    exp_3::color pic(image);
    exp_3::filter f_sobel_A = exp_3::filter_genarate(7, 3);
    exp_3::filter f_sobel_B = exp_3::filter_genarate(8, 3);
    DisplayMat_3(pic.filt_sharpen(f_sobel_A, f_sobel_B));
}

void MainWindow::DisplayMat_5(cv::Mat image)
{
    cv::Mat rgb;
    QImage img;
    if(image.channels() == 3)
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_Indexed8);
    }
    ui->labelimage_5->setPixmap(QPixmap::fromImage(img).scaled(ui->labelimage_5->size()));//setPixelmap(QPixmap::fromImage(img));
    ui->labelimage_5->resize(ui->labelimage_5->pixmap()->size());//resize(ui->label->pixmap()->size());
}

void MainWindow::on_pushButton_26_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Open Image"),".",tr("Image File (*.jpg *.png *.bmp)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()
    image = cv::imread(name);
    gray = exp_1::convert(image, exp_1::gray_convert).clone();
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image Data Is Null"));
        msgBox.exec();
    }
    else {
       DisplayMat_5(gray);
       exp_2::channel test(gray);
       DisplayMat_5_hist(test.hist_graph());
    }
}

void MainWindow::on_pushButton_27_clicked()
{
    if(image.empty()) return;
    DisplayMat_5(gray);
}

void MainWindow::on_pushButton_28_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_guass->text();
    double val = str.toDouble();
    QString str2 = ui->lineEdit_guass_2->text();
    double val2 = str2.toDouble();
    exp_5::noise guass(0, val, val2);
    DisplayMat_5(guass.polution(gray));
    polluted = guass.polution(gray).clone();

    int delta2 = val2*val2;
    QString str3 = QString::number(delta2, 10);
    ui->lineEdit_adavl->setText(str3);

    exp_2::channel test(polluted);
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_29_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_pepper->text();
    double val = str.toDouble();
    exp_5::noise pepper(2, val);
    DisplayMat_5(pepper.polution(gray));
    polluted = pepper.polution(gray).clone();

    exp_2::channel test(polluted);
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_30_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_salt->text();
    double val = str.toDouble();
    exp_5::noise salt(3, val);
    DisplayMat_5(salt.polution(gray));
    polluted = salt.polution(gray).clone();

    exp_2::channel test(polluted);
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_31_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_peppersalt->text();
    double val = str.toDouble();
    exp_5::noise pepper_salt(1, val, 1);
    DisplayMat_5(pepper_salt.polution(gray));
    polluted = pepper_salt.polution(gray).clone();

    exp_2::channel test(polluted);
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_32_clicked()
{
    if(image.empty()) return;
    if(polluted.empty()) return;
    DisplayMat_5(exp_5::traversal(polluted, exp_5::avl_filter));

    exp_2::channel test(exp_5::traversal(polluted, exp_5::avl_filter));
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_33_clicked()
{
    if(image.empty()) return;
    if(polluted.empty()) return;
    DisplayMat_5(exp_5::traversal(polluted, exp_5::mid_filter));

    exp_2::channel test(exp_5::traversal(polluted, exp_5::mid_filter));
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_34_clicked()
{
    if(image.empty()) return;
    if(polluted.empty()) return;
    DisplayMat_5(exp_5::traversal(polluted, exp_5::har_filter));

    exp_2::channel test(exp_5::traversal(polluted, exp_5::har_filter));
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_35_clicked()
{
    if(image.empty()) return;
    if(polluted.empty()) return;
    QString str = ui->lineEdit_inverhar->text();
    double val = str.toDouble();
    DisplayMat_5(exp_5::traversal(polluted, exp_5::inver_har_filter, 5, val));

    exp_2::channel test(exp_5::traversal(polluted, exp_5::inver_har_filter, 5, val));
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_36_clicked()
{
    if(image.empty()) return;
    if(polluted.empty()) return;
    QString str = ui->lineEdit_adavl->text();
    double val = str.toDouble();
    DisplayMat_5(exp_5::traversal(polluted, exp_5::adapt_avl_filter, 3, val));

    exp_2::channel test(exp_5::traversal(polluted, exp_5::adapt_avl_filter, 3, val));
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_37_clicked()
{
    if(image.empty()) return;
    if(polluted.empty()) return;
    DisplayMat_5(exp_5::traversal(polluted, exp_5::adapt_mid_filter, 3));

    exp_2::channel test(exp_5::traversal(polluted, exp_5::adapt_mid_filter, 3));
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::on_pushButton_38_clicked()
{
    if(image.empty()) return;
    if(polluted.empty()) return;
    DisplayMat_5(exp_5::traversal(polluted, exp_5::geo_filter));

    exp_2::channel test(exp_5::traversal(polluted, exp_5::geo_filter));
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::DisplayMat_4(cv::Mat image)
{
    cv::Mat rgb;
    QImage img;
    if(image.channels() == 3)
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_Indexed8);
    }
    ui->labelimage_4->setPixmap(QPixmap::fromImage(img).scaled(ui->labelimage_4->size()));//setPixelmap(QPixmap::fromImage(img));
    ui->labelimage_4->resize(ui->labelimage_4->pixmap()->size());//resize(ui->label->pixmap()->size());
}

void MainWindow::on_pushButton_39_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Open Image"),".",tr("Image File (*.jpg *.png *.bmp)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()
    image = cv::imread(name);
    gray = exp_1::convert(image, exp_1::gray_convert).clone();
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image Data Is Null"));
        msgBox.exec();
    }
    else {
       DisplayMat_4(gray);
    }
}

void MainWindow::DisplayMat_6(cv::Mat image)
{
    cv::Mat rgb;
    QImage img;
    if(image.channels() == 3)
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_Indexed8);
    }
    ui->labelimage_6->setPixmap(QPixmap::fromImage(img).scaled(ui->labelimage_6->size()));//setPixelmap(QPixmap::fromImage(img));
    ui->labelimage_6->resize(ui->labelimage_6->pixmap()->size());//resize(ui->label->pixmap()->size());
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    Mat about = imread("about.jpg");
    DisplayMat_6(about);
}

void MainWindow::on_pushButton_40_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_idealh->text();
    double val = str.toDouble();
    DisplayMat_4(exp_4::spectrum(gray, exp_4::ideal_highpass, val));
}

void MainWindow::on_pushButton_44_clicked()
{
    if(image.empty()) return;
    DisplayMat_4(gray);
}

void MainWindow::on_pushButton_41_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_ideall->text();
    double val = str.toDouble();
    DisplayMat_4(exp_4::spectrum(gray, exp_4::ideal_lowpass, val));
}

void MainWindow::on_pushButton_42_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_butterh->text();
    double val = str.toDouble();
    QString str2 = ui->lineEdit_butterho->text();
    double val2 = str2.toDouble();
    DisplayMat_4(exp_4::spectrum(gray, exp_4::butterworth_highpass, val, val2));
}

void MainWindow::on_pushButton_43_clicked()
{
    if(image.empty()) return;
    QString str = ui->lineEdit_butterl->text();
    double val = str.toDouble();
    QString str2 = ui->lineEdit_butterlo->text();
    double val2 = str2.toDouble();
    DisplayMat_4(exp_4::spectrum(gray, exp_4::butterworth_lowpass, val, val2));
}

void MainWindow::on_pushButton_45_clicked()
{
    image = Mat(200, 200, CV_8U);
    gray = image;
    exp_5::pic_init(gray);
    DisplayMat_5(gray);

    exp_2::channel test(gray);
    DisplayMat_5_hist(test.hist_graph());
}

void MainWindow::DisplayMat_5_hist(cv::Mat image)
{
    cv::Mat rgb;
    QImage img;
    if(image.channels() == 3)
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_Indexed8);
    }
    ui->labelimage_noisehist->setPixmap(QPixmap::fromImage(img).scaled(ui->labelimage_noisehist->size()));//setPixelmap(QPixmap::fromImage(img));
    ui->labelimage_noisehist->resize(ui->labelimage_noisehist->pixmap()->size());//resize(ui->label->pixmap()->size());
}
