#include <QApplication>
#include <QDebug>
#include "ElaApplication.h"
#include "ElaWindow.h"
#include "ElaText.h"
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 初始化 Ela
    ElaApplication::getInstance()->init();
    
    // 创建窗口
    ElaWindow* window = new ElaWindow();
    window->setWindowTitle("测试窗口");
    window->resize(800, 600);
    
    // 创建简单页面
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    ElaText* text = new ElaText("Hello ElaWidgetTools!");
    text->setTextPixelSize(24);
    layout->addWidget(text);
    
    // 添加页面
    window->addPageNode("首页", page, 0);
    
    window->show();
    
    return a.exec();
}

