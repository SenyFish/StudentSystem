#include "mainwindow.h"
#include "DesignSystem.h"
#include "TransparentMask.h"

#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    qDebug() << "程序启动...";
    
    // 加载 Noto Sans SC 字体（失败也继续运行）
    int fontId = QFontDatabase::addApplicationFont(":/fonts/NotoSansSC-Regular.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family);
        font.setPixelSize(14);
        font.setHintingPreference(QFont::PreferNoHinting);
        QApplication::setFont(font);
        qDebug() << "字体加载成功:" << family;
    } else {
        qWarning() << "字体加载失败，使用系统默认字体";
    }
    
    // 初始化设计系统
    qDebug() << "初始化设计系统...";
    DesignSystem::instance();
    
    qDebug() << "创建主窗口...";
    MainWindow w;
    
    // 创建全局透明遮罩（在窗口创建后）
    qDebug() << "创建透明遮罩...";
    TransparentMask* tpMask = new TransparentMask(&w);
    DesignSystem::instance()->setTransparentMask(tpMask);
    
    qDebug() << "显示窗口...";
    w.show();
    
    qDebug() << "进入事件循环";
    return a.exec();
}
