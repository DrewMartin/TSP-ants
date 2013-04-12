#include "edge.h"
#include <QPen>
#include <QBrush>
#include <QDebug>

double Edge::decayRate = PHEROMONE_DEFAULT_DECAY_RATE;

Edge::Edge(QPointF p1, QPointF p2) :
    pheromone(0.0),
    onBestTour(false)
{
    length = dist(p1, p2);
    line = new QGraphicsLineItem(p1.x(), p1.y(), p2.x(), p2.y());
    line->setZValue(PHEROMONE_Z);
    updateLine(0.0);
}

Edge::~Edge()
{
    delete line;
}

double Edge::addPheromone(double amount)
{
    pheromone = pheromone + amount;
    return pheromone;
}

void Edge::doDecay()
{
    pheromone *= decayRate;
    if (pheromone < 0.0000001)
        pheromone = 0.0;
    onBestTour = false;
}

void Edge::reset()
{
    pheromone = 0.0;
    updateLine(0.0);
}

void Edge::setBest()
{
    onBestTour = true;
}

double Edge::getLength()
{
    return length;
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
    decayRate = 1.0 - rate;
}

void Edge::updateLine(double best)
{
    if (best <= 0.0 || best / 10.0 > pheromone) {
        line->setVisible(false);
        return;
    }
    line->setVisible(true);
    QPen pen;
    if (onBestTour) {
        pen.setColor(BEST_TOUR_COLOR);
        pen.setWidthF(5.0);
    } else {
        pen.setColor(EDGE_COLOR);
        pen.setWidthF(5.0*pheromone/best);
    }

    line->setPen(pen);
}
