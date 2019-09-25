#include "mainwindow.h"
#include "interface.h"
#include "setting.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Interface w;
    w.show();

    return a.exec();
}




/* AI蛇
 * 选择水果离自己最近的一个点
 * 其他蛇
 * * * 学会吃鸡蛋
 */







//点击按钮pause后键盘无法控制
//高阶游戏：障碍物/水果实时移动
//蛇头用图片代替
//设置游戏时间
