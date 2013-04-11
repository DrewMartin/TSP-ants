#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsLineItem>
#include "common.h"

class Edge
{
public:
    Edge(QPointF p1, QPointF p2);
    ~Edge();

    void addPheromone(double amount);
    void update();
    void reset();
    QGraphicsItem* getGraphicsItem();

    double getPheromone();

    static void setDecayRate(double rate);

private:
    QGraphicsLineItem *line;
    double pheromone;
    QColor color;

    void updateLine();

    static double decayRate;

};

#endif // EDGE_H
