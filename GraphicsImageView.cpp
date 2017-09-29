#include "GraphicsImageView.h"
#include "ui_GraphicsImageView.h"

#include <QWheelEvent>
#include <QtMath>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QImage>
#include <QPixmap>

GraphicsImageView::GraphicsImageView(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::GraphicsImageView)
{
    ui->setupUi(this);

    setRenderHint(QPainter::HighQualityAntialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
}

GraphicsImageView::~GraphicsImageView()
{
    delete ui;
}

void GraphicsImageView::wheelEvent(QWheelEvent* e)
{
    if (e->modifiers() & Qt::ControlModifier)
    {
        // Do a wheel-based zoom about the cursor position
        double angle = e->angleDelta().y();
        double factor = qPow(1.0015, angle);

        auto targetViewportPos = e->pos();
        auto targetScenePos = mapToScene(e->pos());

        scale(factor, factor);
        centerOn(targetScenePos);
        QPointF deltaViewportPos = targetViewportPos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
        QPointF viewportCenter = mapFromScene(targetScenePos) - deltaViewportPos;
        centerOn(mapToScene(viewportCenter.toPoint()));

        return;
    }

    if ((e->modifiers()&Qt::ControlModifier) != Qt::ControlModifier) {
            /* no scrolling while control is held */
            QGraphicsView::wheelEvent(e);
    }

}

void GraphicsImageView::addImage(std::shared_ptr<QImage> img)
{
    _currentImage = img;
    _scene = new QGraphicsScene();
    setScene(_scene);
    _item = new QGraphicsPixmapItem(QPixmap::fromImage(*_currentImage));
    _item->setTransformationMode(Qt::SmoothTransformation);
    _scene->addItem(_item);
    show();
}

void GraphicsImageView::updateUI()
{
    if(_currentImage)
    {
        //_item->setPixmap(QPixmap::fromImage(*_currentImage));
        _scene = new QGraphicsScene();
        setScene(_scene);
        _item = new QGraphicsPixmapItem(QPixmap::fromImage(*_currentImage));
        _item->setTransformationMode(Qt::SmoothTransformation);
        _scene->addItem(_item);
        show();


        QGraphicsView::update();
        show();
    }

}




//if ((e->modifiers()&Qt::ControlModifier) == Qt::ControlModifier
//                && e->angleDelta().x() == 0) {

//        QPoint  pos  = e->pos();
//        QPointF posf = this->mapToScene(pos);

//        double by;
//        double angle = e->angleDelta().y();

//        if      (angle > 0) { by = 1 + ( angle / 360 * 0.1); }
//        else if (angle < 0) { by = 1 - (-angle / 360 * 0.1); }
//        else                { by = 1; }

//        this->scale(by, by);

//        double w = this->viewport()->width();
//        double h = this->viewport()->height();

//        double wf = this->mapToScene(QPoint(w-1, 0)).x()
//                        - this->mapToScene(QPoint(0,0)).x();
//        double hf = this->mapToScene(QPoint(0, h-1)).y()
//                        - this->mapToScene(QPoint(0,0)).y();

//        double lf = posf.x() - pos.x() * wf / w;
//        double tf = posf.y() - pos.y() * hf / h;

//        /* try to set viewport properly */
//        this->ensureVisible(lf, tf, wf, hf, 0, 0);

//        QPointF newPos = this->mapToScene(pos);

//        /* readjust according to the still remaining offset/drift
//         * I don't know how to do this any other way */
//        this->ensureVisible(QRectF(QPointF(lf, tf) - newPos + posf,
//                        QSizeF(wf, hf)), 0, 0);

//        e->accept();
//}

