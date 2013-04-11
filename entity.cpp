#include "entity.h"

Entity::Entity(QPoint location) :
    location(location)
{
}

Entity::~Entity()
{
}

QPoint &Entity::getLocation()
{
    return location;
}
