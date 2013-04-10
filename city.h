#ifndef CITY_H
#define CITY_H

#include "entity.h"
#include <QList>

#define DECAY_RATE 0.95

class City : public Entity
{
public:
    City(QPoint &location, int cityCount);

    int pheromoneForNeighbour(int num);
    double distance(QSP<City> other);

    void addCity();
    void removeCity(int num);
    void addPheromone(int city, int amount);

    void update();

    virtual QGraphicsItem *getGraphicsItem();

private:
    QGraphicsEllipseItem *ellipse;
    QList<int> edgePheromones;
};

#endif // CITY_H
