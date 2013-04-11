#include "city.h"
#include <qmath.h>

#define CHECK_CITY(num) if (num < 0 || num >= edges.length()) throw "Bad city"

City::City(QPoint location, int myIndex, int cityCount) :
    Entity(location),
    ellipse(NULL),
    index(myIndex)
{
    for (int i = 0; i < cityCount; i++) {
        edges.append(QSP<Edge>());
    }
}

void City::addCity()
{
    edges.append(QSP<Edge>());
}

void City::removeCity(int num)
{
    CHECK_CITY(num);

    if (num < index)
        index--;

    edges.removeAt(num);
}


QSP<Edge> City::edgeForNeighbour(int num)
{
    CHECK_CITY(num);

    return edges.at(num);
}

double City::distance(QSP<City> other)
{
    return dist(location, other->getLocation());
}

void City::addPheromone(int city, double amount)
{
    CHECK_CITY(city);
    edges.at(city)->addPheromone(amount);
}

void City::addEdge(QSP<Edge> edge, int neighbour)
{
    CHECK_CITY(neighbour);

    edges[neighbour] = edge;
}

void City::update()
{
    for (int i = 0; i < index; i++)
        edges.at(i)->update();

}

void City::reset()
{
    for (int i = 0; i < index; i++)
        edges.at(i)->reset();
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
