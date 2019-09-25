#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myscene.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    void keyPressEvent(QKeyEvent *event);


private:
    Ui::MainWindow *ui;
    myScene *sc;
    QTimer *ptimer0;

private:
    void SceneUpdate();
    void Rule(int snake,int temp);

public slots:
    void btn_start();
    void btn_pause();
    void btn_end();
    void SceneAutoUpdate();
    void FruitAutoUpdate();
    void GameOver();
};

#endif // MAINWINDOW_H
