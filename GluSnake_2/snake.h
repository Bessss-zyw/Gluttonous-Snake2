#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QPoint>
#include <QTimer>
#include <QEvent>

#include "fruit.h"

//蛇的初始长度为3，最短长度也为3
class Snake : public QWidget
{
    Q_OBJECT

friend class myScene;


private:
    QColor SnakeColor;
    Qt::Key *keyPos;   //控制蛇移动的按键
    QPoint *snakeNode;  //保存蛇的位置
    QPoint direction;  //保存蛇自动运动方向
    int currentSize;   //snakeNode[currentSize-1]为蛇头位置
    int maxSize;   //snakeNode数组当前大小
    int score;   //记录蛇的分数

private:
    void doubleSnake();   //扩大蛇节点数组
    void addNode(int x,int y);  //添加蛇的节点
    void addNode(QPoint dire);  //添加蛇的节点
    void subNode(int n);   //减少蛇节点
    QPoint head() const;   //返回蛇头位置

public:
    int immortal;   //记录蛇是否有复活机会

public:
    //指定了parent父类，无需析构函数
    explicit Snake(QWidget *parent = nullptr);
    Snake (Qt::Key up,Qt::Key down,Qt::Key left,Qt::Key right,QWidget *parent = nullptr);  //构造函数，可以规定用哪几个键控制蛇
    void changeColor(QColor color);

    QPoint * snakePos() const;   //公有函数，方便从外部获取蛇节点位置
    QPoint direct() const;   //公有函数，方便从外部获取蛇运动方向
    int Size() const;   //公有函数，方便从外部获取蛇节点个数
    int Score() const;   //公有函数，方便从外部获取蛇当前分数

    void addScore(int dScore);  //加或者减分数
    void changeDirect(QPoint temp);   //让蛇改变移动方向
    void snakeAutoMove();   //让蛇向其direction自行移动

    void revival();    //复活（蛇尾变蛇头）
    bool ifHitRect(QPoint *l,int x,int y) const;   //判断蛇是否与该矩形相撞
    bool ifHitBody(QPoint dire) const;  //判断蛇是否碰到自己
    bool ifHitOtherSnake(Snake *s) const;   //判断是否碰到其他蛇


signals:


public slots:

};



#endif // SNAKE_H
