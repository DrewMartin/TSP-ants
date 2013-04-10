#ifndef ANT_H
#define ANT_H

#include "entity.h"
#include <QList>
#include "city.h"

#define MOVE_SPEED 5

class Ant : public Entity
{
public:
    Ant(QPoint &location);
    virtual ~Ant();

    static void setInstantMove(bool instant);
    virtual QGraphicsItem *getGraphicsItem();
    void update(QList<QSP<City> > &cities);

protected:

    QGraphicsEllipseItem *ellipse;

    static bool instantMove;
};

#endif // ANT_H
