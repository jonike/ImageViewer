#ifndef GRAPHICSIMAGEVIEW_H
#define GRAPHICSIMAGEVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QImage>

#include <memory>

namespace Ui {
class GraphicsImageView;
}

class GraphicsImageView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphicsImageView(QWidget *parent = 0);
    ~GraphicsImageView();

    void wheelEvent(QWheelEvent* e);
    void addImage(std::shared_ptr<QImage> img);

public slots:
    void updateUI();

private:
    Ui::GraphicsImageView *ui;
    QGraphicsScene * _scene;
    QGraphicsPixmapItem * _item;
    std::shared_ptr<QImage> _currentImage; //same as Viewer

};

#endif // GRAPHICSIMAGEVIEW_H
