#include "entity.h"

Entity::Entity(QPoint &location) :
    location(location)
{
}

Entity::~Entity()
{
}

const QPoint &Entity::getLocation()
{
    return location;
}
