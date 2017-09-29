#include "Histogram.h"
#include "ui_Histogram.h"

#include <QPainter>

#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>

#include <QtDebug>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

QT_CHARTS_USE_NAMESPACE

Histogram::Histogram(QWidget *parent) :
    QDockWidget(parent),

    ui(new Ui::Histogram)
{
    ui->setupUi(this);

    _chartView = NULL;
}

Histogram::~Histogram()
{
    delete ui;
}

void Histogram::create(std::shared_ptr<cv::Mat> mat)
{
    _currentMat = mat;

    if( !_currentMat->data )
        return ;


    if(_currentMat->type()==CV_8UC1)
    {
        histogramCV_8UC1();
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    else if(_currentMat->type()==CV_8UC3)
    {
       histogramCV_8UC3();
    }
    else if(_currentMat->type() == CV_16U)
    {
       histogramCV_16U();
    }
    else if(_currentMat->type() == CV_32F)
    {
        histogramCV_32F();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return ;
    }

}


void Histogram::histogramCV_8UC1()
{



}

void Histogram::histogramCV_16U()
{




}

void Histogram::histogramCV_32F()
{




}

void Histogram::histogramCV_8UC3()
{
    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( *_currentMat, bgr_planes );

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    double minB, maxB, minR, maxR, minG, maxG;
    cv::minMaxLoc(b_hist, &minB, &maxB);
    cv::minMaxLoc(g_hist, &minG, &maxG);
    cv::minMaxLoc(r_hist, &minR, &maxR);

    double max = maximum<double>(maxB,maxG,maxR);

    QSplineSeries *seriesR = new QSplineSeries();
    seriesR->setName("R");

    QSplineSeries *seriesG = new QSplineSeries();
    seriesG->setName("G");

    QSplineSeries *seriesB = new QSplineSeries();
    seriesB->setName("B");

    /// Draw for each channel
    for( int i = 0; i < histSize; i = i+5 )
    {
        seriesR->append( i,cvRound(r_hist.at<float>(i)));
        seriesG->append( i,cvRound(g_hist.at<float>(i)));
        seriesB->append( i,cvRound(b_hist.at<float>(i)));
    }

    QChart *chart = new QChart();
    chart->addSeries(seriesR);
    chart->addSeries(seriesG);
    chart->addSeries(seriesB);
    chart->createDefaultAxes();
    chart->axisY()->setRange(0, max);
    chart->axisX()->setRange(0, histSize-1);
    chart->axisY()->setGridLineVisible(false);
    chart->axisX()->setGridLineVisible(false);

    _chartView = new QChartView(chart);
    QChart::ChartTheme theme = QChart::ChartThemeDark;
    _chartView->chart()->setTheme(theme);
    _chartView->setRenderHint(QPainter::Antialiasing);
    _chartView->setRubberBand( QChartView::HorizontalRubberBand );
    _chartView->setInteractive(true);

    //after set theme otherwise the color not take into consideration
    seriesR->setColor(QColor(255,0,0,255));
    seriesG->setColor(QColor(0,153,0,255));
    seriesB->setColor(QColor(0,128,255,255));

    this->setWidget(_chartView);
    resize(400,400);
    show();

}

void Histogram::updateUI()
{

}

