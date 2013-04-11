#ifndef ENTITY_H
#define ENTITY_H

#include <QPoint>
#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include "common.h"

class Entity
{
public:
    Entity(QPoint location);
    virtual ~Entity();

    QPoint &getLocation();
    virtual QGraphicsItem *getGraphicsItem() = 0;

protected:
    QPoint location;
};

#endif // ENTITY_H
