#include "ant.h"
#include <QDebug>
bool Ant::instantMove = false;

Ant::Ant(QPoint &location) :
    Entity(location),
    ellipse(NULL)
{
}

Ant::~Ant()
{
    if (ellipse)
        delete ellipse;
}

void Ant::setInstantMove(bool instant)
{
    instantMove = instant;
}

QGraphicsItem *Ant::getGraphicsItem()
{
    if (!ellipse) {
        ellipse = new QGraphicsEllipseItem(LOC_HELPER(location,ANT_RADIUS));
        ellipse->setPen(QPen(ANT_COLOR));
        ellipse->setBrush(QBrush(ANT_COLOR));
        ellipse->setZValue(ANT_Z);
    }
    return ellipse;
}

void Ant::update(QList<QSharedPointer<City> > &cities)
{
}
