#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tours.h"
#include <qmath.h>
#include <QDebug>
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    frameTime(FRAME_TIME)
{
    ui->setupUi(this);
    reset();

    cityCountChanged(ui->cityCountSlider->value());
    pheromoneImportanceChanged(ui->pheromoneImportanceSlider->value());
    distanceImportanceChanged(ui->distanceImportanceSlider->value());
    speedChanged(ui->speedSlider->value());
    decayRateChanged(ui->decayRateSlider->value());
    defaultPheromoneChanged(ui->defaultPheromoneSlider->value());
    framerateChanged(ui->framerateSlider->value());
    mutationRateChanged(ui->mutationSlider->value());

    setWidgetsEnabled(true);

    connect(ui->cityCountSlider, SIGNAL(valueChanged(int)), SLOT(cityCountChanged(int)));
    connect(ui->startButton, SIGNAL(clicked()), SLOT(start()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(stop()));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(reset()));
    connect(ui->capitalTourButton, SIGNAL(clicked()), SLOT(capitalTourClicked()));
    connect(ui->fiftyOneTourButton, SIGNAL(clicked()), SLOT(fiftyOneCitiesClicked()));
    connect(ui->berlinTourButton, SIGNAL(clicked()), SLOT(berlinTourClicked()));
    connect(ui->generateButton, SIGNAL(clicked()), SLOT(generateClicked()));
    connect(ui->graphicsView, SIGNAL(clicked(QPoint,Qt::MouseButton)), SLOT(viewClicked(QPoint,Qt::MouseButton)));
    connect(ui->decayRateSlider, SIGNAL(valueChanged(int)), SLOT(decayRateChanged(int)));
    connect(ui->speedSlider, SIGNAL(valueChanged(int)), SLOT(speedChanged(int)));
    connect(ui->pheromoneImportanceSlider, SIGNAL(valueChanged(int)), SLOT(pheromoneImportanceChanged(int)));
    connect(ui->distanceImportanceSlider, SIGNAL(valueChanged(int)), SLOT(distanceImportanceChanged(int)));
    connect(ui->defaultPheromoneSlider, SIGNAL(valueChanged(int)), SLOT(defaultPheromoneChanged(int)));
    connect(ui->framerateSlider, SIGNAL(valueChanged(int)), SLOT(framerateChanged(int)));
    connect(ui->mutationSlider, SIGNAL(valueChanged(int)), SLOT(mutationRateChanged(int)));
    connect(ui->showPheromoneCheckbox, SIGNAL(clicked(bool)), SLOT(showPheromoneClicked(bool)));

    ui->graphicsView->setToolTip("Left click to place a new city. Right click to remove any city under the mouse cursor.");
}

MainWindow::~MainWindow()
{
    reset();
    delete ui;
}

void MainWindow::start()
{
    ants.clear();
    setWidgetsEnabled(false);
    bestSoFar = LONG_LONG_MAX;
    bestTour.clear();
    QSP<Ant> ant;

    for (int i = 0; i < cities.length(); i++) {
        ant = QSP<Ant>(new Ant(cities[i], i, cities.length()));
        ants.append(ant);
        scene->addItem(ant->getGraphicsItem());
        cities.at(i)->reset();
    }
    layoutChanged = false;

    ui->pathLengthEdit->setText("");
    tours = 0;
    ui->toursLengthEdit->setText(QString::number(tours));
    updateLoop();
}

void MainWindow::stop()
{
    setWidgetsEnabled(true);
}

void MainWindow::reset()
{
    ants.clear();
    cities.clear();
    Edge::setScale(1.0, 1.0);
    Entity::setScale(1.0, 1.0);
    ui->graphicsView->resetTransform();
    scene = QSharedPointer<QGraphicsScene>(new QGraphicsScene(0, 0, MAX_X, MAX_Y));
    ui->graphicsView->setSceneRect(0, 0, MAX_X, MAX_Y);
    ui->graphicsView->setScene(scene.data());
    usingPresetTour = false;
    optimal = 0.0;
}

void MainWindow::cityCountChanged(int val)
{
    ui->cityCountLabel->setText(QString::number(val));
}

void MainWindow::decayRateChanged(int val)
{
    double decay = val/100.0;
    Edge::setDecayRate(val);
    ui->decayRateLabel->setText(QString::number(decay, 'f', 2));
}

void MainWindow::speedChanged(int val)
{
    if (val > 100) {
        ui->speedLabel->setText("∞");
    } else {
        ui->speedLabel->setText(QString::number(val));
    }
    Ant::setSpeed(val);
}

void MainWindow::pheromoneImportanceChanged(int val)
{
    Ant::setPheromoneImportance(val);
    ui->pheromoneImportanceLabel->setText(QString::number(val / 10.0, 'f', 1));
}


void MainWindow::distanceImportanceChanged(int val)
{
    Ant::setDistanceImportance(val);
    ui->distanceImportanceLabel->setText(QString::number(val / 10.0, 'f', 1));
}

void MainWindow::defaultPheromoneChanged(int val)
{
    double amount = val / 100.0f;
    Edge::setDefaultPheromone(amount);
    ui->defaultPheromoneLabel->setText(QString::number(amount, 'f', 2));
}

void MainWindow::framerateChanged(int val)
{
    ui->framerateLabel->setText(QString::number(val));
    frameTime = 1000 / val;
}

void MainWindow::mutationRateChanged(int val)
{
    ui->mutationLabel->setText(QString::number(val));
    Ant::setMutation(val);
}

void MainWindow::showPheromoneClicked(bool checked)
{
    Edge::setShowPheromone(checked);
}

void MainWindow::generateClicked()
{
    reset();
    int count = ui->cityCountSlider->value();

    QSet<QString> unique;
    QPoint p;
    QString hash;

    while (cities.length() < count) {
        p = QPoint(qrand() % MAX_GEN_X, qrand() % MAX_GEN_Y);
        hash = QString("%1,%2").arg(p.x()).arg(p.y());
        if (!unique.contains(hash)) {
            unique.insert(hash);
            addCityToScene(p, count);
        }
    }
}

void MainWindow::capitalTourClicked()
{
    tourHelper(capitalTour, capitalOpt);
}

void MainWindow::fiftyOneCitiesClicked()
{
    tourHelper(fiftyOneTour, fiftyOneOpt);
}

void MainWindow::berlinTourClicked()
{
    tourHelper(berlinTour, berlinOpt);
}

void MainWindow::viewClicked(QPoint p, Qt::MouseButton button)
{
    if (usingPresetTour)
        return;
    QSP<Ant> ant;

    if (button == Qt::LeftButton) {
        p.setX(qBound(0, p.x(), MAX_X));
        p.setY(qBound(0, p.y(), MAX_Y));
        addCityToScene(p);
        if (running) {
            for (int i = 0; i < ants.length(); i++)
                ants.at(i)->addCity();

            ant = QSP<Ant>(new Ant(cities.last(), cities.length()-1, cities.length()));
            ants.append(ant);
            scene->addItem(ant->getGraphicsItem());

        }
        layoutChanged = true;
    } else if (button == Qt::RightButton) {
        int i = 0;
        int maxDist = qPow(CITY_RADIUS+1, 2);
        QMutableListIterator<QSP<City> > iter(cities);
        QMutableListIterator<QSP<Ant> > antIter(ants);
        while (iter.hasNext()) {
            QPointF cPoint = iter.next()->getLocation();
            if (sqDist(p, cPoint) < maxDist) {
                layoutChanged = true;
                iter.remove();
                antIter.toFront();
                while (antIter.hasNext()) {
                    if (antIter.next()->removeCity(i)) {
                        antIter.remove();
                    }
                }
                for (int j = 0; j < cities.length(); j++)
                    cities.at(j)->removeCity(i);
                for (int j = 0; j < bestTour.length(); j++) {
                    if (bestTour.at(j) == i) {
                        bestTour.removeAt(j);
                        j--;
                    } else if (bestTour.at(j) > i) {
                        bestTour[j] = bestTour[j] - 1;
                    }
                }
                i--;
            }
            i++;
        }

        if (running) {

            while (ants.length() < cities.length()) {
                i = qrand() % cities.length();
                ant = QSP<Ant>(new Ant(cities[i], i, cities.length()));
                scene->addItem(ant->getGraphicsItem());
                ants.append(ant);
            }

            while (ants.length() > cities.length()) {
                ants.pop_back();
            }
        }
    }
}

void MainWindow::timerSlot()
{
    updateLoop();
}

void MainWindow::tourHelper(const QList<QPointF> &tour, const QList<int> &opt)
{
    reset();
    usingPresetTour = true;

    QBrush brush(Qt::black);
    QPen pen(brush, 0);
    pen.setStyle(Qt::DotLine);

    QPointF curr, next;
    double length = 0.0;
    double maxY = 0, maxX = 0;
    for (int i = 0; i < tour.length(); i++) {
//        addCityToScene(tour.at(i), tour.length());
        curr = tour.at(opt.at(i));
        next = tour.at(opt.at((i+1) % tour.length()));
        length += dist(curr, next);
        if (curr.x() > maxX)
            maxX = curr.x();
        if (curr.y() > maxY)
            maxY = curr.y();

        scene->addLine(curr.x(), curr.y(), next.x(), next.y(), pen)->setZValue(LINE_Z);
    }
    optimal = length;
    qDebug() << "Optimal path length:" << length;
    ui->graphicsView->setSceneRect(0, 0, maxX * 1.02, maxY * 1.02);
    double scaleX = MAX_GEN_X / maxX, scaleY = MAX_GEN_Y / maxY;
    ui->graphicsView->scale(scaleX, scaleY);
    Edge::setScale(scaleX, scaleY);
    Entity::setScale(scaleX, scaleY);

    for (int i = 0; i < tour.length(); i++)
        addCityToScene(tour.at(i), tour.length());
}

void MainWindow::setWidgetsEnabled(bool enabled)
{
    ui->generateCitiesGroup->setEnabled(enabled);
    ui->startButton->setEnabled(enabled);
    ui->resetButton->setEnabled(enabled);
    ui->tourGroup->setEnabled(enabled);

    ui->stopButton->setEnabled(!enabled);

    running = !enabled;
}

void MainWindow::addCityToScene(QPointF p, int expectedNumCities)
{
    if (expectedNumCities < 0) {
        for (int i = 0; i < cities.length(); i++) {
            cities.at(i)->addCity();
        }
        expectedNumCities = cities.length() + 1;
    }
    QSP<City> city(new City(p, cities.length(), expectedNumCities));
    QSP<Edge> edge;
    for (int i = 0; i < cities.length(); i++) {
        edge = QSP<Edge>(new Edge(cities.at(i)->getLocation(), p));
        scene->addItem(edge->getGraphicsItem());
        city->addEdge(edge, i);
        cities.at(i)->addEdge(edge, cities.length());
    }
    cities.append(city);
    scene->addItem(city->getGraphicsItem());
}

void MainWindow::updateLoop()
{
    if (!running)
        return;

    QTime timer;
    timer.start();
    bool allDone = true;
    int i;

    for (i = 0; i < ants.length(); i++)
        allDone &= ants.at(i)->update(cities);

    if (allDone && cities.length() > 1) {
        tours++;
        ui->toursLengthEdit->setText(QString::number(tours));
        for (i = 0; i < cities.length(); i++)
            cities.at(i)->doDecay();

        int shortTourIndex = -1;
        double bestPheromone = 0.0, shortestTour = LONG_LONG_MAX;
        BestDistancePheromone curr;
        for (i = 0; i < ants.length(); i++) {
            curr = ants.at(i)->addPheromoneToTour(cities);
            if (curr.first < shortestTour && ants.at(i)->tourLength() == cities.length()) {
                shortestTour = curr.first;
                shortTourIndex = i;
            }
            if (curr.second > bestPheromone)
                bestPheromone = curr.second;
        }

        if (shortestTour < bestSoFar || layoutChanged) {
            bestSoFar = shortestTour;
            ui->pathLengthEdit->setText(QString::number(bestSoFar));

            for (i = 0; i < bestTour.length(); i++)
                cities.at(bestTour.at(i))->edgeForNeighbour(bestTour.at((i + 1) % bestTour.length()))->setBest(false);

            bestTour = ants.at(shortTourIndex)->getTour();
            for (i = 0; i < bestTour.length(); i++)
                cities.at(bestTour.at(i))->edgeForNeighbour(bestTour.at((i + 1) % bestTour.length()))->setBest(true);
            layoutChanged = false;

        }

        for (i = 0; i < cities.length(); i++)
            cities.at(i)->updateLines(bestPheromone);

        for (i = 0; i < ants.length(); i++) {
            ants.at(i)->resetTour();
        }

    }

//    qDebug() << "time elapsed" << timer.elapsed();
    QTimer::singleShot(qMax(1, frameTime - timer.elapsed()), this, SLOT(timerSlot()));
}
