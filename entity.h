#ifndef ENTITY_H
#define ENTITY_H

#include <QPointF>
#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include "common.h"

class Entity
{
public:
    Entity(QPointF location);
    virtual ~Entity();

    QPointF &getLocation();
    virtual QGraphicsItem *getGraphicsItem() = 0;

protected:
    QPointF location;
};

#endif // ENTITY_H
