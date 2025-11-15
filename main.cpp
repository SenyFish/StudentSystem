#include "mainwindow.h"
#include "ElaApplication.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "程序启动";
    
    QApplication a(argc, argv);
    
    qDebug() << "QApplication 创建成功";
    
    // 初始化 ElaApplication
    qDebug() << "开始初始化 ElaApplication";
    ElaApplication::getInstance()->init();
    qDebug() << "ElaApplication 初始化完成";
    
    // 启用亚克力效果（Windows 11+ 支持 Acrylic, Windows 11 支持 Mica）
#ifdef Q_OS_WIN
    ElaApplication::getInstance()->setWindowDisplayMode(ElaApplicationType::Acrylic);
    qDebug() << "已启用 Acrylic 亚克力效果";
#endif
    
    qDebug() << "开始创建 MainWindow";
    MainWindow w;
    qDebug() << "MainWindow 创建完成";
    
    qDebug() << "显示窗口";
    w.show();
    qDebug() << "窗口已显示，进入事件循环";
    
    return a.exec();
}
