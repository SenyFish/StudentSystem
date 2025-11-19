#include "mainwindow.h"
#include "loginwindow.h"
#include "usermanager.h"
#include "ElaApplication.h"

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QTimer>
#include <QLabel>
#include <QPointer>

int main(int argc, char *argv[])
{
    qDebug() << "程序启动";
    
    // 在 QApplication 创建前设置 DPI 缩放属性（最重要的修复）
    // 这会让 Qt 自动处理高 DPI 屏幕的缩放
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
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
    
    // 设置主窗口引用，用于自动创建学生账户
    loginWindow->setMainWindow(mainWindow);
    
    
    // 连接登录成功信号
    QObject::connect(loginWindow, &LoginWindow::loginSuccess, [mainWindow, loginWindow, &a](const QString &username) {
        qDebug() << "登录成功信号收到，用户名:" << username;
        
        // 从UserManager获取用户信息
        User user = UserManager::getInstance().getCurrentUser();
        UserRole userRole = user.getRole();
        QString realName = user.getRealName();
        QString roleString = user.getRoleString();
        QString studentId = user.getStudentId();  // 获取关联的学生ID
        QString major = user.getMajor();  // 获取关联的专业
        
        qDebug() << "用户信息获取完成:" << realName << roleString << "学生ID:" << studentId << "专业:" << major;
        
        // 更新主窗口标题
        if (mainWindow) {
            QString title = QString("学生信息管理系统 - %1（%2）").arg(realName).arg(roleString);
            if (userRole == UserRole::Student && !studentId.isEmpty()) {
                title += QString(" - 学号：%1").arg(studentId);
            } else if (userRole == UserRole::Teacher && !major.isEmpty()) {
                title += QString(" - 专业：%1").arg(major);
            }
            mainWindow->setWindowTitle(title);
        }
        
        // 先隐藏登录窗口（不关闭，避免程序退出）
        if (loginWindow) {
            loginWindow->hide();
        }
        
        qDebug() << "准备显示主窗口";
        
        // 使用指针来安全管理生命周期
        QPointer<MainWindow> mainWindowPtr(mainWindow);
        QPointer<LoginWindow> loginWindowPtr(loginWindow);
        
        // 第一步：设置用户权限
        QTimer::singleShot(50, [mainWindowPtr, loginWindowPtr, userRole, studentId, major]() {
            if (!mainWindowPtr) return;
            
            qDebug() << "开始预加载主窗口 - 设置权限";
            
            // 先设置用户权限（在显示前），传递学生ID和专业
            mainWindowPtr->setUserRole(userRole, studentId, major);
            
            // 第二步：等待布局计算完成
            QTimer::singleShot(300, [mainWindowPtr, loginWindowPtr]() {
                if (!mainWindowPtr) return;
                
                qDebug() << "预加载 - 强制更新布局";
                
                // 强制更新布局（窗口隐藏时）
                mainWindowPtr->update();
                mainWindowPtr->updateGeometry();
                mainWindowPtr->adjustSize();
                
                // 第三步：再等待一段时间确保布局稳定
                QTimer::singleShot(200, [mainWindowPtr, loginWindowPtr]() {
                    if (!mainWindowPtr) return;
                    
                    qDebug() << "主窗口预加载完成，准备显示";
                    
                    // 现在显示窗口（此时所有布局已经计算完成）
                    mainWindowPtr->show();
                    
                    // 主窗口已显示，现在可以安全关闭登录窗口
                    if (loginWindowPtr) {
                        loginWindowPtr->close();
                    }
                    
                    qDebug() << "主窗口已显示，等待展开动画完成";
                    
                    // 第四步：等待ElaWindow的展开动画完成
                    QTimer::singleShot(1000, [mainWindowPtr]() {
                        if (!mainWindowPtr) return;
                        
                        if (!mainWindowPtr->isVisible()) {
                            qDebug() << "主窗口不可见";
                            return;
                        }
                        
                        qDebug() << "展开动画完成，启用窗口交互";
                        
                        // 启用窗口交互
                        mainWindowPtr->setEnabled(true);
                        
                        qDebug() << "主窗口完全就绪，可以安全交互";
                    });
                });
            });
        });
        
        // 清理登录窗口和恢复退出设置（在主窗口完全就绪后）
        QTimer::singleShot(1700, [loginWindowPtr, &a]() {
            if (loginWindowPtr) {
                loginWindowPtr->deleteLater();
            }
            // 主窗口已显示，现在可以恢复默认退出行为
            a.setQuitOnLastWindowClosed(true);
            qDebug() << "登录窗口已清理，退出设置已恢复";
        });
    });
    
    // 显示登录窗口
    qDebug() << "显示登录窗口";
    loginWindow->show();
    
    qDebug() << "进入事件循环";
    
    return a.exec();
}
