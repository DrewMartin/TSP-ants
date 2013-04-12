#ifndef CITY_H
#define CITY_H

#include "entity.h"
#include <QList>
#include "edge.h"

class City : public Entity
{
public:
    City(QPointF location, int myIndex, int cityCount);
    virtual ~City();

    QSP<Edge> edgeForNeighbour(int num);
    double distance(int other);

    void addCity();
    void removeCity(int num);
    double addPheromone(int city, double amount);
    void addEdge(QSP<Edge> edge, int neighbour);
    void updateLines(double best);

    void doDecay();
    void reset();

    virtual QGraphicsItem *getGraphicsItem();

private:
    QGraphicsEllipseItem *ellipse;
    QList<QSP<Edge> > edges;
    int index;
};

#endif // CITY_H
