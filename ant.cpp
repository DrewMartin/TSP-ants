#include "ant.h"
#include <QDebug>
#include <QVector2D>

int Ant::speed = ANT_SPEED;
double Ant::pheromoneExponent = 0.50;
double Ant::distanceExponent = 0.50;

Ant::Ant(QSP<City> &startingCity, int index, int numCities) :
    Entity(QPointF(startingCity->getLocation())),
    startCity(index),
    fromCity(index),
    toCity(-1),
    doneTour(false)
{
    ellipse = new QGraphicsEllipseItem(LOC_HELPER(location,ANT_RADIUS));
    ellipse->setPen(QPen(ANT_COLOR));
    ellipse->setBrush(QBrush(ANT_COLOR));
    ellipse->setZValue(ANT_Z);

    for (int i = 0; i < numCities; i++)
        tabu.append(i == index);
}

Ant::~Ant()
{
    delete ellipse;
}

QGraphicsItem *Ant::getGraphicsItem()
{
    return ellipse;
}

bool Ant::update(QList<QSP<City> > &cities)
{
    if (toCity == -1)
        chooseNextCity(cities);

    double speedLeft = speed;
    if (speedLeft == 99.0)
        speedLeft = INT_MAX;
    double distance;

    if (doneTour)
        return doneTour;

    while(!doneTour) {
        distance = dist(location, cities.at(toCity)->getLocation());

        // you can make it to the destination this turn. Move there, pick next city
        if (distance - speedLeft < 0.0001) {
            speedLeft -= distance;
            location = cities.at(toCity)->getLocation();
            chooseNextCity(cities);
        } else {
            break;
        }
    }

    //move speedLeft amount towards toCity
    if (!doneTour && speedLeft > 0) {
        QPointF dest = cities.at(toCity)->getLocation();
        QVector2D move(dest - location);
        move.normalize();
        move *= speedLeft;
        location.setX(location.x() + move.x());
        location.setY(location.y() + move.y());
    }

    updateEllipse();
    return doneTour;
}

bool Ant::removeCity(int cityIndex)
{
    tabu.removeAt(cityIndex);
    if (cityIndex == fromCity || cityIndex == toCity || startCity == cityIndex)
        return true;
    if (cityIndex < fromCity)
        fromCity--;
    if (cityIndex < toCity)
        toCity--;
    if (cityIndex < startCity)
        startCity--;

    for (int i = 0; i < tour.length(); i++) {
        if (tour.at(i) == cityIndex) {
            tour.removeAt(i);
            i--;
        } else if (tour.at(i) > cityIndex) {
            tour[i] = tour[i] - 1;
        }
    }

    return false;
}

void Ant::addCity()
{
    tabu.append(false);
}

void Ant::updateEllipse()
{
    ellipse->setRect(LOC_HELPER(location,ANT_RADIUS));
}

void Ant::chooseNextCity(QList<QSP<City> > &cities)
{
    if (cities.length() <= 1 || doneTour)
        return;

    if (toCity >= 0) {
        tour.append(fromCity);
        tabu[toCity] = true;
        fromCity = toCity;
        //ant has completed its tour
        if (fromCity == startCity) {
            doneTour = true;
            toCity = -1;
            return;
        }

    }

    double cityProb[cities.length()];
    double total = 0;
    QSP<Edge> edge;

    //if every city has been visited, return to the starting city
    if (!tabu.contains(false)) {
        toCity = startCity;
        return;
    }

    //set chance to visit any particular neighbour that hasn't been visited this cycle
    for (int i = 0; i < cities.length(); i++) {
        cityProb[i] = 0;
        if (!tabu.at(i)) {
            edge = cities.at(fromCity)->edgeForNeighbour(i);
            cityProb[i] = qPow(qMax(0.000001, edge->getPheromone()), pheromoneExponent) * qPow(1.0/cities.at(fromCity)->distance(i), distanceExponent);
            total += cityProb[i];
        }
    }

    for (int i = 0; i < cities.length(); i++) {
        cityProb[i] /= total;
    }

    do {
        //pick a neighbour proportional to its inverse distance and pheromone
        double random = ((double)qrand() / RAND_MAX);
        for (int i = 0; i < cities.length(); i++) {
            if (random - cityProb[i] < 0.0000001 && !tabu.at(i)) {
                toCity = i;
                break;
            }
            random -= cityProb[i];
        }
        if (toCity == fromCity)
            qDebug() << "Equal cities. Bad";
    } while (toCity == fromCity || toCity < 0);

}


BestDistancePheromone Ant::addPheromoneToTour(QList<QSharedPointer<City> > &cities)
{
    double tourLength = cities.at(tour.at(0))->edgeForNeighbour(tour.last())->getLength();
    for (int i = 1; i < tour.length(); i++)
        tourLength += cities.at(tour.at(i))->edgeForNeighbour(tour.at(i-1))->getLength();

    double pheromoneAdd = 1.0/tourLength;
    double bestPheromone = 0.0, curr;
    for (int i = 1; i < tour.length(); i++) {
        curr = cities.at(tour.at(i))->addPheromone(tour.at(i-1), pheromoneAdd);
        if (curr > bestPheromone)
            bestPheromone = curr;
    }
    cities.at(tour.at(0))->addPheromone(tour.last(), pheromoneAdd);

    return BestDistancePheromone(tourLength, bestPheromone);
}

void Ant::resetTour()
{
    fromCity = startCity;
    toCity = -1;
    for (int i = 0; i < tabu.length(); i++) {
        tabu[i] = (fromCity == i);
    }
    tour.clear();
    doneTour = false;
}

QList<int> Ant::getTour()
{
    return tour;
}

void Ant::setSpeed(int speed)
{
    Ant::speed = speed;
}

void Ant::setPheromoneImportance(int val)
{
    pheromoneExponent = 1.0 - val / 100.0;
}

void Ant::setDistanceImportance(int val)
{
    distanceExponent = 1.0 - val / 100.0;
}
