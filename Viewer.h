#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QString>
#include <memory>

#include "Histogram.h"

namespace cv{
class Mat;

namespace cuda {
class GpuMat;
}
}


namespace Ui {
class Viewer;
}

class Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();

    void openImage( );

signals:
    void imageUpdate(); //when there is a change with the current image (update Histogram, Viewer)
    void createHistogram(QDockWidget * dock);

private slots:
    void on_pushButton_clicked();

    void on__pushButonFilter_clicked();

private:
    Ui::Viewer *ui;
    std::shared_ptr<QImage> _currentImage;
    std::shared_ptr<cv::Mat> _currentMat;

    Histogram * histoDockWidget;



};

#endif // VIEWER_H
