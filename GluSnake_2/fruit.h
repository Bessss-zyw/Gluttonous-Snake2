#ifndef FRUIT_H
#define FRUIT_H

#include <QGraphicsPixmapItem>
#include <QString>
#include <QTime>
#include <QtGlobal>
#include <QPoint>
#include <QRandomGenerator>

int generateRandomNumber(int min, int max);

class Fruit : public QGraphicsPixmapItem
{
    friend class myScene;

private:
    int property;   //pineapple 0,strawberry 1,kiwi 2,egg 3,dimond 4,bomb 5
    int value;    //pineapple 1,strawberry 2,kiwi 3,egg 0,dimond 10,bomb -5

public:
    Fruit(int pro);
    Fruit(int pro,QPoint position);

    int returnValue() const;   //返回水果value
    int returnPro() const;   //返回水果属性

    void rePos(int maxX = 56, int maxY = 47);  //重置位置

};

#endif // FRUIT_H



