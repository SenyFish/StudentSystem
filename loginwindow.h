#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "ElaWindow.h"
#include "user.h"
#include <QPropertyAnimation>
#include <QTimer>
#include <QLabel>

class ElaLineEdit;
class ElaPushButton;
class ElaComboBox;
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
    void onRegisterButtonClicked();
    void switchToRegister();
    void switchToLogin();
    void onAccountInfoButtonClicked();  // 账号说明按钮点击事件

private:
    void setupUI();
    void showLoginError(const QString &message);
    void showRegisterError(const QString &message);

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
    ElaPushButton *m_toRegisterButton;
    ElaPushButton *m_accountInfoButton;  // 账号说明按钮
    
    // 学生信息查询（用于自动创建账户）
    MainWindow* m_mainWindow;  // 用于访问学生列表
    
    // 注册界面
    QWidget *m_registerWidget;
    ElaLineEdit *m_regUsernameInput;
    ElaLineEdit *m_regPasswordInput;
    ElaLineEdit *m_regPasswordConfirmInput;
    ElaLineEdit *m_regRealNameInput;
    ElaComboBox *m_regRoleComboBox;
    ElaLineEdit *m_regStudentIdInput;  // 学号输入框（仅学生角色显示）
    QLabel *m_regStudentIdLabel;  // 学号标签
    ElaComboBox *m_regMajorComboBox;  // 专业选择框（仅教师角色显示）
    QLabel *m_regMajorLabel;  // 专业标签
    ElaPushButton *m_registerButton;
    ElaPushButton *m_toLoginButton;
};

#endif // LOGINWINDOW_H

