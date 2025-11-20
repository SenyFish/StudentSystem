#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "ElaWindow.h"
#include "user.h"
#include <QPropertyAnimation>
#include <QTimer>
#include <QLabel>

class ElaLineEdit;
class ElaPushButton;
class CardWidget;
class MainWindow;

class LoginWindow : public ElaWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    
    void setMainWindow(MainWindow* mainWindow) { m_mainWindow = mainWindow; }  // 设置主窗口引用

signals:
    void loginSuccess(const QString &username);
    void needStudentInfo(const QString &studentId);  // 需要学生信息来创建账户

private slots:
    void onLoginButtonClicked();
    void onAccountInfoButtonClicked();  // 账号说明按钮点击事件

private:
    void setupUI();
    void showLoginError(const QString &message);

    // UI 组件
    CardWidget *m_loginCard;
    QLabel *m_logoLabel;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    
    // 登录界面
    QWidget *m_loginWidget;
    ElaLineEdit *m_usernameInput;  // 现在用于输入学号或用户名
    ElaLineEdit *m_passwordInput;
    ElaPushButton *m_loginButton;
    ElaPushButton *m_accountInfoButton;  // 账号说明按钮
    
    // 学生信息查询（用于自动创建账户）
    MainWindow* m_mainWindow;  // 用于访问学生列表
};

#endif // LOGINWINDOW_H

