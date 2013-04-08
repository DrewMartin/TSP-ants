#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPoint>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QSharedPointer>

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
    void generateClicked();
    void capitalTourClicked();
    void berlinTourClicked();
    void fiftyOneCitiesClicked();
    void viewClicked(QPoint p, Qt::MouseButton button);

private:
    void tourHelper(const QList<QPoint> &tour,const QList<int> &opt);
    void setWidgetsEnabled(bool enabled);
    void addCityToScene(QPoint p);

    QList<QPoint> cities;
    QSP<QGraphicsScene> scene;
    QList<QGraphicsItem*> cityGraphicsItems;
    bool usingPresetTour;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
