#ifndef MYSCENE_H
#define MYSCENE_H

#include "snake.h"
#include "fruit.h"

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QTextStream>

class myScene : public QGraphicsScene
{
    Q_OBJECT
public:
    //指定了parent父类，无需析构函数
    explicit myScene(QObject *parent = nullptr);
    void addSnake(Snake *head);   //向场景中添加蛇
    void addBG();   //向场景中添加背景
    void addFruit();   //向场景中添加水果
    void addBarrier();   //向场景中添加障碍物

    void addNewFruit(int index);   //向场景中添加特定水果

    bool ifFoodOverlap(Fruit *foodx);
    bool ifEatFood(Snake *s,Fruit *foodx);   //判断s蛇是否吃到foodx水果
    bool ifHitBarrier(Snake *s);   //判断s蛇是否撞到barrier

    int keyPress(Snake *s,QKeyEvent *event);   //响应键盘事件
    int judgeAndMove(Snake *s,QPoint dire);   //判断蛇遇到情况并返回int值
                    //正常前进 6，方向往后 7，碰到障碍物dead 8,吃到水果 012345
    QPoint chooseSide(int n,QPoint hitPoint);
    int AIsnake();

private:
    void setMode();

signals:

public slots:
    void SpecialFoodUpdate();

private:
    Fruit **food;  //food[0/1]为水果，food[2]为鸡蛋，food[3]为钻石，food[4]为炸弹
    QPoint **barrier;   //barrier0-2 crosswise,barrier3-5 lengthwise


public:
    Snake *head1;
    Snake *head2;
    QTimer *ptimer1;
    QTimer *ptimer2;
    bool *foodExist;   //判断场景中是否添加特殊水果，food[0]为鸡蛋,food[1]为钻石，food[2]为炸弹,food[3]为more fruit
    bool AI;
};

#endif // MYSCENE_H
