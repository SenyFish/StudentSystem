#include "loginwindow.h"
#include "cardwidget.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaText.h"
#include "ElaMessageBar.h"
#include "ElaApplication.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QIcon>
#include <QPixmap>

LoginWindow::LoginWindow(QWidget *parent)
    : ElaWindow(parent)
    , m_loginCard(nullptr)
    , m_logoLabel(nullptr)
    , m_titleLabel(nullptr)
    , m_subtitleLabel(nullptr)
    , m_usernameInput(nullptr)
    , m_passwordInput(nullptr)
    , m_loginButton(nullptr)
{
    // 配置 ElaWindow
    setWindowTitle("学生信息管理系统 - 登录");
    setWindowIcon(QIcon(":/icon.ico"));
    resize(900, 650);
    setMinimumSize(800, 600);
    
    // 禁用导航栏（登录界面不需要导航）
    setIsNavigationBarEnable(false);
    
    // 隐藏用户信息卡
    setUserInfoCardVisible(false);
    
    // 设置窗口居中
    setWindowModality(Qt::NonModal);
    
    setupUI();
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::setupUI()
{
    // 创建中心容器
    QWidget *centralWidget = new QWidget(this);
    
    // 主布局 - 垂直居中
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    mainLayout->setSpacing(0);
    
    // 上方弹簧
    mainLayout->addStretch(1);
    
    // 创建登录卡片
    m_loginCard = new CardWidget(centralWidget);
    m_loginCard->setFixedSize(550, 500);
    m_loginCard->setBorderRadius(20);
    
    // 创建卡片内容容器（避免 CardWidget 的鼠标事件干扰）
    QWidget *cardContentWidget = new QWidget(m_loginCard);
    QVBoxLayout *cardWrapperLayout = new QVBoxLayout(m_loginCard);
    cardWrapperLayout->setContentsMargins(0, 0, 0, 0);
    cardWrapperLayout->setSpacing(0);
    cardWrapperLayout->addWidget(cardContentWidget);
    
    // 卡片内部布局
    QVBoxLayout *cardLayout = new QVBoxLayout(cardContentWidget);
    cardLayout->setContentsMargins(60, 50, 60, 50);
    cardLayout->setSpacing(20);
    
    // Logo 图标区域
    m_logoLabel = new QLabel(cardContentWidget);
    m_logoLabel->setAlignment(Qt::AlignCenter);
    
    // 尝试加载图标
    QPixmap iconPixmap(":/icon.ico");
    if (!iconPixmap.isNull()) {
        m_logoLabel->setPixmap(iconPixmap.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_logoLabel->setFixedHeight(75);
    } else {
        // 如果没有图标，创建一个带文字的占位符
        m_logoLabel->setStyleSheet(R"(
            QLabel {
                color: #667eea;
                font-size: 56px;
            }
        )");
        m_logoLabel->setText("🎓");
        m_logoLabel->setFixedHeight(70);
    }
    cardLayout->addWidget(m_logoLabel);
    
    // Logo和标题之间添加间距
    cardLayout->addSpacing(15);
    
    // 标题
    m_titleLabel = new QLabel("学生信息管理系统", cardContentWidget);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(R"(
        QLabel {
            color: #667eea;
            font-size: 26px;
            font-weight: bold;
            font-family: "Microsoft YaHei UI", sans-serif;
            letter-spacing: 1px;
            padding-top: 5px;
        }
    )");
    cardLayout->addWidget(m_titleLabel);
    
    // 副标题
    m_subtitleLabel = new QLabel("欢迎回来，请登录您的账户", cardContentWidget);
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    m_subtitleLabel->setStyleSheet(R"(
        QLabel {
            color: #8B95A5;
            font-size: 14px;
            font-family: "Microsoft YaHei UI", sans-serif;
        }
    )");
    cardLayout->addWidget(m_subtitleLabel);
    
    cardLayout->addSpacing(10);
    
    // 用户名输入框
    m_usernameInput = new ElaLineEdit(cardContentWidget);
    m_usernameInput->setPlaceholderText("请输入用户名");
    m_usernameInput->setFixedHeight(50);
    m_usernameInput->setText("root");  // 预填充用户名
    m_usernameInput->setStyleSheet(R"(
        ElaLineEdit {
            font-size: 15px;
            padding: 0 18px;
            border: 2px solid transparent;
            border-radius: 10px;
            background-color: rgba(102, 126, 234, 0.08);
        }
        ElaLineEdit:focus {
            border: 2px solid #667eea;
            background-color: rgba(102, 126, 234, 0.12);
        }
    )");
    
    // 添加输入框阴影
    QGraphicsDropShadowEffect *usernameShadow = new QGraphicsDropShadowEffect(m_usernameInput);
    usernameShadow->setBlurRadius(15);
    usernameShadow->setColor(QColor(102, 126, 234, 40));
    usernameShadow->setOffset(0, 2);
    m_usernameInput->setGraphicsEffect(usernameShadow);
    
    cardLayout->addWidget(m_usernameInput);
    
    // 密码输入框
    m_passwordInput = new ElaLineEdit(cardContentWidget);
    m_passwordInput->setPlaceholderText("请输入密码");
    m_passwordInput->setEchoMode(QLineEdit::Password);
    m_passwordInput->setFixedHeight(50);
    m_passwordInput->setStyleSheet(R"(
        ElaLineEdit {
            font-size: 15px;
            padding: 0 18px;
            border: 2px solid transparent;
            border-radius: 10px;
            background-color: rgba(102, 126, 234, 0.08);
        }
        ElaLineEdit:focus {
            border: 2px solid #667eea;
            background-color: rgba(102, 126, 234, 0.12);
        }
    )");
    
    // 添加输入框阴影
    QGraphicsDropShadowEffect *passwordShadow = new QGraphicsDropShadowEffect(m_passwordInput);
    passwordShadow->setBlurRadius(15);
    passwordShadow->setColor(QColor(102, 126, 234, 40));
    passwordShadow->setOffset(0, 2);
    m_passwordInput->setGraphicsEffect(passwordShadow);
    
    // 回车键登录
    connect(m_passwordInput, &ElaLineEdit::returnPressed, this, &LoginWindow::onLoginButtonClicked);
    
    cardLayout->addWidget(m_passwordInput);
    
    cardLayout->addSpacing(10);
    
    // 登录按钮
    m_loginButton = new ElaPushButton("登 录", cardContentWidget);
    m_loginButton->setFixedHeight(52);
    m_loginButton->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #667eea, stop:1 #764ba2);
            color: white;
            font-size: 17px;
            font-weight: bold;
            border-radius: 12px;
            border: none;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #7a8df5, stop:1 #8a5bb8);
        }
        ElaPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #5a6fd8, stop:1 #6a3f90);
            padding-top: 2px;
        }
    )");
    
    // 添加增强的按钮阴影效果
    QGraphicsDropShadowEffect *buttonShadow = new QGraphicsDropShadowEffect(m_loginButton);
    buttonShadow->setBlurRadius(30);
    buttonShadow->setColor(QColor(102, 126, 234, 120));
    buttonShadow->setOffset(0, 6);
    m_loginButton->setGraphicsEffect(buttonShadow);
    
    connect(m_loginButton, &ElaPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    
    cardLayout->addWidget(m_loginButton);
    
    cardLayout->addSpacing(10);
    
    // 提示信息
    QLabel *hintLabel = new QLabel("默认账号：root  |  默认密码：root", cardContentWidget);
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setStyleSheet(R"(
        QLabel {
            color: #95A5A6;
            font-size: 12px;
            font-family: "Microsoft YaHei UI", sans-serif;
        }
    )");
    cardLayout->addWidget(hintLabel);
    
    // 将登录卡片添加到主布局（水平居中）
    QHBoxLayout *cardHLayout = new QHBoxLayout();
    cardHLayout->addStretch();
    cardHLayout->addWidget(m_loginCard);
    cardHLayout->addStretch();
    
    mainLayout->addLayout(cardHLayout);
    
    // 下方弹簧
    mainLayout->addStretch(1);
    
    // 底部版权信息
    QLabel *copyrightLabel = new QLabel("© 2024 学生信息管理系统", centralWidget);
    copyrightLabel->setAlignment(Qt::AlignCenter);
    copyrightLabel->setStyleSheet(R"(
        QLabel {
            color: #BDC3C7;
            font-size: 12px;
            padding: 10px;
        }
    )");
    mainLayout->addWidget(copyrightLabel);
    
    // 设置中心部件
    setCentralWidget(centralWidget);
    
    // 延迟设置焦点到密码框（等待窗口完全显示和布局完成）
    QTimer::singleShot(500, this, [this]() {
        if (m_passwordInput && this->isVisible()) {
            m_passwordInput->setFocus();
        }
    });
}

void LoginWindow::onLoginButtonClicked()
{
    QString username = m_usernameInput->text().trimmed();
    QString password = m_passwordInput->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        showLoginError("请输入用户名和密码");
        return;
    }
    
    // 验证用户名和密码
    if (username == "root" && password == "root") {
        // 显示成功消息
        ElaMessageBar::success(ElaMessageBarType::TopRight, "登录成功", 
                              QString("欢迎，%1！").arg(username), 2000, this);
        
        // 短暂延迟后发送登录信号（让用户看到成功消息）
        QTimer::singleShot(300, [this, username, password]() {
            emit loginClicked(username, password);
        });
    } else {
        showLoginError("用户名或密码错误！");
        // 清空密码输入框
        m_passwordInput->clear();
        m_passwordInput->setFocus();
    }
}

void LoginWindow::showLoginError(const QString &message)
{
    // 使用 ElaMessageBar 显示错误信息
    ElaMessageBar::error(ElaMessageBarType::TopRight, "登录失败", message, 3000, this);
    
    // 添加输入框抖动效果
    QPropertyAnimation *shakeAnim = new QPropertyAnimation(m_loginCard, "pos");
    shakeAnim->setDuration(500);
    shakeAnim->setLoopCount(1);
    
    QPoint originalPos = m_loginCard->pos();
    shakeAnim->setKeyValueAt(0, originalPos);
    shakeAnim->setKeyValueAt(0.1, originalPos + QPoint(-10, 0));
    shakeAnim->setKeyValueAt(0.3, originalPos + QPoint(10, 0));
    shakeAnim->setKeyValueAt(0.5, originalPos + QPoint(-10, 0));
    shakeAnim->setKeyValueAt(0.7, originalPos + QPoint(10, 0));
    shakeAnim->setKeyValueAt(0.9, originalPos + QPoint(-5, 0));
    shakeAnim->setKeyValueAt(1.0, originalPos);
    
    shakeAnim->start(QAbstractAnimation::DeleteWhenStopped);
}
