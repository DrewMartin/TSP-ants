#ifndef ANT_H
#define ANT_H

#include "entity.h"
#include <QSet>
#include <QPair>
#include "city.h"

#define MOVE_SPEED 5

class Ant : public Entity
{
public:
    Ant(QSharedPointer<City> &startingCity, int index, int numCities);
    virtual ~Ant();

    virtual QGraphicsItem *getGraphicsItem();
    bool update(QList<QSP<City> > &cities);

    bool removeCity(int city);
    void addCity();
    BestDistancePheromone addPheromoneToTour(QList<QSP<City> > &cities);
    void resetTour();
    QList<int> getTour();

    static void setSpeed(int speed);
    static void setPheromoneImportance(int val);
    static void setDistanceImportance(int val);
    static void setMutation(int val);

protected:
    void updateEllipse();
    void chooseNextCity(QList<QSP<City> > &cities);
    void chooseRandomCity();

    QGraphicsEllipseItem *ellipse;
    QList<bool> tabu;
    QList<int> tour;

    int startCity;
    int fromCity;
    int toCity;
    bool doneTour;

    static int speed;
    static double pheromoneExponent;
    static double distanceExponent;
    static int mutation;
};

#endif // ANT_H
