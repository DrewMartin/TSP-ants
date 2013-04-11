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
    decayRateChanged(ui->decayRateSlider->value());

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    ants.clear();
    setWidgetsEnabled(false);
    frameTime = MOVE_FRAME_TIME;
    if (ui->instantCheckbox->isChecked()) {
        frameTime = INSTANT_FRAME_TIME;
        Ant::setInstantMove(true);
    } else {
        Ant::setInstantMove(false);
    }

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
    Edge::setDecayRate(val / 100.0);
    ui->decayRateLabel->setText(QString::number(val / 100.0, 'f', 2));
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

    if (button == Qt::LeftButton) {
        p.setX(qBound(0, p.x(), MAX_X));
        p.setY(qBound(0, p.y(), MAX_Y));
        addCityToScene(p);
        if (running) {
            for (int i = 0; i < ants.length(); i++)
                ants.at(i)->addCity();
        }
    } else if (button == Qt::RightButton) {
        int i = 0;
        int maxDist = qPow(CITY_RADIUS+1, 2);
        QMutableListIterator<QSP<City> > iter(cities);
        QMutableListIterator<QSP<Ant> > antIter(ants);
        while (iter.hasNext()) {
            QPoint cPoint = iter.next()->getLocation();
            if (sqDist(p, cPoint) < maxDist) {
                iter.remove();
                antIter.toFront();
                while (antIter.hasNext()) {
                    if (antIter.next()->removeCity(i)) {
                        antIter.remove();
                    }
                }
                i--;
            }
            i++;
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
    ui->optionsGroups->setEnabled(enabled);
    ui->tourGroup->setEnabled(enabled);
    ui->decayGroup->setEnabled(enabled);

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

    for (int i = 0; i < cities.length(); i++)
        cities.at(i)->update();

    for (int i = 0; i < ants.length(); i++)
        ants.at(i)->update(cities);

    qDebug() << "time elapsed" << timer.elapsed();
    QTimer::singleShot(qMax(0, frameTime - timer.elapsed()), this, SLOT(timerSlot()));
}
