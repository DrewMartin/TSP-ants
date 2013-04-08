#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit clicked(event->pos());
}
