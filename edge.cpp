#include "edge.h"
#include <QPen>
#include <QBrush>

double Edge::decayRate = PHEROMONE_DEFAULT_DECAY_RATE;

Edge::Edge(QPoint p1, QPoint p2) :
    pheromone(0.0),
    color(EDGE_COLOR)
{
    line = new QGraphicsLineItem(p1.x(), p1.y(), p2.x(), p2.y());
    line->setZValue(PHEROMONE_Z);
    updateLine();
}

Edge::~Edge()
{
    delete line;
}

void Edge::addPheromone(double amount)
{
    pheromone = qMin(pheromone + amount, MAX_PHEROMONE);
    updateLine();
}

void Edge::update()
{
    double old = pheromone;
    pheromone *= decayRate;
    if (pheromone < 0.5)
        pheromone = 0.0;

    if (old != pheromone)
        updateLine();
    else
        old = 1234.0;
}

void Edge::reset()
{
    pheromone = 0.0;
    updateLine();
}

QGraphicsItem *Edge::getGraphicsItem()
{
    return line;
}

double Edge::getPheromone()
{
    return pheromone;
}

void Edge::setDecayRate(double rate)
{
    decayRate = rate;
}

void Edge::updateLine()
{
    color.setAlphaF(pheromone/(2.0*MAX_PHEROMONE));
    QBrush brush(color);
    QPen pen(brush, 5.0*pheromone/MAX_PHEROMONE);
    line->setPen(pen);
}
