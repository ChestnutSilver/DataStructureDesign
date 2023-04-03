#include "mainwindow.h"
#include <QApplication>
#include "logindlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
// 建立自己新建的LoginDlg类的实例dlg
    LoginDlg dlg;
// 利用Accepted返回值判断按钮是否被按下
    if(dlg.exec() == QDialog::Accepted)
    {
// 如果被按下，显示主窗口
        w.show();
// 程序一直执行，直到主窗口关闭
        return a.exec();
    }
//如果没有被按下，整个程序结束运行
    else return 0;
}
