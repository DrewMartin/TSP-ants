#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPoint>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QSharedPointer>

#include "ant.h"
#include "city.h"
#include "common.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void start();
    void stop();
    void reset();
    void cityCountChanged(int val);
    void decayRateChanged(int val);
    void speedChanged(int val);
    void pheromoneImportanceChanged(int val);
    void distanceImportanceChanged(int val);
    void defaultPheromoneChanged(int val);
    void framerateChanged(int val);
    void mutationRateChanged(int val);
    void generateClicked();
    void capitalTourClicked();
    void berlinTourClicked();
    void fiftyOneCitiesClicked();
    void viewClicked(QPoint p, Qt::MouseButton button);
    void timerSlot();

private:
    void tourHelper(const QList<QPointF> &tour, const QList<int> &opt);
    void setWidgetsEnabled(bool enabled);
    void addCityToScene(QPointF p, int expectedNumCities = -1);
    void updateLoop();

    Ui::MainWindow *ui;
    QList<QSP<City> > cities;
    QSP<QGraphicsScene> scene;
    QList<QSP<Ant> > ants;
    QList<QSP<QGraphicsItem> > pheromoneLines;
    bool usingPresetTour;
    bool running;
    int frameTime;

};

#endif // MAINWINDOW_H
