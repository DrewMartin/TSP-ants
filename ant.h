#ifndef ANT_H
#define ANT_H

#include "entity.h"
#include <QSet>
#include "city.h"

#define MOVE_SPEED 5

class Ant : public Entity
{
public:
    Ant(QSharedPointer<City> &startingCity, int index, int numCities);
    virtual ~Ant();

    static void setInstantMove(bool instant);
    virtual QGraphicsItem *getGraphicsItem();
    void update(QList<QSP<City> > &cities);

    bool removeCity(int city);
    void addCity();

protected:
    void updateEllipse(const QPoint &newPoint);

    QGraphicsEllipseItem *ellipse;
    QList<bool> history;
    int currentCity;

    static bool instantMove;
};

#endif // ANT_H
