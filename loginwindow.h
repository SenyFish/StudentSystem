#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "ElaWindow.h"
#include <QPropertyAnimation>
#include <QTimer>
#include <QLabel>

class ElaLineEdit;
class ElaPushButton;
class CardWidget;

class LoginWindow : public ElaWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginClicked(const QString &username, const QString &password);

private slots:
    void onLoginButtonClicked();

private:
    void setupUI();
    void showLoginError(const QString &message);

    // UI 组件
    CardWidget *m_loginCard;
    QLabel *m_logoLabel;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    ElaLineEdit *m_usernameInput;
    ElaLineEdit *m_passwordInput;
    ElaPushButton *m_loginButton;
};

#endif // LOGINWINDOW_H

