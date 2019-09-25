#include "setting.h"
#include "ui_setting.h"

#include <QMessageBox>
#include <QtDebug>
#include <QDialogButtonBox>
#include <QFile>



Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);

    connect(this->ui->btn_ok,SIGNAL(clicked()), this,SLOT(btn_ok_clicked()));
    connect(this->ui->btn_cancel,SIGNAL(clicked()), this,SLOT(btn_cancel_cliked()));
}

Setting::~Setting()
{
    delete ui;
}

void Setting::btn_cancel_cliked()
{
    this->close();
}

void Setting::btn_ok_clicked()
{
    qDebug() << "btn_ok" << endl;

    QMessageBox msgBox;
    msgBox.setText("请确认信息：");
    msgBox.setInformativeText("");
    msgBox.addButton("开始游戏",QMessageBox::AcceptRole);
    msgBox.addButton("返回",QMessageBox::RejectRole);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::AcceptRole:
          record();

          w = new MainWindow;
          w->show();

          this->close();
          return;
      case QMessageBox::RejectRole:
          return;
      default:
          // should never be reached
          break;
    }
}


void Setting::WriteOnly(int cnt)
{
    QFile file("mode.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this,"错误","文件打开失败","确定");
        return;
    }

    QTextStream out(&file);
    out << cnt << endl;
    file.close();
}

void Setting::AppendSpace(int cnt)
{
    QFile file("mode.txt");
    if(!file.open(QIODevice::Append | QIODevice::Text))
    {
        QMessageBox::critical(this,"错误","文件打开失败","确定");
        return;
    }

    QTextStream out(&file);
    out << cnt << " ";
    file.close();
}

void Setting::AppendReturn(int cnt)
{
    QFile file("mode.txt");
    if(!file.open(QIODevice::Append | QIODevice::Text))
    {
        QMessageBox::critical(this,"错误","文件打开失败","确定");
        return;
    }

    QTextStream out(&file);
    out << cnt << '\n';
    file.close();
}


void Setting::record()
{
    /* mode(0/1/2/3)
     * color1
     * (color2)
     * fruit
     * barrier
     */


    //mode
    int mode = this->ui->comboBox_mode->currentIndex();
    WriteOnly(mode);

    //color
    int *snake1 = new int[3];
    snake1[0] = this->ui->snake_1_r->text().toInt();AppendSpace(snake1[0]);
    snake1[1] = this->ui->snake_1_g->text().toInt();AppendSpace(snake1[1]);
    snake1[2] = this->ui->snake_1_b->text().toInt();AppendReturn(snake1[2]);
    delete [] snake1;
    if(mode == 1 || mode == 3){
        int *snake2 = new int[3];
        snake2[0] = this->ui->snake_2_r->text().toInt();AppendSpace(snake2[0]);
        snake2[1] = this->ui->snake_2_g->text().toInt();AppendSpace(snake2[1]);
        snake2[2] = this->ui->snake_2_b->text().toInt();AppendReturn(snake2[2]);
        delete [] snake2;
    }


    //fruit
    bool *fruit = new bool[4];
    fruit[0] = this->ui->cBox_egg->isChecked(); AppendSpace(fruit[0]);
    fruit[1] = this->ui->cBox_dimond->isChecked(); AppendSpace(fruit[1]);
    fruit[2] = this->ui->cBox_bomb->isChecked(); AppendSpace(fruit[2]);
    fruit[3] = this->ui->cBox_moreFruit->isChecked();AppendReturn(fruit[3]);
    delete [] fruit;

    //barrier
    bool barrier = this->ui->cBox_Barrier_cross->isChecked();
    AppendSpace(barrier);
    barrier = this->ui->cBox_Barrier_length->isChecked();
    AppendSpace(barrier);

/*    QFile file("mode.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    QString text = in.readLine();
    qDebug() << text ;

    while (!in.atEnd()) {
        int i;
        in >> i;
        qDebug() << i ;
    }
*/
}








