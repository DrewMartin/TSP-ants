#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tours.h"
#include <qmath.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    reset();
    addCityToScene(QPoint(0, 0));

    cityCountChanged(ui->cityCountSlider->value());

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    setWidgetsEnabled(false);
}

void MainWindow::stop()
{
    setWidgetsEnabled(true);
}

void MainWindow::reset()
{
    cityGraphicsItems.clear();
    cities.clear();
    scene = QSharedPointer<QGraphicsScene>(new QGraphicsScene(0, 0, MAX_X, MAX_Y));
    ui->graphicsView->setScene(scene.data());
    usingPresetTour = false;
}

void MainWindow::cityCountChanged(int val)
{
    ui->cityCountLabel->setText(QString::number(val));
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
            addCityToScene(p);
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

int sqDist(const QPoint &p1, const QPoint &p2) {
    return qPow(p1.x() - p2.x(), 2) + qPow(p1.y() - p2.y(), 2);
}

void MainWindow::viewClicked(QPoint p, Qt::MouseButton button)
{
    if (usingPresetTour)
        return;

    if (button == Qt::LeftButton) {
        p.setX(qBound(0, p.x(), MAX_X));
        p.setY(qBound(0, p.y(), MAX_Y));
        addCityToScene(p);
    } else if (button == Qt::RightButton) {
        int i = 0;
        int maxDist = qPow(CITY_RADIUS+1, 2);
        QMutableListIterator<QPoint> iter(cities);
        while (iter.hasNext()) {
            QPoint cPoint = iter.next();
            if (sqDist(p, cPoint) < maxDist) {
                delete cityGraphicsItems[i];
                cityGraphicsItems.removeAt(i);
                iter.remove();
                i--;
            }
            i++;
        }
    }
}

void MainWindow::tourHelper(const QList<QPoint> &tour, const QList<int> &opt)
{
    reset();
    usingPresetTour = true;
    QBrush brush(Qt::black);
    QPen pen(brush, 0);

    QPoint curr, next;
    for (int i = 0; i < tour.length(); i++) {
        addCityToScene(tour.at(i));
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
    ui->stopButton->setEnabled(!enabled);
}

void MainWindow::addCityToScene(QPoint p)
{
    QPen pen(QColor(CITY_COLOR));
    QBrush brush(QColor(CITY_COLOR),Qt::SolidPattern);
    int x = p.x()-CITY_RADIUS, y = p.y()-CITY_RADIUS;
    QGraphicsEllipseItem *item = scene->addEllipse(x, y, CITY_RADIUS*2+1, CITY_RADIUS*2+1, pen, brush);
    item->setZValue(CITY_Z);
    cityGraphicsItems.append(item);
    cities.append(p);
}
