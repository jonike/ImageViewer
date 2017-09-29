#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDockWidget>
#include <memory>

#include <QtCharts/QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

namespace cv{
class Mat;
}


namespace Ui {
class Histogram;
}

class Histogram : public QDockWidget
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = 0);
    ~Histogram();

    void create(std::shared_ptr<cv::Mat> mat);


public slots:
    void updateUI();


private:
    void histogramCV_8UC1();
    void histogramCV_8UC3();
    void histogramCV_16U();
    void histogramCV_32F();

    Ui::Histogram *ui;
    std::shared_ptr<cv::Mat> _currentMat;
    QChartView * _chartView;
};

template<typename T>
inline T maximum( T a, T b ) { return a>b ? a : b ; }

template<typename T>
inline T maximum( T a, T b, T c ) { return maximum( maximum(a,b), c ) ; }

#endif // HISTOGRAM_H
