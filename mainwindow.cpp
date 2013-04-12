#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tours.h"
#include <qmath.h>
#include <QDebug>
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    reset();

    cityCountChanged(ui->cityCountSlider->value());
    pheromoneImportanceChanged(ui->pheromoneImportanceSlider->value());
    distanceImportanceChanged(ui->distanceImportanceSlider->value());

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

    ui->decayRateSlider->setValue(PHEROMONE_DEFAULT_DECAY_RATE);
    ui->speedSlider->setValue(ANT_SPEED);
    speedChanged(ui->speedSlider->value());
    decayRateChanged(ui->decayRateSlider->value());

    ui->graphicsView->setToolTip("Left click to place a new city. Right click to remove any city under the mouse cursor.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    ants.clear();
    setWidgetsEnabled(false);

    QSP<Ant> ant;

    for (int i = 0; i < cities.length(); i++) {
        ant = QSP<Ant>(new Ant(cities[i], i, cities.length()));
        ants.append(ant);
        scene->addItem(ant->getGraphicsItem());
        cities.at(i)->reset();
    }
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
    scene = QSharedPointer<QGraphicsScene>(new QGraphicsScene(0, 0, MAX_X, MAX_Y));
    ui->graphicsView->setScene(scene.data());
    usingPresetTour = false;
}

void MainWindow::cityCountChanged(int val)
{
    ui->cityCountLabel->setText(QString::number(val));
}

void MainWindow::decayRateChanged(int val)
{
    double decay = val/100.0;
    Edge::setDecayRate(decay);
    ui->decayRateLabel->setText(QString::number(decay, 'f', 2));
}

void MainWindow::speedChanged(int val)
{
    ui->speedLabel->setText(QString::number(val));
    Ant::setSpeed(val);
}

void MainWindow::pheromoneImportanceChanged(int val)
{
    Ant::setPheromoneImportance(val);
    ui->pheromoneImportanceLabel->setText(QString::number(val));
}


void MainWindow::distanceImportanceChanged(int val)
{
    Ant::setDistanceImportance(val);
    ui->distanceImportanceLabel->setText(QString::number(val));
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
    } else if (button == Qt::RightButton) {
        int i = 0;
        int maxDist = qPow(CITY_RADIUS+1, 2);
        QMutableListIterator<QSP<City> > iter(cities);
        QMutableListIterator<QSP<Ant> > antIter(ants);
        while (iter.hasNext()) {
            QPointF cPoint = iter.next()->getLocation();
            if (sqDist(p, cPoint) < maxDist) {
                iter.remove();
                antIter.toFront();
                while (antIter.hasNext()) {
                    if (antIter.next()->removeCity(i)) {
                        antIter.remove();
                    }
                }
                for (int j = 0; j < cities.length(); j++)
                    cities.at(j)->removeCity(i);
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

void MainWindow::tourHelper(const QList<QPoint> &tour, const QList<int> &opt)
{
    reset();
    usingPresetTour = true;
    QBrush brush(Qt::black);
    QPen pen(brush, 0);
    pen.setStyle(Qt::DotLine);

    QPoint curr, next;
    for (int i = 0; i < tour.length(); i++) {
        addCityToScene(tour.at(i), tour.length());
        curr = tour.at(opt.at(i));
        next = tour.at(opt.at((i+1) % tour.length()));
        scene->addLine(curr.x(), curr.y(), next.x(), next.y(), pen)->setZValue(LINE_Z);
    }
}

void MainWindow::setWidgetsEnabled(bool enabled)
{
    ui->generateCitiesGroup->setEnabled(enabled);
    ui->startButton->setEnabled(enabled);
    ui->resetButton->setEnabled(enabled);
    ui->tourGroup->setEnabled(enabled);
    ui->decayGroup->setEnabled(enabled);
    ui->importanceGroup->setEnabled(enabled);

    ui->stopButton->setEnabled(!enabled);

    running = !enabled;
}

void MainWindow::addCityToScene(QPoint p, int expectedNumCities)
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
        for (i = 0; i < cities.length(); i++)
            cities.at(i)->doDecay();

        int shortTourIndex = -1;
        double bestPheromone = 0.0, shortestTour = LONG_LONG_MAX;
        BestDistancePheromone curr;
        for (i = 0; i < ants.length(); i++) {
            curr = ants.at(i)->addPheromoneToTour(cities);
            if (curr.first < shortestTour) {
                shortestTour = curr.first;
                shortTourIndex = i;
            }
            if (curr.second > bestPheromone)
                bestPheromone = curr.second;
        }

        QList<int> bestTour = ants.at(shortTourIndex)->getTour();
        for (i = 1; i < bestTour.length(); i++) {
            cities.at(bestTour.at(i))->edgeForNeighbour(bestTour.at(i-1))->setBest();
        }
        cities.at(bestTour.at(0))->edgeForNeighbour(bestTour.last())->setBest();

        for (i = 0; i < cities.length(); i++)
            cities.at(i)->updateLines(bestPheromone);

        for (i = 0; i < ants.length(); i++) {
            ants.at(i)->resetTour();
        }

    }

//    qDebug() << "time elapsed" << timer.elapsed();
    QTimer::singleShot(qMax(0, FRAME_TIME - timer.elapsed()), this, SLOT(timerSlot()));
}
