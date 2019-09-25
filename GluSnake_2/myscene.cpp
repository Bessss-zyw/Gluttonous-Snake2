#include "myscene.h"
#include <QPoint>
#include <QtDebug>
#include <QKeyEvent>
#include <QFile>
#include <QTextStream>
#include <QString>

myScene::myScene(QObject *parent) : QGraphicsScene(parent)
{
    ptimer1 = new QTimer;   //水果出现8秒后，若没被吃，改变位置
    ptimer2 = nullptr;

    //设置背景图片（600*510）
    addBG();

    //初始化barrier指针
    barrier = new QPoint*[6]{nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

    //设置1号蛇
    head1 = new Snake;    head2 = nullptr;
    addSnake(head1);
    AI = false;

    //设置水果并记录其属性
    foodExist = new bool[4]{0,0,0,0};  //1为存在，0为不存在
    food = new Fruit*[5]{nullptr,nullptr,nullptr,nullptr,nullptr};

    //设置游戏模式
    setMode();
}




//向场景中添加蛇
void myScene::addSnake(Snake *head)
{
    QPoint *pos = head->snakePos();
    int size = head->Size();
    QGraphicsRectItem **temp = new QGraphicsRectItem*[size];

    for(int i = 0;i < size;++i)
    {
        temp[i] = new QGraphicsRectItem;
        temp[i]->setRect(pos[i].x(),pos[i].y(),10,10);
        temp[i]->setBrush(QBrush(head->SnakeColor));
        this->addItem(temp[i]);
    }
}

//向场景中添加背景（600*510）
void myScene::addBG()
{
    QGraphicsPixmapItem *item1 = new QGraphicsPixmapItem;
    item1->setPixmap(QPixmap(":/bg/pic/background.jpg"));
    this->addItem(item1);

}

//向场景中添加水果
void myScene::addFruit()
{
    for(int i = 0;i < 5;++i){
        if(food[i] != nullptr){
            Fruit *temp = new Fruit(food[i]->property,food[i]->scenePos().toPoint());
            this->addItem(temp);
        }
    }
}

//向场景中添加障碍物
void myScene::addBarrier()
{    
    //障碍物3个为一组记录位置（横：（0，0），（50，-20），（100，-10）；竖：（0，0），（40，20），（-10，40），（30，60）

    for(int n = 0 ; n < 3 ; ++n)
    {
        if(barrier[n] != nullptr){   //crosswise
            QGraphicsPixmapItem *item1 = new QGraphicsPixmapItem;
            item1->setPos(barrier[n]->x(),barrier[n]->y());
            item1->setPixmap(QPixmap(":/barrier/pic/barrier_line.png"));
            this->addItem(item1);

            QGraphicsPixmapItem *item2 = new QGraphicsPixmapItem;
            item2->setPos(barrier[n]->x() + 50,barrier[n]->y() -20);
            item2->setPixmap(QPixmap(":/barrier/pic/barrier_line.png"));
            this->addItem(item2);

            QGraphicsPixmapItem *item3 = new QGraphicsPixmapItem;
            item3->setPos(barrier[n]->x() + 100,barrier[n]->y() - 10);
            item3->setPixmap(QPixmap(":/barrier/pic/barrier_line.png"));
            this->addItem(item3);
        }
    }

    for(int n = 3 ; n < 6 ; ++n)   //lengthwise
    {
        if(barrier[n] != nullptr){
            QGraphicsPixmapItem *item1 = new QGraphicsPixmapItem;
            item1->setPos(barrier[n]->x(),barrier[n]->y());
            item1->setPixmap(QPixmap(":/barrier/pic/barrier_line.png"));
            this->addItem(item1);

            QGraphicsPixmapItem *item2 = new QGraphicsPixmapItem;
            item2->setPos(barrier[n]->x() + 40,barrier[n]->y() + 20);
            item2->setPixmap(QPixmap(":/barrier/pic/barrier_line.png"));
            this->addItem(item2);

            QGraphicsPixmapItem *item3 = new QGraphicsPixmapItem;
            item3->setPos(barrier[n]->x() - 10,barrier[n]->y() + 40);
            item3->setPixmap(QPixmap(":/barrier/pic/barrier_line.png"));
            this->addItem(item3);

            QGraphicsPixmapItem *item4 = new QGraphicsPixmapItem;
            item4->setPos(barrier[n]->x() + 30,barrier[n]->y() + 60);
            item4->setPixmap(QPixmap(":/barrier/pic/barrier_line.png"));
            this->addItem(item4);
        }
    }
}





//向场景中添加特定水果
void myScene::addNewFruit(int index)
{    
    int pro;

    if(index == 0 || index ==1)   pro = generateRandomNumber(0,2);
    else if (index == 2)    pro = 3;
    else if (index ==3)    pro = 4;
    else   pro = 5;

    Fruit *temp = new Fruit(pro);
    while(ifFoodOverlap(temp)) temp->rePos();
    this->addItem(temp);
    if(food[index] != nullptr) delete food[index];
    food[index] = new Fruit(temp->property,temp->scenePos().toPoint());
}

//特殊水果的更新
void myScene::SpecialFoodUpdate()
{
    for(int i = 0; i < 3 ;++i){
        if(foodExist[i] == 1){
            if(food[i+2] != nullptr)  {delete food[i+2]; food[i+2] = nullptr;}
            else if(generateRandomNumber(0,10) == i)  addNewFruit(i+2);
        }
    }
}




//判断水果是否与蛇/障碍物重合
bool myScene::ifFoodOverlap(Fruit *foodx)
{
    if(foodx == nullptr) return false;

    //是否与蛇1重合
    for (int n = 0;n < head1->Size();++n)
        for(int i = 0;i<3;++i)
            for(int j = 0;j<3;++j)
            {
                if(head1->head() + head1->direct() == foodx->scenePos().toPoint()+QPoint(i*10,j*10)) {
                    return true;
                }
            }

    //是否与蛇2重合
    if(head2 != nullptr){
        for (int n = 0;n < head2->Size();++n)
            for(int i = 0;i<3;++i)
                for(int j = 0;j<3;++j)
                {
                    if(head2->head() + head2->direct() == foodx->scenePos().toPoint()+QPoint(i*10,j*10)) {
                        return true;
                    }
                }
    }

    //是否与障碍物重合
    //障碍物3个为一组记录位置（横：（0，0），（50，-20），（100，-10）；竖：（0，0），（40，20），（-10，40），（30，60）
    //crosswise barrier
        for(int n = 0;n < 3;++n){
            if(barrier[n]!= nullptr){
                for(int x = 0;x < 3;++x)
                    for(int y = 0;y < 3;++y)
                        for(int i = 0;i < 4; ++i)
                            for(int j = i;j < 10-i/2;++j)
                            {
                                if(foodx->scenePos().toPoint() + QPoint(x*10,y*10) == QPoint(barrier[n]->x() + j*10,barrier[n]->y() + 30 - i*10))   return true;
                                if(foodx->scenePos().toPoint() + QPoint(x*10,y*10) == QPoint(barrier[n]->x() + j*10,barrier[n]->y() + 30 - i*10) + QPoint(50,-20))   return true;
                                if(foodx->scenePos().toPoint() + QPoint(x*10,y*10) == QPoint(barrier[n]->x() + j*10,barrier[n]->y() + 30 - i*10) + QPoint(100,-10))   return true;
                            }
            }
        }

        //lengthwise barrier
            for(int n = 3;n < 6;++n){
                if(barrier[n]!= nullptr){
                    for(int x = 0;x < 3;++x)
                        for(int y = 0;y < 3;++y)
                            for(int i = 0;i < 4; ++i)
                                for(int j = i;j < 10-i/2;++j)
                                {
                                    if(foodx->scenePos().toPoint() + QPoint(x*10,y*10) == QPoint(barrier[n]->x() + j*10,barrier[n]->y() + 30 - i*10))   return true;
                                    if(foodx->scenePos().toPoint() + QPoint(x*10,y*10) == QPoint(barrier[n]->x() + j*10,barrier[n]->y() + 30 - i*10) + QPoint(40,20))   return true;
                                    if(foodx->scenePos().toPoint() + QPoint(x*10,y*10) == QPoint(barrier[n]->x() + j*10,barrier[n]->y() + 30 - i*10) + QPoint(-10,40))   return true;
                                    if(foodx->scenePos().toPoint() + QPoint(x*10,y*10) == QPoint(barrier[n]->x() + j*10,barrier[n]->y() + 30 - i*10) + QPoint(30,60))   return true;
                                }
                }
            }

    return false;
}

//判断s蛇是否吃到foodx水果
bool myScene::ifEatFood(Snake *s,Fruit *foodx)
{
    if(foodx == nullptr) return false;

    for(int i = 0;i<3;++i)
        for(int j = 0;j<3;++j)
        {
            if(s->head() + s->direct() == foodx->scenePos().toPoint()+QPoint(i*10,j*10)) {
                return true;
            }
        }

    return false;
}

//判断是否撞到障碍物
bool myScene::ifHitBarrier(Snake *s)
{
    for(int n = 0; n < 6 ; ++n){
        if(barrier[n] == nullptr) break;
        QPoint *barrierPos = new QPoint(barrier[n]->x(),barrier[n]->y());

        //障碍物3个为一组记录位置（横：（0，0），（50，-20），（100，-10）；竖：（0，0），（40，20），（-10，40），（30，60）
        if(n/3 == 0){   //crosswise barrier
            for(int i = 0;i < 4; ++i)
                for(int j = i;j < 10-i/2;++j)
                {
                    if(s->head() + s->direct() == QPoint(barrierPos->x() + j*10,barrierPos->y() + 30 - i*10))   return true;
                    if(s->head() + s->direct() == QPoint(barrierPos->x() + j*10,barrierPos->y() + 30 - i*10) + QPoint(50,-20))   return true;
                    if(s->head() + s->direct() == QPoint(barrierPos->x() + j*10,barrierPos->y() + 30 - i*10) + QPoint(100,-10))  return true;
                }
        }
        else if(n/3 == 1){   //lengthwise barrier
            for(int i = 0;i < 4; ++i)
                for(int j = i;j < 10-i/2;++j)
                {
                    if(s->head() + s->direct() == QPoint(barrierPos->x() + j*10,barrierPos->y() + 30 - i*10))   return true;
                    if(s->head() + s->direct() == QPoint(barrierPos->x() + j*10,barrierPos->y() + 30 - i*10) + QPoint(40,20)) return true;
                    if(s->head() + s->direct() == QPoint(barrierPos->x() + j*10,barrierPos->y() + 30 - i*10) + QPoint(-10,40)) return true;
                    if(s->head() + s->direct() == QPoint(barrierPos->x() + j*10,barrierPos->y() + 30 - i*10) + QPoint(30,60)) return true;
                }
        }
    }

    return false;
}




int myScene::keyPress(Snake *s,QKeyEvent *event)
{
    int temp = 1;

    if(event->key() == s->keyPos[0]){   //up
        temp = judgeAndMove(s,QPoint(0,-10));
    }
    else if (event->key() == s->keyPos[1]) {   //down
        temp = judgeAndMove(s,QPoint(0,10));
    }
    else if (event->key() == s->keyPos[2]) {   //left
        temp = judgeAndMove(s,QPoint(-10,0));
    }
    else if (event->key() == s->keyPos[3]) {   //right
        temp = judgeAndMove(s,QPoint(10,0));
    }

    return temp;

}


int myScene::judgeAndMove(Snake *s,QPoint dire)
{
    //正常前进 0，不作为 1，碰到障碍物 2，碰到自己 3,吃到东西 4

    if(s->snakePos()[s->Size()-1] + dire == s->snakePos()[s->Size()-2])  return 1;


    s->changeDirect(dire);

    //是否死亡（撞墙/撞到障碍物）2
    if(s->head().x()+dire.x() == -10 || s->head().x()+dire.x() == 600 || s->head().y()+dire.y() == -10 || s->head().y()+dire.y() == 510 || ifHitBarrier(s)){
        return 2;
    }

    //是否死亡（碰到自己）3
    if(s->ifHitBody(s->direct())) return 3;

    //是否碰到另一条蛇
    if(head2 != nullptr){
        if(s == head1 && s->ifHitOtherSnake(head2)) return 2;
        if(s == head2 && s->ifHitOtherSnake(head1)) return 2;
    }


    //是否吃到水果，并返回水果属性值  4
    for(int n = 0;n < 2;++n){
        if(ifEatFood(s,food[n])){
            s->addScore(food[n]->value);   //加分

            //增加节点
            for(int i = 1;i < food[n]->value + 1;++i){
                s->addNode(s->head() + dire);
            }

            //改变水果位置
            addNewFruit(n);
            ptimer1->start(8000);
            return 4;
        }
    }

    s->snakeAutoMove();


    //是否吃到egg 1
    if(foodExist[0] == 1 && ifEatFood(s,food[2])){

        //设定可以复活
        s->immortal += 1;

        //改变水果位置
        delete food[2]; food[2] = nullptr;
        return 1;
    }

    //是否吃到dimond 4
    if(foodExist[1] == 1 && ifEatFood(s,food[3])){
        s->addScore(food[3]->value);   //加分

        //改变水果位置
        delete food[3]; food[3] = nullptr;
        return 4;
    }

    //是否吃到bomb 4
    if(foodExist[2] == 1 && ifEatFood(s,food[4])){
        s->addScore(food[4]->value);   //扣分
        s->snakeAutoMove();

        //减少蛇节点，若小于3就dead
        if(s->Size()-5 < 3){
            s->subNode(5);
            return 2;
        }else {
            s->subNode(5);

            //改变水果位置
            delete food[4]; food[4] = nullptr;
            return 4;
        }
    }



    //前进 0
    return 0;

}


QPoint myScene::chooseSide(int n, QPoint hitPoint)
{
    if(n < 3){
        QPoint *mostLeftUp = new QPoint(barrier[n]->x(),barrier[n]->y() - 20);
        if(hitPoint.y() == mostLeftUp->y() || hitPoint.y() == mostLeftUp->y() + 40){   //upline and downline
            if(hitPoint.x() - mostLeftUp->x() > 90) return QPoint(10,0);
            else return QPoint(-10,0);
        }
        else {   //leftline and rightline
            if(hitPoint.y() - mostLeftUp->y() > 20) return  QPoint(0,10);
            else return QPoint(0,-10);
        }
    }
    else {
        QPoint *mostLeftUp = new QPoint(barrier[n]->x() - 10,barrier[n]->y());
        if(hitPoint.y() == mostLeftUp->y() || hitPoint.y() == mostLeftUp->y() + 90){   //upline and downline
            if(hitPoint.x() - mostLeftUp->x() > 70) return QPoint(10,0);
            else return QPoint(-10,0);
        }
        else {   //leftline and rightline
            if(hitPoint.y() - mostLeftUp->y() > 40) return  QPoint(0,10);
            else return QPoint(0,-10);
        }
    }
}


int myScene::AIsnake()
{
    QPoint initial = head1->direct();
    QPoint target;
    int type;

    //锁定目标
    if (food[3] != nullptr)   target = food[3]->pos().toPoint();
    else if(foodExist[3] && food[1]->value > food[0]->value) target = food[1]->pos().toPoint();
    else target = food[0]->pos().toPoint();
    QPoint ds = target - head1->head();
    if(ds.x() != 0) ds.setX(10 * ds.x() / qAbs(ds.x()));
    if(ds.y() != 0) ds.setY(10 * ds.y() / qAbs(ds.y()));

    //运动方向朝向目标
    if(ds.x() != 0 && ds.x() + head1->direct().x()) {
        head1->changeDirect(QPoint(ds.x(),0));
        type = 0;
    }
    else if(ds.x() != 0) {
        if(ds.y()) head1->changeDirect(QPoint(0,ds.y()));
        else head1->changeDirect(QPoint(0,10));
        type = 1;
    }
    else if(ds.y() + head1->direct().y()) {
        head1->changeDirect(ds);
        type = 2;
    }
    else {
        head1->changeDirect(QPoint(10,0));
        type = 3;
    }

    //判断是否碰到障碍物
    bool flag = false;int n = 0;
    int *x = new int[2]{200,160};
    int *y = new int[2]{60,100};
    QPoint *temp = nullptr;
    for(n = 0;n < 6;++n){
        if(barrier[n] == nullptr) continue;

        if(n < 3) temp = new QPoint(barrier[n]->x(),barrier[n]->y()-20);
        else temp = new QPoint(barrier[n]->x()-10,barrier[n]->y());

        if(head1->ifHitRect(temp,x[n/3],y[n/3])){
            flag = true;
            break;
        }
    }
    delete [] x;
    delete [] y;
    delete temp;


    //避开障碍物
    if(flag){
        switch (type) {
        case 0:   //ds.x() != 0 && ds.x() + head1->direct().x()
            if(initial.y()) head1->changeDirect(initial);
            else if(ds.y()) head1->changeDirect(QPoint(0,ds.y()));
            else head1->changeDirect(chooseSide(n,head1->head()+head1->direct()));
            break;
        case 1:   //ds.x() != 0
            if(ds.y()) head1->changeDirect(initial);
            else head1->changeDirect(chooseSide(n,head1->head()+head1->direct()));
            break;
        case 2:   //ds.y() + head1->direct().y()
            if(initial.x()) head1->changeDirect(initial);
            else head1->changeDirect(chooseSide(n,head1->head()+head1->direct()));
            break;
        case 3:
            head1->changeDirect(chooseSide(n,head1->head()+head1->direct()));
        }
    }

    //避开其他蛇
    if(head2 != nullptr && head1->ifHitOtherSnake(head2)){
        if(head1->direct().x()){
            if((head1->head() - head1->snakeNode[head1->currentSize -2]).x() == 0)
                head1->changeDirect(head1->head() - head1->snakeNode[head1->currentSize -2]);
            else {
                if(ds.y()) head1->changeDirect(QPoint(0,ds.y()));
                else head1->changeDirect(QPoint(0,10));
            }
        }
        else {
            if((head1->head() - head1->snakeNode[head1->currentSize -2]).y() == 0)
                head1->changeDirect(head1->head() - head1->snakeNode[head1->currentSize -2]);
            else {
                if(ds.x()) head1->changeDirect(QPoint(ds.x(),0));
                else head1->changeDirect(QPoint(10,0));
            }
        }
    }


    //避免碰到自己其他节点
    int a = 0;
    while(a++ <2)
    {
        if(head1->ifHitBody(head1->direct())){
            if(head1->direct().x()){
                int i = 1;
                while (head1->head().x() != head1->snakeNode[head1->currentSize-(++i)].x()) ;
                QPoint temp = head1->head() - head1->snakeNode[head1->currentSize-i];
                if(temp.y() != 0) temp.setY(10 * temp.y() / qAbs(temp.y()));
                head1->changeDirect(temp);
            }
            else {
                int i = 1;
                while (head1->head().y() != head1->snakeNode[head1->currentSize-(++i)].y()) ;
                QPoint temp = head1->head() - head1->snakeNode[head1->currentSize-i];
                if(temp.x() != 0) temp.setX(10 * temp.x() / qAbs(temp.x()));
                head1->changeDirect(temp);
            }
        }
        if(head1->ifHitBody(head1->head()-head1->snakeNode[head1->currentSize-2])){
            if(QPoint(head1->head()-head1->snakeNode[head1->currentSize-2]).x()){
                int i = 1;
                while (head1->head().x() != head1->snakeNode[head1->currentSize-(++i)].x()) ;
                QPoint temp = head1->head() - head1->snakeNode[head1->currentSize-i];
                if(temp.y() != 0) temp.setY(10 * temp.y() / qAbs(temp.y()));
                head1->changeDirect(temp);
            }
            else {
                int i = 1;
                while (head1->head().y() != head1->snakeNode[head1->currentSize-(++i)].y()) ;
                QPoint temp = head1->head() - head1->snakeNode[head1->currentSize-i];
                if(temp.x() != 0) temp.setX(10 * temp.x() / qAbs(temp.x()));
                head1->changeDirect(temp);
            }
        }
    }

    int tmp = judgeAndMove(head1,head1->direct());
    return tmp;
}





void myScene::setMode()
{
    /* mode(0/1/2/3)
     * color1
     * (color2)
     * fruit
     * barrier
     */


    QFile file("mode.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString text = in.readLine();

    if(text == "single") {
        head1->keyPos[0] = Qt::Key_Up;
        head1->keyPos[1] = Qt::Key_Down;
        head1->keyPos[2] = Qt::Key_Left;
        head1->keyPos[3] = Qt::Key_Right;
    }
    else if(text == "double"){
        head1->keyPos[0] = Qt::Key_Up;
        head1->keyPos[1] = Qt::Key_Down;
        head1->keyPos[2] = Qt::Key_Left;
        head1->keyPos[3] = Qt::Key_Right;

        head2 = new Snake(Qt::Key_W,Qt::Key_S,Qt::Key_A,Qt::Key_D);
        head2->changeColor(QColor(120,180,120));
        addSnake(head2);

    }
    else if(text == "AI"){
        AI = true;
    }
    else if(text == "AgainstAI"){
        AI = true;
        head2 = new Snake(Qt::Key_Up,Qt::Key_Down,Qt::Key_Left,Qt::Key_Right);
        head2->changeColor(QColor(120,180,120));
        addSnake(head2);
    }
    else if (text == "0") {
        head1->keyPos[0] = Qt::Key_Up;
        head1->keyPos[1] = Qt::Key_Down;
        head1->keyPos[2] = Qt::Key_Left;
        head1->keyPos[3] = Qt::Key_Right;

        int *temp = new int[3];

        //color
        for(int i = 0; i < 3 ; ++i){
            in >> temp[i];
        }
        head1->changeColor(QColor(temp[0],temp[1],temp[2]));
        addSnake(head1);

        delete [] temp;
    }
    else if (text == "1")
    {
        head1->keyPos[0] = Qt::Key_Up;
        head1->keyPos[1] = Qt::Key_Down;
        head1->keyPos[2] = Qt::Key_Left;
        head1->keyPos[3] = Qt::Key_Right;

        head2 = new Snake(Qt::Key_W,Qt::Key_S,Qt::Key_A,Qt::Key_D);

        int *temp = new int[3];

        //color
        for(int i = 0; i < 3 ; ++i){
            in >> temp[i];
        }
        head1->changeColor(QColor(temp[0],temp[1],temp[2]));
        addSnake(head1);
        for(int i = 0; i < 3 ; ++i){
            in >> temp[i];
        }
        head2->changeColor(QColor(temp[0],temp[1],temp[2]));
        addSnake(head2);

        delete [] temp;

    }
    else if (text == "2") {
        AI = true;

        int *temp = new int[3];

        //color
        for(int i = 0; i < 3 ; ++i){
            in >> temp[i];
        }
        head1->changeColor(QColor(temp[0],temp[1],temp[2]));
        addSnake(head1);

        delete [] temp;
    }
    else if (text == "3") {
        AI = true;

        head2 = new Snake(Qt::Key_Up,Qt::Key_Down,Qt::Key_Left,Qt::Key_Right);

        int *temp = new int[3];

        //color
        for(int i = 0; i < 3 ; ++i){
            in >> temp[i];
        }
        head1->changeColor(QColor(temp[0],temp[1],temp[2]));
        addSnake(head1);
        for(int i = 0; i < 3 ; ++i){
            in >> temp[i];
        }
        head2->changeColor(QColor(temp[0],temp[1],temp[2]));
        addSnake(head2);

        delete [] temp;
    }


    int *temp = new int[4];

    //special fruit
    for(int i = 0; i < 4 ; ++i){
        in >> temp[i];
        foodExist[i] = temp[i];
    }
    if(foodExist[0] || foodExist[1] || foodExist[2]) {
        ptimer2 = new QTimer;
        connect(this->ptimer2,SIGNAL(timeout()),this,SLOT(SpecialFoodUpdate()));
    }

    //barrier (100*40)
    //障碍物3个为一组记录位置（横：（0，0），（50，-20），（100，-10）；竖：（0，0），（40，20），（-10，40），（30，60）
        in >> temp[1] >>temp[2];
        if(temp[1]){   //crosswise
            barrier[0] = new QPoint(generateRandomNumber(15,25)*10,generateRandomNumber(10,15)*10);
            barrier[1] = new QPoint(generateRandomNumber(35,40)*10,generateRandomNumber(20,30)*10);
            barrier[2] = new QPoint(generateRandomNumber(15,25)*10,generateRandomNumber(35,40)*10);
        }
        if(temp[2]){   //lengthwise
            barrier[3] = new QPoint(generateRandomNumber(2,10)*10,generateRandomNumber(2,8)*10);
            barrier[4] = new QPoint(generateRandomNumber(15,25)*10,generateRandomNumber(20,30)*10);
            barrier[5] = new QPoint(generateRandomNumber(35,40)*10,generateRandomNumber(35,40)*10);
        }
    if(temp[2] || temp[1])   addBarrier();

    if(temp[3]) addNewFruit(1);
    addNewFruit(0);
    addFruit();
    delete [] temp;

    file.close();


}





/*    //画方格
    QGraphicsRectItem *temp = new QGraphicsRectItem;
    temp->setRect(100,100,20,20);
    temp->setBrush(QBrush(QColor(200,100,100)));
    this->addItem(temp);

    //画水果
    QGraphicsPixmapItem *item2 = new QGraphicsPixmapItem;
    item2->setPixmap(QPixmap(":/fruit/pic/strawberry.png/"));
    this->addItem(item2);
*/


