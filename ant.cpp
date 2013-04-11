#include "ant.h"
#include <QDebug>
#include <QVector2D>
bool Ant::proportionalMove = false;

Ant::Ant(QSP<City> &startingCity, int index, int numCities) :
    Entity(QPointF(startingCity->getLocation())),
    fromCity(index),
    toCity(-1),
    speed(ANT_SPEED)
{
    ellipse = new QGraphicsEllipseItem(LOC_HELPER(location,ANT_RADIUS));
    ellipse->setPen(QPen(ANT_COLOR));
    ellipse->setBrush(QBrush(ANT_COLOR));
    ellipse->setZValue(ANT_Z);

    for (int i = 0; i < numCities; i++)
        history.append(i == index);
}

Ant::~Ant()
{
    delete ellipse;
}

void Ant::setProportionalMove(bool proportional)
{
    proportionalMove = proportional;
}

QGraphicsItem *Ant::getGraphicsItem()
{
    return ellipse;
}

void Ant::update(QList<QSP<City> > &cities)
{
    if (toCity == -1)
        chooseNextCity(cities);

    double speedLeft = speed;
    double distance;

    while(true) {
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
    if (speedLeft > 0) {
        QPointF dest = cities.at(toCity)->getLocation();
        QVector2D move(dest - location);
        move.normalize();
        move *= speedLeft;
        location.setX(location.x() + move.x());
        location.setY(location.y() + move.y());
//        double angle;
//        if (location.x() == dest.x())
//            angle = 3 * M_PI_2;
//        else
//            angle = qAtan(((double) dest.y() - location.y())/(dest.x() - location.x()));
//        double dx, dy;
//        dx = speedLeft * qCos(angle);
//        dy = speedLeft * qSin(angle);
//        if (location.y() > dest.y()) {
//            location.setX(location.x() + dx);
//            location.setY(location.y() + dy);
//        } else {
//            location.setX(location.x() - dx);
//            location.setY(location.y() - dy);
//        }
    }

    updateEllipse();
}

bool Ant::removeCity(int cityIndex)
{
    history.removeAt(cityIndex);
    if (cityIndex == fromCity || cityIndex == toCity)
        return true;
    if (cityIndex < fromCity)
        fromCity--;
    if (cityIndex < toCity)
        toCity--;
    if (toCity == fromCity)
        qDebug() << "From city == toCity after thing";
    return false;
}

void Ant::addCity()
{
    history.append(false);
}

void Ant::updateEllipse()
{
    ellipse->setRect(LOC_HELPER(location,ANT_RADIUS));
}

void Ant::chooseNextCity(QList<QSP<City> > &cities)
{
    if (cities.length() <= 1)
        return;

    if (toCity >= 0) {
        history[toCity] = true;
        fromCity = toCity;
    }

    double cityProb[cities.length()];
    double total = 0;
    QSP<Edge> edge;

    //if the full cycle has been completed, set all cities to unvisited (except the current one)
    if (!history.contains(false)) {
        for (int i = 0; i < cities.length(); i++) {
            history[i] = (i == toCity);
        }
    }

    //set chance to visit any particular neighbour that hasn't been visited this cycle
    for (int i = 0; i < cities.length(); i++) {
        cityProb[i] = 0;
        if (!history.at(i)) {
            edge = cities.at(fromCity)->edgeForNeighbour(i);
//            cityProb[i] = (1.0 + edge->getPheromone()) / (cities.at(fromCity)->distance(i) * MAX_PHEROMONE);
            cityProb[i] = qPow((1.0 + edge->getPheromone())/MAX_PHEROMONE, PHEROMONE_WEIGHT) * qPow(1.0/cities.at(fromCity)->distance(i), DISTANCE_WEIGHT);
            total += cityProb[i];
        }
    }

    for (int i = 0; i < cities.length(); i++) {
        cityProb[i] /= total;
    }

    //pick a neighbour proportional to its inverse distance and pheromone
    double random = ((double)qrand() / RAND_MAX);
    for (int i = 0; i < cities.length(); i++) {
        if (random < cityProb[i] && !history.at(i)) {
            toCity = i;
            break;
        }
        random -= cityProb[i];
    }

    //ant speed gets set based on distance to next city if proportional move is true
    if (proportionalMove) {
        speed = cities.at(fromCity)->distance(toCity) / ANT_PROPORTIONAL_STEPS;
    }

    if (fromCity == toCity) {
        qDebug() << "same city chosen" << cityProb[fromCity] << random;
    } else {
        cities.at(fromCity)->addPheromone(toCity, PHEROMONE_ADD);
    }
}
