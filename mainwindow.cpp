#include "mainwindow.h"
#include "ui_mainwindow.h"

#define CITY_COLOR "#ff9c00"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    reset();

    cityCountChanged(ui->cityCountSlider->value());

    setWidgetsEnabled(true);

    connect(ui->cityCountSlider, SIGNAL(valueChanged(int)), SLOT(cityCountChanged(int)));
    connect(ui->startButton, SIGNAL(clicked()), SLOT(start()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(stop()));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(reset()));
    connect(ui->generateButton, SIGNAL(clicked()), SLOT(generateClicked()));
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
    scene = QSharedPointer<QGraphicsScene>(new QGraphicsScene(0, 0, MAX_X + BUFFER*2, MAX_Y + BUFFER*2));
    ui->graphicsView->setScene(scene.data());
}

void MainWindow::cityCountChanged(int val)
{
    ui->cityCountLabel->setText(QString::number(val));
}

void MainWindow::generateClicked()
{
    reset();
    int count = ui->cityCountSlider->value();

    cities.clear();
    QSet<QString> unique;
    QPoint p;
    QString hash;

    while (cities.length() < count) {
        p = QPoint(qrand() % MAX_X, qrand() % MAX_Y);
        hash = QString("%1,%2").arg(p.x()).arg(p.y());
        if (!unique.contains(hash)) {
            unique.insert(hash);
            cities.append(p);
            addCityToScene(p);
        }
    }
}

void MainWindow::setWidgetsEnabled(bool enabled)
{
    ui->generateCitiesGroup->setEnabled(enabled);
    ui->startButton->setEnabled(enabled);
    ui->resetButton->setEnabled(enabled);
    ui->optionsGroups->setEnabled(enabled);
    ui->stopButton->setEnabled(!enabled);
}

void MainWindow::addCityToScene(QPoint p)
{
    QPen pen(QColor(CITY_COLOR));
    QBrush brush(QColor(CITY_COLOR),Qt::SolidPattern);
    scene->addEllipse(p.x()-CITY_RADIUS+BUFFER,p.y()-CITY_RADIUS+BUFFER, CITY_RADIUS*2+1, CITY_RADIUS*2+1, pen, brush)->setZValue(5);
}
