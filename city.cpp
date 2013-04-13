#include "city.h"
#include <qmath.h>
#include <QDebug>

#define CHECK_CITY(num) if (num < 0 || num >= edges.length()) throw "Bad city"

City::City(QPointF location, int myIndex, int cityCount) :
    Entity(location),
    index(myIndex)
{
    ellipse = new QGraphicsEllipseItem(LOC_HELPER(location,CITY_RADIUS));
    ellipse->setPen(QPen(CITY_COLOR));
    ellipse->setBrush(QBrush(CITY_COLOR));
    ellipse->setZValue(CITY_Z);

    for (int i = 0; i < cityCount; i++) {
        edges.append(QSP<Edge>());
    }
}

City::~City()
{
    delete ellipse;
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

double City::distance(int other)
{
    if (other == index)
        return 0;
    return edges.at(other)->getLength();
}

double City::addPheromone(int city, double amount)
{
    CHECK_CITY(city);

    return edges.at(city)->addPheromone(amount);
}

void City::addEdge(QSP<Edge> edge, int neighbour)
{
    CHECK_CITY(neighbour);

    edges[neighbour] = edge;
}

void City::updateLines(double best)
{
    for (int i = 0; i < index; i++)
        edges.at(i)->updateLine(best);
}

void City::doDecay()
{
    for (int i = 0; i < index; i++)
        edges.at(i)->doDecay();

}

void City::reset()
{
    for (int i = 0; i < index; i++)
        edges.at(i)->reset();
}

QGraphicsItem *City::getGraphicsItem()
{
    return ellipse;
}
