#include "ant.h"
#include <QDebug>
bool Ant::instantMove = false;

Ant::Ant(QSP<City> &startingCity, int index, int numCities) :
    Entity(QPoint(startingCity->getLocation())),
    currentCity(index)
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

void Ant::setInstantMove(bool instant)
{
    instantMove = instant;
}

QGraphicsItem *Ant::getGraphicsItem()
{
    return ellipse;
}

void Ant::update(QList<QSharedPointer<City> > &cities)
{
    int prev = currentCity;
    double cityProb[cities.length()];
    double total = 0, visitedCount = 0;
    QSP<Edge> edge;
    for (int i = 0; i < cities.length(); i++) {
        cityProb[i] = 0;
        if (!history.at(i)) {
            edge = cities.at(currentCity)->edgeForNeighbour(i);
            cityProb[i] = 1.0 * (1.0 + edge->getPheromone()) / cities.at(currentCity)->distance(cities.at(i));
            total += cityProb[i];
        } else {
            visitedCount++;
        }
    }

    double random = ((double)qrand() / RAND_MAX) * total;
    for (int i = 0; i < cities.length(); i++) {
        if (random < cityProb[i] && i != currentCity) {
            currentCity = i;
            break;
        }
        random -= cityProb[i];
    }

    if (visitedCount >= cities.length() - 1) {
        for (int i = 0; i < history.length(); i++) {
            history[i] = false;
        }
    }

    history[currentCity] = true;
    if (currentCity == prev) {
        qDebug() << cityProb[currentCity];
    }
    cities.at(prev)->addPheromone(currentCity, PHEROMONE_ADD);
    updateEllipse(cities.at(currentCity)->getLocation());
}

bool Ant::removeCity(int cityIndex)
{
    history.removeAt(cityIndex);
    if (cityIndex == currentCity)
        return true;
    if (cityIndex < currentCity)
        currentCity--;
    return false;
}

void Ant::addCity()
{
    history.append(false);
}

void Ant::updateEllipse(const QPoint &newPoint)
{
    ellipse->setRect(LOC_HELPER(newPoint,ANT_RADIUS));
}
