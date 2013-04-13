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
    void updateLine(double onBestTour);

    static void setDecayRate(int rate);
    static void setDefaultPheromone(double amount);
    static void setShowPheromone(bool show);

private:
    QGraphicsLineItem *line;
    double pheromone;
    double length;
    bool onBestTour;

    static double decayRate;
    static double defaultPheromone;
    static bool showPheromone;

};

#endif // EDGE_H
