#ifndef CITY_H
#define CITY_H

#include "entity.h"
#include <QList>
#include "edge.h"

class City : public Entity
{
public:
    City(QPoint location, int myIndex, int cityCount);

    QSP<Edge> edgeForNeighbour(int num);
    double distance(QSP<City> other);

    void addCity();
    void removeCity(int num);
    void addPheromone(int city, double amount);
    void addEdge(QSP<Edge> edge, int neighbour);

    void update();
    void reset();

    virtual QGraphicsItem *getGraphicsItem();

private:
    QGraphicsEllipseItem *ellipse;
    QList<QSP<Edge> > edges;
    int index;
};

#endif // CITY_H
