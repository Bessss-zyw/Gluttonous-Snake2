#include "snake.h"
#include <QPainter>
#include <QKeyEvent>
#include <QtDebug>

Snake::Snake(QWidget *parent) : QWidget(parent)
{
    this->keyPos = new Qt::Key[4];

    SnakeColor.setRgb(200,100,100);
    snakeNode = new QPoint[10];
    direction.setX(10);direction.setY(0);

    currentSize = 0;
    maxSize = 10;
    score = 0;
    immortal = 0;

    QPoint temp(50,200);
    for (int i = 0;i < 3;++i){
        addNode(temp + QPoint(i*10,0));
    }
}

Snake::Snake (Qt::Key up ,Qt::Key down,Qt::Key left,Qt::Key right,QWidget *parent): QWidget(parent)
{
    this->keyPos = new Qt::Key[4];
    this->keyPos[0] = up;
    this->keyPos[1] = down;
    this->keyPos[2] = left;
    this->keyPos[3] = right;

    SnakeColor.setRgb(200,100,100);
    snakeNode = new QPoint[10];
    direction.setX(10);direction.setY(0);

    currentSize = 0;
    maxSize = 10;
    score = 0;
    immortal = 0;

    QPoint temp(50,300);
    for (int i = 0;i < 3;++i){
        addNode(temp + QPoint(i*10,0));
    }

}

void Snake::changeColor(QColor color)
{
    SnakeColor = color;
}




void Snake::doubleSnake()
{
    QPoint *temp = new QPoint[maxSize*2];
    for(int i = 0;i < currentSize;++i)
    {
        temp[i] = snakeNode[i];
    }

    delete [] snakeNode;
    snakeNode = temp;

    maxSize *= 2;
}

void Snake::addNode(int x,int y)
{
    if(currentSize == maxSize) doubleSnake();

    snakeNode[currentSize].setX(x);
    snakeNode[currentSize].setY(y);

    currentSize++;
}

void Snake::addNode(QPoint dire)
{
    if(currentSize == maxSize) doubleSnake();

    snakeNode[currentSize] = dire;

    currentSize++;
}

void Snake::subNode(int n)
{
    if(currentSize-n < 3){
        for(int i = 0;i < 3;++i){
            snakeNode[i] = snakeNode[currentSize-3+i];
        }
        currentSize = 3;
    }
    else{
        for(int i = 0;i < currentSize-n ;++i){
            snakeNode[i] = snakeNode[i+n];
        }
        currentSize -= n;
    }
}

QPoint Snake::head() const
{
    return snakeNode[currentSize-1];
}



QPoint *Snake::snakePos() const
{
    return snakeNode;
}

QPoint Snake::direct() const
{
    return direction;
}

int Snake::Size() const
{
    return currentSize;
}

int Snake::Score() const
{
    return score;
}




void Snake::addScore(int dScore)
{
    score += dScore;
}

void Snake::snakeAutoMove()
{
    for (int i = 0;i<currentSize-1;++i){
        snakeNode[i].setX(snakeNode[i+1].x());
        snakeNode[i].setY(snakeNode[i+1].y());
    }
    snakeNode[currentSize-1].setX(snakeNode[currentSize-1].x()+direction.x());
    snakeNode[currentSize-1].setY(snakeNode[currentSize-1].y()+direction.y());
}

void Snake::changeDirect(QPoint temp)
{
    direction.setX(temp.x());
    direction.setY(temp.y());
}



void Snake::revival()
{
    //让蛇转向（改变节点位置，改变前进方向
    QPoint temp;
    for(int i = 0;i < currentSize/2;++i){
        temp = snakeNode[i];
        snakeNode[i] = snakeNode[currentSize-1-i];
        snakeNode[currentSize-1-i] = temp;
    }
    changeDirect(snakeNode[currentSize-1]-snakeNode[currentSize-2]);

    //减少一次复活机会
    immortal -= 1;
    qDebug() << "immortal = " << immortal;
}


bool Snake::ifHitRect(QPoint *l, int x, int y) const
{
    QPoint loc(l->x(),l->y());

    //upline
    for(int i = 0;i < x/10;++i){
        if(head() + direction == loc + QPoint(i*10,0)) return true;
    }

    //downline
    if(y > 10){
        for(int i = 0;i < x/10;++i){
            if(head() + direction == loc + QPoint(i*10,y-10)) return true;
        }
    }

    //left
    if(y > 20){
        for(int i = 1;i < y/10-1;++i){
            if(head() + direction == loc + QPoint(0,i*10)) return true;
        }

        if(x > 10){
            for(int i = 1;i < y/10-1;++i){
                if(head() + direction == loc + QPoint(x-10,i*10)) return true;
            }
        }
    }

    return false;
}

bool Snake::ifHitBody(QPoint dire) const
{
    for (int i = 0;i < currentSize-3;++i) {
        if(head() + dire == snakeNode[i])   return true;
    }
    return false;
}

bool Snake::ifHitOtherSnake(Snake *s) const
{
    for (int i = 0;i < s->currentSize;++i) {
        if(head() + direct() == s->snakeNode[i])   return true;
    }
    return false;
}




/*
int Snake::keyPress(QKeyEvent *event)
{
    int temp = 1;

    if(event->key() == keyPos[0]){   //up
        temp = judgeAndMove(QPoint(0,-10));
    }
    else if (event->key() == keyPos[1]) {   //down
        temp = judgeAndMove(QPoint(0,10));
    }
    else if (event->key() == keyPos[2]) {   //left
        temp = judgeAndMove(QPoint(-10,0));
    }
    else if (event->key() == keyPos[3]) {   //right
        temp = judgeAndMove(QPoint(10,0));
    }

    return temp;

}



int Snake::judgeAndMove(QPoint dire)
{
    //正常前进 0，不作为 1，碰到障碍物dead 2,吃到水果 345678

    //是否死亡（撞墙）
    if(head().x()+dire.x() == -10 || head().x()+dire.x() == 600 || head().y()+dire.y() == -10 || head().y()+dire.y() == 510){
        return 2;
    }

    //是否死亡（碰到自己）
    for (int i = 0;i < currentSize-3;++i) {
        if(head().x()+dire.x() == snakeNode[i].x() && head().y()+dire.y() == snakeNode[i].y())   return 2;
    }

    //是否能往该方向前进
    if(snakeNode[currentSize-1] + dire == snakeNode[currentSize-2])  return 1;
    else {
        changeDirect(dire);
        snakeAutoMove();
        return 0;
    }
}

*/




