#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsLineItem>
#include "common.h"

class Edge
{
public:
    Edge(QPointF p1, QPointF p2);
    ~Edge();

    double addPheromone(double amount);
    void doDecay();
    void reset();
    void setBest();
    double getLength();
    QGraphicsItem* getGraphicsItem();

    double getPheromone();

    static void setDecayRate(double rate);

    void updateLine(double onBestTour);

private:
    QGraphicsLineItem *line;
    double pheromone;
    double length;
    bool onBestTour;

    static double decayRate;

};

#endif // EDGE_H
