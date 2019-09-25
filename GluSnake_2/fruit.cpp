#include "fruit.h"
#include <QtDebug>


Fruit::Fruit(int pro)
{
    property = pro;

    switch (pro) {
    case 0:
        this->setPixmap(QPixmap(":/fruit/pic/pineapple.png/"));
        value = 1;
        break;
    case 1:
        this->setPixmap(QPixmap(":/fruit/pic/strawberry.png/"));
        value = 2;
        break;
    case 2:
        this->setPixmap(QPixmap(":/fruit/pic/kiwi.png/"));
        value = 3;
        break;
    case 3:
        this->setPixmap(QPixmap(":/fruit/pic/egg.png/"));
        this->
        value = 0;
        break;
    case 4:
        this->setPixmap(QPixmap(":/fruit/pic/dimond.png/"));
        value = 10;
        break;
    case 5:
        this->setPixmap(QPixmap(":/fruit/pic/bomb.png/"));
        value = -5;
        break;
    }

    rePos();
}



Fruit::Fruit(int pro,QPoint position)
{
    property = pro;

    switch (pro) {
    case 0:
        this->setPixmap(QPixmap(":/fruit/pic/pineapple.png/"));
        value = 1;
        break;
    case 1:
        this->setPixmap(QPixmap(":/fruit/pic/strawberry.png/"));
        value = 2;
        break;
    case 2:
        this->setPixmap(QPixmap(":/fruit/pic/kiwi.png/"));
        value = 3;
        break;
    case 3:
        this->setPixmap(QPixmap(":/fruit/pic/egg.png/"));
        value = 0;
        break;
    case 4:
        this->setPixmap(QPixmap(":/fruit/pic/dimond.png/"));
        value = 10;
        break;
    case 5:
        this->setPixmap(QPixmap(":/fruit/pic/bomb.png/"));
        value = -5;
        break;
    }

    this->setPos(position);
}


int Fruit::returnValue() const
{
    return value;
}

int Fruit::returnPro() const
{
    return property;
}


void Fruit::rePos(int maxX,int maxY)
{
    int tempx = generateRandomNumber(0,maxX);
    this->setX(tempx*10);

    int tempy = generateRandomNumber(0,maxY);
    this->setY(tempy*10);
}




int generateRandomNumber(int min, int max)
{
//    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//    return qrand() % (max-min+1) + min;

    int v = QRandomGenerator::global()->bounded(min, max+1);
    return v;
}


