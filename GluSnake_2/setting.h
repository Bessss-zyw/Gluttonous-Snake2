#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QString>

#include "mainwindow.h"

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

    void record();

private:
    void WriteOnly(int cnt);
    void AppendSpace(int cnt);
    void AppendReturn(int cnt);

private:
    Ui::Setting *ui;
    MainWindow *w;

public slots:
    void btn_ok_clicked();
    void btn_cancel_cliked();

};




#endif // SETTING_H
