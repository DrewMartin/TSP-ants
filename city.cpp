#include "city.h"
#include <qmath.h>

#define CHECK_CITY(num) if (num < 0 || num >= edgePheromones.length()) throw "Bad city"

City::City(QPoint &location, int cityCount) :
    Entity(location),
    ellipse(NULL)
{
    for (int i = 0; i < cityCount; i++)
        edgePheromones.append(0);
}

void City::addCity()
{
    edgePheromones.append(0);
}

void City::removeCity(int num)
{
    CHECK_CITY(num);

    edgePheromones.removeAt(num);
}


int City::pheromoneForNeighbour(int num)
{
    CHECK_CITY(num);

    return edgePheromones.at(num);
}

double City::distance(QSharedPointer<City> other)
{
    return dist(location, other->getLocation());
}

void City::addPheromone(int city, int amount)
{
    CHECK_CITY(city);
    edgePheromones[city] = edgePheromones[city] + amount;
}

void City::update()
{
    for (int i = 0; i < edgePheromones.length(); i++)
        edgePheromones[i] = edgePheromones[i] * DECAY_RATE;

}

QGraphicsItem *City::getGraphicsItem()
{
    if (!ellipse) {
        ellipse = new QGraphicsEllipseItem(LOC_HELPER(location,CITY_RADIUS));
        ellipse->setPen(QPen(CITY_COLOR));
        ellipse->setBrush(QBrush(CITY_COLOR));
        ellipse->setZValue(CITY_Z);
    }
    return ellipse;
}
