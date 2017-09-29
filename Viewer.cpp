#include "Viewer.h"
#include "ui_Viewer.h"

#include "GraphicsImageView.h"
#include "MatToQImage.h"
#include "Histogram.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QImage>
#include <QFileDialog>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/cudafilters.hpp"
#include "opencv2/cudaimgproc.hpp"

#include "mainwindow.h"

Viewer::Viewer(QWidget *parent) :
    QWidget(parent),
    histoDockWidget(NULL),
    ui(new Ui::Viewer)
{
    ui->setupUi(this);
    connect(this, SIGNAL(imageUpdate()), ui->_graphicsViewImg, SLOT(updateUI()));

}

Viewer::~Viewer()
{
    delete ui;
}

void Viewer::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/martin/Documents/Python/Kaggle/InvasiveSpeciesMonitoring/input", tr("Image Files (*.png *.jpg *.bmp *.tif)"));

    _currentMat = std::make_shared<cv::Mat>(cv::imread(fileName.toStdString()));
    _currentImage = std::make_shared<QImage>(MatToQImage(*_currentMat));
    ui->_graphicsViewImg->addImage(_currentImage);

    //QString formatimg = _currentImage->format() == QImage::Format_Grayscale8 ? "Format_Grayscale8" : "ARFFF";
    //ui->_labelImageInfo->setText(QString("Image Infos : %1, %2").arg(formatimg).arg("test"));

}

void Viewer::on_pushButton_clicked()
{
    if(histoDockWidget == NULL)
    {
        MainWindow *w = (MainWindow *)this->parent();
        histoDockWidget = new Histogram(w);
        connect(this, SIGNAL(imageUpdate()), histoDockWidget, SLOT(updateUI()));
    }

    if(_currentMat)
        histoDockWidget->create(_currentMat);

    histoDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    histoDockWidget->setFeatures(histoDockWidget->features() | QDockWidget::DockWidgetMovable);
    histoDockWidget->setWindowTitle( "Histogram" );
    histoDockWidget->show();
}

void Viewer::on__pushButonFilter_clicked()
{
    //Modify _currentMat
    cv::cuda::GpuMat src, dst;
    src.upload(*_currentMat);
    if (src.channels() == 3)
    {
        // gpu support only 4th channel images
        cuda::GpuMat src4ch;
        cuda::cvtColor(src, src4ch, COLOR_BGR2BGRA);
        src = src4ch;
    }

    Mat element = getStructuringElement(MORPH_RECT, Size(22*2+1, 22*2+1), Point(2, 2));
    Ptr<cuda::Filter> openFilter = cuda::createMorphologyFilter(MORPH_OPEN, src.type(), element);
    openFilter->apply(src, dst);
    //dst.download(*_currentMat);

    Mat h_dst(dst);
    cv::cvtColor(h_dst, *_currentMat, COLOR_BGRA2BGR);

    emit imageUpdate();
}
