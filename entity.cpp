#include "entity.h"

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
