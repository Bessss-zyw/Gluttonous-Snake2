#include "interface.h"
#include "ui_interface.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QtDebug>


Interface::Interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Interface)
{
    ui->setupUi(this);

    sc = new QGraphicsScene;
    QGraphicsPixmapItem *item1 = new QGraphicsPixmapItem;
    item1->setPixmap(QPixmap(":/bg/pic/background.jpg"));
    this->sc->addItem(item1);
    this->ui->graphicsView->setBackgroundBrush(QBrush(QColor(130,100,90)));
    this->ui->graphicsView->setScene(sc);


    connect(this->ui->btn_single,SIGNAL(clicked()),this,SLOT(PlaySingle()));
    connect(this->ui->btn_double,SIGNAL(clicked()),this,SLOT(PlayDouble()));
    connect(this->ui->btn_AI,SIGNAL(clicked()),this,SLOT(AI()));
    connect(this->ui->btn_againstAI,SIGNAL(clicked()),this,SLOT(AgainstAI()));
    connect(this->ui->btn_more,SIGNAL(clicked()),this,SLOT(more()));

}

Interface::~Interface()
{
    delete ui;
}



void Interface::writeToFile(QString cnt)
{
    QFile file("mode.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this,"错误","文件打开失败","确定");
        return;
    }

    QTextStream out(&file);
    out << cnt;
    file.close();
}


/* mode ("single","double","AI","againstAI","more")
 * snake color （int r int g int b) (int r int g int b)
 * fruit (egg 0/1 dimond 0/1 bomb 0/1)
 * barrier 0/1
 */



void Interface::PlaySingle()
{
    writeToFile("single");
    w = new MainWindow;
    w->show();
}

void Interface::PlayDouble()
{
    QString cnt = "double";
    writeToFile(cnt);

    w = new MainWindow;
    w->show();
}

void Interface::AI()
{
    writeToFile("AI");
    w = new MainWindow;
    w->show();
}

void Interface::AgainstAI()
{
    writeToFile("AgainstAI");
    w = new MainWindow;
    w->show();
}

void Interface::more()
{
    s = new Setting;
    s->show();
}




