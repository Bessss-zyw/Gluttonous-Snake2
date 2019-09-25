#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBrush>
#include <QColor>
#include <QPalette>
#include <QtDebug>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ptimer0 = new QTimer;
    this->sc = new myScene;
//    this->sc->doubelSnake(Qt::Key_W,Qt::Key_S,Qt::Key_A,Qt::Key_D);

    if(sc->head2 == nullptr){
        this->ui->lcdNumber_2->hide();
        this->ui->label_2->hide();
    }
    else {
        this->ui->label_1->setText("玩家1：");
    }

    this->ui->graphicsView->setBackgroundBrush(QBrush(QColor(130,100,90)));
    this->ui->graphicsView->setScene(sc);

/*    QGraphicsPixmapItem *item2 = new QGraphicsPixmapItem;
    item2->setPos(50,50);
    item2->setPixmap(QPixmap(":/barrier/pic/barrier_row1.png"));
    this->sc->addItem(item2);
*/

    setFocusPolicy(Qt::StrongFocus);
    connect(this->ui->btn_start,SIGNAL(clicked()),this,SLOT(btn_start()));
    connect(this->ui->btn_pause,SIGNAL(clicked()),this,SLOT(btn_pause()));
    connect(this->ui->btn_end,SIGNAL(clicked()),this,SLOT(btn_end()));
    connect(this->ptimer0,SIGNAL(timeout()),this,SLOT(SceneAutoUpdate()));
    connect(this->sc->ptimer1,SIGNAL(timeout()),this,SLOT(FruitAutoUpdate()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete ptimer0;
}


void MainWindow::btn_start()
{
    this->ptimer0->start(250);
    this->sc->ptimer1->start(8000);
    if(sc->ptimer2 != nullptr)
    {
        this->sc->ptimer2->start(5000);
    }
}

void MainWindow::btn_pause()
{
    this->ptimer0->stop();
    this->sc->ptimer1->stop();
    if(sc->ptimer2 != nullptr)    this->sc->ptimer2->stop();

    this->ui->btn_start->setText("restart");
    this->setFocus();
}

void MainWindow::btn_end()
{
    this->ptimer0->stop();
    this->sc->ptimer1->stop();
    if(sc->ptimer2 != nullptr)    this->sc->ptimer2->stop();
    this->close();
}



void MainWindow::GameOver()
{
    if( sc->head1->immortal > 0) {
        sc->head1->revival();
        return;
    }
    else if(sc->head2 != nullptr && sc->head2->immortal > 0){
        sc->head2->revival();
        return;
    }

    this->ptimer0->stop();
    this->sc->ptimer1->stop();
    if(sc->ptimer2 != nullptr) this->sc->ptimer2->stop();
//    this->setEnabled(false);

    QString temp;
    if(sc->head2 == nullptr){
        temp += "分数:  " + QString::number(this->sc->head1->Score());
    }
    else {
        temp += "玩家一:  " + QString::number(this->sc->head1->Score());
        temp += "\n玩家二:  " + QString::number(this->sc->head2->Score());
    }
    temp += "\n\n想要继续游戏吗?";

    QMessageBox msgBox;
    msgBox.setText("游戏结束!");
    msgBox.setInformativeText(temp);
    msgBox.addButton("再来一局",QMessageBox::AcceptRole);
    msgBox.addButton("去意已决",QMessageBox::RejectRole);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::AcceptRole:
          delete sc;
          sc = new myScene;
          this->ui->graphicsView->setScene(sc);
          this->ui->lcdNumber_1->display(0);
          this->ui->lcdNumber_2->display(0);
          this->ui->btn_start->setText("start");
          break;
      case QMessageBox::RejectRole:
          this->close();
          break;
      default:
          // should never be reached
          break;
    }
}

void MainWindow::SceneUpdate()
{
    this->sc->clear();
    this->sc->addBG();
    this->sc->addBarrier();
    this->sc->addSnake(sc->head1);
    if(sc->head2 != nullptr) this->sc->addSnake(sc->head2);
    this->sc->addFruit();
}

void MainWindow::FruitAutoUpdate()
{
    this->sc->addNewFruit(0);
    if(sc->foodExist[3]) this->sc->addNewFruit(1);
}



void MainWindow::Rule(int snake,int temp)
{
    switch (temp) {
    case 2:case 3:   //dead
        GameOver();
        return;
    case 4:   //eat
        if(snake == 1)  this->ui->lcdNumber_1->display(sc->head1->Score());
        else  this->ui->lcdNumber_2->display(sc->head2->Score());
    }
    SceneUpdate();
}



void MainWindow::SceneAutoUpdate()
{
    //正常前进 0，不作为 1，碰到障碍物 2，碰到自己 3,吃到东西 4

//
    if(sc->AI){
        int temp1 = sc->AIsnake();
        temp1 = sc->AIsnake();
        Rule(1,temp1);
    }
    else {
        int temp1 = sc->judgeAndMove(sc->head1,sc->head1->direct());
        Rule(1,temp1);
    }

    if(sc->head2 != nullptr){
        int temp2 = sc->judgeAndMove(sc->head2,sc->head2->direct());
        Rule(2,temp2);
    }
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //正常前进 0，不作为 1，碰到障碍物 2，碰到自己 3,吃到东西 4

    int temp1 = sc->keyPress(sc->head1,event);
    Rule(1,temp1);

    if(sc->head2 != nullptr){
        int temp2 = sc->keyPress(sc->head2,event);
        Rule(2,temp2);
    }
}




