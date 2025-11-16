#include "mainwindow.h"
#include "loginwindow.h"
#include "ElaApplication.h"

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QTimer>
#include <QLabel>
#include <QCoreApplication>
#include <QThread>
#include <QEventLoop>

int main(int argc, char *argv[])
{
    qDebug() << "程序启动";
    
    QApplication a(argc, argv);
    
    // 设置应用程序图标
    a.setWindowIcon(QIcon(":/icon.ico"));
    
    // 设置应用程序在最后一个窗口关闭时不自动退出
    // 这样登录窗口关闭时程序不会退出
    a.setQuitOnLastWindowClosed(false);
    
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
    
    qDebug() << "创建主窗口（隐藏状态）";
    
    // 创建主窗口（不显示，避免闪烁）
    MainWindow *mainWindow = new MainWindow();
    mainWindow->setWindowTitle("学生信息管理系统");
    
    qDebug() << "创建独立的登录窗口";
    
    // 创建独立的登录窗口
    LoginWindow *loginWindow = new LoginWindow();
    loginWindow->setWindowTitle("学生信息管理系统 - 登录");
    loginWindow->resize(1200, 800);
    
    // 显示提示信息
    QTimer::singleShot(800, [loginWindow]() {
        if (loginWindow && loginWindow->isVisible()) {
            QLabel *hintLabel = new QLabel("默认用户名和密码：root", loginWindow);
            hintLabel->setStyleSheet(R"(
                QLabel {
                    color: rgb(100, 120, 180);
                    background-color: rgba(200, 220, 255, 150);
                    border-radius: 5px;
                    padding: 8px 15px;
                    font-size: 13px;
                }
            )");
            hintLabel->adjustSize();
            int x = (loginWindow->width() - hintLabel->width()) / 2;
            int y = loginWindow->height() - 100;
            hintLabel->move(x, y);
            hintLabel->show();
            
            // 5秒后消失
            QTimer::singleShot(5000, hintLabel, &QLabel::deleteLater);
        }
    });
    
    // 连接登录成功信号
    QObject::connect(loginWindow, &LoginWindow::loginClicked, [mainWindow, loginWindow, &a](const QString &username, const QString &password) {
        qDebug() << "登录尝试 - 用户名:" << username;
        
        // 验证用户名和密码（默认都是 root）
        if (username == "root" && password == "root") {
            qDebug() << "登录成功，准备显示主窗口";
            
            // 更新主窗口标题
            mainWindow->setWindowTitle("学生信息管理系统 - " + username);
            
            // 先关闭登录窗口
            loginWindow->close();
            
            // 先关闭登录窗口动画，立即显示主窗口（避免间隙）
            loginWindow->hide();
            
            // 临时禁用主窗口的拖动功能，防止在初始化时拖动导致卡死
            mainWindow->setEnabled(false);
            
            // 显示主窗口
            mainWindow->show();
            
            // 立即处理所有待处理事件，确保窗口完全渲染
            for (int i = 0; i < 10; ++i) {
                QCoreApplication::processEvents(QEventLoop::AllEvents);
                QThread::msleep(20); // 每次间隔20ms，总共200ms
            }
            
            qDebug() << "主窗口已显示";
            
            // 等待窗口布局完全完成后再启用交互（防止拖动卡死）
            QTimer::singleShot(500, [mainWindow]() {
                mainWindow->setEnabled(true);
                qDebug() << "主窗口交互已启用";
            });
            
            // 延迟删除登录窗口和恢复退出设置
            QTimer::singleShot(600, [loginWindow, &a]() {
                loginWindow->deleteLater();
                // 恢复默认行为：当最后一个窗口关闭时退出程序
                a.setQuitOnLastWindowClosed(true);
                qDebug() << "清理完成，窗口已完全初始化";
            });
        } else {
            qDebug() << "登录失败 - 用户名或密码错误";
        }
    });
    
    // 显示登录窗口
    qDebug() << "显示登录窗口";
    loginWindow->show();
    
    qDebug() << "进入事件循环";
    
    return a.exec();
}
