#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QString>

#include "setting.h"
#include "mainwindow.h"

namespace Ui {
class Interface;
}

class Interface : public QWidget
{
    Q_OBJECT

public:
    explicit Interface(QWidget *parent = nullptr);
    ~Interface();

public slots:
    void PlaySingle();
    void PlayDouble();
    void AI();
    void AgainstAI();
    void more();

private:
    void writeToFile(QString cnt);

private:
    Ui::Interface *ui;
    MainWindow *w;
    Setting *s;
    QGraphicsScene *sc;
};

#endif // INTERFACE_H
