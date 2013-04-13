#include "entity.h"

double Entity::scaleX = 1.0;
double Entity::scaleY = 1.0;

Entity::Entity(QPointF location) :
    location(location)
{
}

Entity::~Entity()
{
}

QPointF &Entity::getLocation()
{
    return location;
}

void Entity::setScale(double x, double y)
{
    scaleX = x;
    scaleY = y;
}
