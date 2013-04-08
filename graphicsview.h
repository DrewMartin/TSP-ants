#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    
signals:
    void clicked(QPoint position);


public slots:

};

#endif // GRAPHICSVIEW_H
