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

private:
    void setWidgetsEnabled(bool enabled);
    void addCityToScene(QPoint p);

    QList<QPoint> cities;
    QSP<QGraphicsScene> scene;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
