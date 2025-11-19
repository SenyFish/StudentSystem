#include "loginwindow.h"
#include "cardwidget.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaComboBox.h"
#include "ElaText.h"
#include "ElaMessageBar.h"
#include "ElaContentDialog.h"
#include "ElaApplication.h"
#include "usermanager.h"
#include "mainwindow.h"
#include "student.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QIcon>
#include <QPixmap>
#include <QStackedWidget>
#include <QRegularExpression>

LoginWindow::LoginWindow(QWidget *parent)
    : ElaWindow(parent)
    , m_loginCard(nullptr)
    , m_logoLabel(nullptr)
    , m_titleLabel(nullptr)
    , m_subtitleLabel(nullptr)
    , m_loginWidget(nullptr)
    , m_usernameInput(nullptr)
    , m_passwordInput(nullptr)
    , m_mainWindow(nullptr)
    , m_loginButton(nullptr)
    , m_toRegisterButton(nullptr)
    , m_accountInfoButton(nullptr)
    , m_registerWidget(nullptr)
    , m_regUsernameInput(nullptr)
    , m_regPasswordInput(nullptr)
    , m_regPasswordConfirmInput(nullptr)
    , m_regRealNameInput(nullptr)
    , m_regRoleComboBox(nullptr)
    , m_regStudentIdInput(nullptr)
    , m_regStudentIdLabel(nullptr)
    , m_regMajorComboBox(nullptr)
    , m_regMajorLabel(nullptr)
    , m_registerButton(nullptr)
    , m_toLoginButton(nullptr)
{
    // 配置 ElaWindow
    setWindowTitle("学生信息管理系统 - 登录");
    setWindowIcon(QIcon(":/icon.ico"));
    resize(900, 700);
    setMinimumSize(800, 650);
    
    // 禁用导航栏
    setIsNavigationBarEnable(false);
    
    // 隐藏用户信息卡
    setUserInfoCardVisible(false);
    
    setupUI();
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::setupUI()
{
    // 创建中心容器
    QWidget *centralWidget = new QWidget(this);
    
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    mainLayout->setSpacing(0);
    
    // 上方弹簧
    mainLayout->addStretch(1);
    
    // 创建登录卡片
    m_loginCard = new CardWidget(centralWidget);
    m_loginCard->setFixedSize(580, 600);
    m_loginCard->setBorderRadius(20);
    
    // 卡片内容容器
    QWidget *cardContentWidget = new QWidget(m_loginCard);
    QVBoxLayout *cardWrapperLayout = new QVBoxLayout(m_loginCard);
    cardWrapperLayout->setContentsMargins(0, 0, 0, 0);
    cardWrapperLayout->setSpacing(0);
    cardWrapperLayout->addWidget(cardContentWidget);
    
    // 卡片内部布局
    QVBoxLayout *cardLayout = new QVBoxLayout(cardContentWidget);
    cardLayout->setContentsMargins(60, 50, 60, 50);
    cardLayout->setSpacing(20);
    
    // Logo
    m_logoLabel = new QLabel(cardContentWidget);
    m_logoLabel->setAlignment(Qt::AlignCenter);
    QPixmap iconPixmap(":/icon.ico");
    if (!iconPixmap.isNull()) {
        m_logoLabel->setPixmap(iconPixmap.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_logoLabel->setFixedHeight(75);
    } else {
        m_logoLabel->setStyleSheet("QLabel { color: #667eea; font-size: 56px; }");
        m_logoLabel->setText("🎓");
        m_logoLabel->setFixedHeight(70);
    }
    cardLayout->addWidget(m_logoLabel);
    cardLayout->addSpacing(10);
    
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
        }
    )");
    cardLayout->addWidget(m_titleLabel);
    
    // 副标题
    m_subtitleLabel = new QLabel("请登录您的账户", cardContentWidget);
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
    
    // 创建堆叠布局（用于切换登录/注册界面）
    QStackedWidget *stackedWidget = new QStackedWidget(cardContentWidget);
    
    // ===== 登录界面 =====
    m_loginWidget = new QWidget();
    QVBoxLayout *loginLayout = new QVBoxLayout(m_loginWidget);
    loginLayout->setSpacing(15);
    loginLayout->setContentsMargins(0, 0, 0, 0);
    
    // 学号/用户名输入
    m_usernameInput = new ElaLineEdit(m_loginWidget);
    m_usernameInput->setPlaceholderText("请输入学号（7位数字）或用户名");
    m_usernameInput->setFixedHeight(50);
    m_usernameInput->clear();
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
    loginLayout->addWidget(m_usernameInput);
    
    // 密码输入
    m_passwordInput = new ElaLineEdit(m_loginWidget);
    m_passwordInput->setPlaceholderText("请输入密码（学生密码与学号相同）");
    m_passwordInput->setEchoMode(QLineEdit::Password);
    m_passwordInput->setFixedHeight(50);
    m_passwordInput->clear();
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
    connect(m_passwordInput, &ElaLineEdit::returnPressed, this, &LoginWindow::onLoginButtonClicked);
    loginLayout->addWidget(m_passwordInput);
    
    loginLayout->addSpacing(10);
    
    // 登录按钮
    m_loginButton = new ElaPushButton("登 录", m_loginWidget);
    m_loginButton->setFixedHeight(52);
    m_loginButton->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #667eea, stop:1 #764ba2);
            color: white;
            font-size: 17px;
            font-weight: bold;
            border-radius: 12px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #7a8df5, stop:1 #8a5bb8);
        }
    )");
    connect(m_loginButton, &ElaPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    loginLayout->addWidget(m_loginButton);
    
    loginLayout->addSpacing(8);
    
    // 账号说明按钮
    m_accountInfoButton = new ElaPushButton("账号说明", m_loginWidget);
    m_accountInfoButton->setFixedHeight(40);
    m_accountInfoButton->setStyleSheet(R"(
        ElaPushButton {
            background-color: transparent;
            color: #667eea;
            font-size: 14px;
            border: 1px solid #667eea;
            border-radius: 8px;
        }
        ElaPushButton:hover {
            background-color: rgba(102, 126, 234, 0.1);
        }
    )");
    connect(m_accountInfoButton, &ElaPushButton::clicked, this, &LoginWindow::onAccountInfoButtonClicked);
    loginLayout->addWidget(m_accountInfoButton);
    
    loginLayout->addSpacing(8);
    
    // 跳转到注册按钮
    m_toRegisterButton = new ElaPushButton("还没有账号？立即注册", m_loginWidget);
    m_toRegisterButton->setFixedHeight(40);
    m_toRegisterButton->setStyleSheet(R"(
        ElaPushButton {
            background: transparent;
            color: #667eea;
            font-size: 13px;
            border: none;
        }
        ElaPushButton:hover {
            color: #7a8df5;
            text-decoration: underline;
        }
    )");
    connect(m_toRegisterButton, &ElaPushButton::clicked, this, &LoginWindow::switchToRegister);
    loginLayout->addWidget(m_toRegisterButton);
    
    stackedWidget->addWidget(m_loginWidget);
    
    // ===== 注册界面 =====
    m_registerWidget = new QWidget();
    QVBoxLayout *registerLayout = new QVBoxLayout(m_registerWidget);
    registerLayout->setSpacing(12);
    registerLayout->setContentsMargins(0, 0, 0, 0);
    
    // 用户名输入
    m_regUsernameInput = new ElaLineEdit(m_registerWidget);
    m_regUsernameInput->setPlaceholderText("请输入用户名");
    m_regUsernameInput->setFixedHeight(45);
    m_regUsernameInput->setStyleSheet(R"(
        ElaLineEdit {
            font-size: 14px;
            padding: 0 15px;
            border: 2px solid transparent;
            border-radius: 10px;
            background-color: rgba(102, 126, 234, 0.08);
        }
        ElaLineEdit:focus {
            border: 2px solid #667eea;
        }
    )");
    registerLayout->addWidget(m_regUsernameInput);
    
    // 密码输入
    m_regPasswordInput = new ElaLineEdit(m_registerWidget);
    m_regPasswordInput->setPlaceholderText("请输入密码");
    m_regPasswordInput->setEchoMode(QLineEdit::Password);
    m_regPasswordInput->setFixedHeight(45);
    m_regPasswordInput->setStyleSheet(m_regUsernameInput->styleSheet());
    registerLayout->addWidget(m_regPasswordInput);
    
    // 确认密码输入
    m_regPasswordConfirmInput = new ElaLineEdit(m_registerWidget);
    m_regPasswordConfirmInput->setPlaceholderText("请再次输入密码");
    m_regPasswordConfirmInput->setEchoMode(QLineEdit::Password);
    m_regPasswordConfirmInput->setFixedHeight(45);
    m_regPasswordConfirmInput->setStyleSheet(m_regUsernameInput->styleSheet());
    registerLayout->addWidget(m_regPasswordConfirmInput);
    
    // 真实姓名输入
    m_regRealNameInput = new ElaLineEdit(m_registerWidget);
    m_regRealNameInput->setPlaceholderText("请输入真实姓名");
    m_regRealNameInput->setFixedHeight(45);
    m_regRealNameInput->setStyleSheet(m_regUsernameInput->styleSheet());
    registerLayout->addWidget(m_regRealNameInput);
    
    // 角色选择
    m_regRoleComboBox = new ElaComboBox(m_registerWidget);
    m_regRoleComboBox->addItem("学生");
    m_regRoleComboBox->addItem("教师");
    m_regRoleComboBox->setFixedHeight(45);
    registerLayout->addWidget(m_regRoleComboBox);
    
    // 学号输入（仅学生角色显示）
    m_regStudentIdLabel = new QLabel("学号", m_registerWidget);
    m_regStudentIdLabel->setStyleSheet(R"(
        QLabel {
            color: #667eea;
            font-size: 14px;
            font-weight: bold;
            padding-left: 2px;
        }
    )");
    m_regStudentIdInput = new ElaLineEdit(m_registerWidget);
    m_regStudentIdInput->setPlaceholderText("请输入您的学号（7位数字）");
    m_regStudentIdInput->setFixedHeight(45);
    m_regStudentIdInput->setStyleSheet(m_regUsernameInput->styleSheet());
    m_regStudentIdInput->setVisible(false);  // 默认隐藏
    m_regStudentIdLabel->setVisible(false);  // 默认隐藏
    
    // 专业选择（仅教师角色显示）
    m_regMajorLabel = new QLabel("专业", m_registerWidget);
    m_regMajorLabel->setStyleSheet(R"(
        QLabel {
            color: #667eea;
            font-size: 14px;
            font-weight: bold;
            padding-left: 2px;
        }
    )");
    m_regMajorComboBox = new ElaComboBox(m_registerWidget);
    m_regMajorComboBox->addItem("计算机科学");
    m_regMajorComboBox->addItem("软件工程");
    m_regMajorComboBox->addItem("信息安全");
    m_regMajorComboBox->addItem("数据科学");
    m_regMajorComboBox->addItem("人工智能");
    m_regMajorComboBox->addItem("网络工程");
    m_regMajorComboBox->addItem("物联网工程");
    m_regMajorComboBox->addItem("电子信息工程");
    m_regMajorComboBox->addItem("通信工程");
    m_regMajorComboBox->addItem("自动化");
    m_regMajorComboBox->setFixedHeight(45);
    m_regMajorComboBox->setVisible(false);  // 默认隐藏
    m_regMajorLabel->setVisible(false);  // 默认隐藏
    
    // 监听角色选择变化，显示/隐藏学号输入框和专业选择框
    connect(m_regRoleComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), 
            this, [this](int index) {
        bool isStudent = (index == 0);  // 0是"学生"
        bool isTeacher = (index == 1);  // 1是"教师"
        m_regStudentIdInput->setVisible(isStudent);
        m_regStudentIdLabel->setVisible(isStudent);
        m_regMajorComboBox->setVisible(isTeacher);
        m_regMajorLabel->setVisible(isTeacher);
    });
    
    registerLayout->addWidget(m_regStudentIdLabel);
    registerLayout->addWidget(m_regStudentIdInput);
    registerLayout->addWidget(m_regMajorLabel);
    registerLayout->addWidget(m_regMajorComboBox);
    
    registerLayout->addSpacing(10);
    
    // 注册按钮
    m_registerButton = new ElaPushButton("注 册", m_registerWidget);
    m_registerButton->setFixedHeight(50);
    m_registerButton->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #43e97b, stop:1 #38f9d7);
            color: white;
            font-size: 16px;
            font-weight: bold;
            border-radius: 12px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #55f58d, stop:1 #4affea);
        }
    )");
    connect(m_registerButton, &ElaPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);
    registerLayout->addWidget(m_registerButton);
    
    // 返回登录按钮
    m_toLoginButton = new ElaPushButton("已有账号？返回登录", m_registerWidget);
    m_toLoginButton->setFixedHeight(36);
    m_toLoginButton->setStyleSheet(R"(
        ElaPushButton {
            background: transparent;
            color: #667eea;
            font-size: 13px;
            border: none;
        }
        ElaPushButton:hover {
            color: #7a8df5;
        }
    )");
    connect(m_toLoginButton, &ElaPushButton::clicked, this, &LoginWindow::switchToLogin);
    registerLayout->addWidget(m_toLoginButton);
    
    stackedWidget->addWidget(m_registerWidget);
    
    cardLayout->addWidget(stackedWidget);
    
    // 将登录卡片添加到主布局
    QHBoxLayout *cardHLayout = new QHBoxLayout();
    cardHLayout->addStretch();
    cardHLayout->addWidget(m_loginCard);
    cardHLayout->addStretch();
    
    mainLayout->addLayout(cardHLayout);
    mainLayout->addStretch(1);
    
    // 底部版权信息
    QLabel *copyrightLabel = new QLabel("© 2025 学生信息管理系统", centralWidget);
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
    
    // 先禁用所有输入框，避免在窗口初始化时点击导致卡死
    if (m_usernameInput) {
        m_usernameInput->setEnabled(false);
    }
    if (m_passwordInput) {
        m_passwordInput->setEnabled(false);
    }
    
    // 延迟启用输入框并设置焦点，确保窗口完全初始化后再允许交互
    QTimer::singleShot(800, this, [this]() {
        if (m_usernameInput && this->isVisible()) {
            m_usernameInput->setEnabled(true);
        }
        if (m_passwordInput && this->isVisible()) {
            m_passwordInput->setEnabled(true);
            m_passwordInput->setFocus();
        }
    });
}

void LoginWindow::onLoginButtonClicked()
{
    QString input = m_usernameInput->text().trimmed();
    QString password = m_passwordInput->text();
    
    if (input.isEmpty()) {
        showLoginError("请输入学号或用户名");
        return;
    }
    
    User user;
    bool loginOk = false;
    
    // 判断输入的是学号（7位数字）还是用户名
    QRegularExpression idPattern("^\\d{7}$");
    if (idPattern.match(input).hasMatch()) {
        // 输入的是学号，按学号登录
        qDebug() << "尝试按学号登录:" << input;
        
        // 密码默认为学号
        if (password.isEmpty()) {
            password = input;
            m_passwordInput->setText(password);
        }
        
        loginOk = UserManager::getInstance().loginByStudentId(input, password, user);
        
        // 如果按学号登录失败，尝试自动创建学生账户
        if (!loginOk && m_mainWindow) {
            Student* student = m_mainWindow->getStudentById(input);
            if (student) {
                // 学号在学生列表中，自动创建学生账户（密码为学号）
                QString username = "student_" + input;  // 生成唯一用户名
                QString realName = student->getName();
                
                // 检查用户名是否已存在，如果存在则使用学号作为用户名
                if (UserManager::getInstance().userExists(username)) {
                    username = input;
                }
                
                // 自动注册学生账户（密码为学号）
                if (UserManager::getInstance().registerUser(username, input, UserRole::Student, realName, input)) {
                    // 注册成功后再次登录
                    loginOk = UserManager::getInstance().loginByStudentId(input, input, user);
                    if (loginOk) {
                        ElaMessageBar::success(ElaMessageBarType::TopRight, "账户已创建", 
                                              QString("已为您自动创建账户，欢迎 %1！\n密码与学号相同：%2").arg(realName).arg(input), 
                                              4000, this);
                    }
                }
            }
        }
    } else {
        // 输入的是用户名，按用户名登录（用于管理员和教师）
        qDebug() << "尝试按用户名登录:" << input;
        loginOk = UserManager::getInstance().login(input, password, user);
    }
    
    if (loginOk) {
        ElaMessageBar::success(ElaMessageBarType::TopRight, "登录成功", 
                              QString("欢迎，%1（%2）！").arg(user.getRealName()).arg(user.getRoleString()), 
                              2000, this);
        
        // 保存当前用户到UserManager
        UserManager::getInstance().setCurrentUser(user);
        
        // 传递用户名（如果是学号登录，传递用户名）
        emit loginSuccess(user.getUsername());
    } else {
        if (idPattern.match(input).hasMatch()) {
            showLoginError("学号或密码错误！\n提示：学生账户密码与学号相同");
        } else {
            showLoginError("用户名或密码错误！");
        }
        m_passwordInput->clear();
        m_usernameInput->setFocus();
    }
}

void LoginWindow::onRegisterButtonClicked()
{
    QString username = m_regUsernameInput->text().trimmed();
    QString password = m_regPasswordInput->text();
    QString passwordConfirm = m_regPasswordConfirmInput->text();
    QString realName = m_regRealNameInput->text().trimmed();
    QString roleStr = m_regRoleComboBox->currentText();
    QString studentId = m_regStudentIdInput->text().trimmed();
    QString major = m_regMajorComboBox->currentText();
    
    // 验证输入
    if (username.isEmpty()) {
        showRegisterError("用户名不能为空！");
        return;
    }
    
    if (password.isEmpty()) {
        showRegisterError("密码不能为空！");
        return;
    }
    
    if (password != passwordConfirm) {
        showRegisterError("两次输入的密码不一致！");
        return;
    }
    
    if (password.length() < 4) {
        showRegisterError("密码长度不能少于4位！");
        return;
    }
    
    if (realName.isEmpty()) {
        showRegisterError("真实姓名不能为空！");
        return;
    }
    
    // 转换角色
    UserRole role = User::stringToRole(roleStr);
    
    // 如果是学生角色，验证学号
    if (role == UserRole::Student) {
        if (studentId.isEmpty()) {
            showRegisterError("学生角色必须输入学号！");
            return;
        }
        
        // 验证学号格式（7位数字）
        QRegularExpression idPattern("^\\d{7}$");
        if (!idPattern.match(studentId).hasMatch()) {
            showRegisterError("学号格式不正确！\n学号必须是7位数字，例如：2021001");
            return;
        }
    }
    // 如果是教师角色，验证专业
    else if (role == UserRole::Teacher) {
        if (major.isEmpty()) {
            showRegisterError("教师角色必须选择专业！");
            return;
        }
    }
    
    // 注册用户（传递学号和专业）
    if (UserManager::getInstance().registerUser(username, password, role, realName, studentId, major)) {
        ElaMessageBar::success(ElaMessageBarType::TopRight, "注册成功", 
                              "账号创建成功，请登录！", 2000, this);
        
        // 延迟切换到登录界面
        QTimer::singleShot(500, this, &LoginWindow::switchToLogin);
        
        // 填充用户名
        m_usernameInput->setText(username);
        m_passwordInput->clear();
    } else {
        showRegisterError("注册失败！用户名可能已存在。");
    }
}

void LoginWindow::switchToRegister()
{
    // 切换到注册界面
    QStackedWidget *stackedWidget = m_loginCard->findChild<QStackedWidget*>();
    if (stackedWidget) {
        stackedWidget->setCurrentWidget(m_registerWidget);
        m_subtitleLabel->setText("创建新账户");
        
        // 清空输入
        m_regUsernameInput->clear();
        m_regPasswordInput->clear();
        m_regPasswordConfirmInput->clear();
        m_regRealNameInput->clear();
        m_regStudentIdInput->clear();
        m_regMajorComboBox->setCurrentIndex(0);
        m_regRoleComboBox->setCurrentIndex(0);
        
        // 根据角色显示/隐藏学号输入框和专业选择框
        m_regStudentIdInput->setVisible(true);  // 默认是学生角色
        m_regStudentIdLabel->setVisible(true);
        m_regMajorComboBox->setVisible(false);
        m_regMajorLabel->setVisible(false);
        
        // 设置焦点
        QTimer::singleShot(100, m_regUsernameInput, QOverload<>::of(&ElaLineEdit::setFocus));
    }
}

void LoginWindow::switchToLogin()
{
    // 切换到登录界面
    QStackedWidget *stackedWidget = m_loginCard->findChild<QStackedWidget*>();
    if (stackedWidget) {
        stackedWidget->setCurrentWidget(m_loginWidget);
        m_subtitleLabel->setText("欢迎回来，请登录您的账户");
        
        // 设置焦点
        QTimer::singleShot(100, m_passwordInput, QOverload<>::of(&ElaLineEdit::setFocus));
    }
}

void LoginWindow::showLoginError(const QString &message)
{
    ElaMessageBar::error(ElaMessageBarType::TopRight, "登录失败", message, 3000, this);
    
    // 抖动效果
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

void LoginWindow::showRegisterError(const QString &message)
{
    ElaMessageBar::error(ElaMessageBarType::TopRight, "注册失败", message, 3000, this);
}

void LoginWindow::onAccountInfoButtonClicked()
{
    // 创建账号说明对话框
    ElaContentDialog* dialog = new ElaContentDialog(this);
    dialog->setWindowTitle("账号说明");
    dialog->resize(600, 500);
    
    QWidget* dialogWidget = new QWidget(this);
    QVBoxLayout* dialogLayout = new QVBoxLayout(dialogWidget);
    dialogLayout->setSpacing(20);
    dialogLayout->setContentsMargins(30, 30, 30, 30);
    
    // 标题
    ElaText* titleText = new ElaText("账号登录说明", this);
    titleText->setTextPixelSize(20);
    titleText->setStyleSheet("font-weight: bold; color: #667eea;");
    dialogLayout->addWidget(titleText);
    
    dialogLayout->addSpacing(10);
    
    // 学生账号说明
    QWidget* studentSection = new QWidget(this);
    QVBoxLayout* studentLayout = new QVBoxLayout(studentSection);
    studentLayout->setSpacing(8);
    studentLayout->setContentsMargins(0, 0, 0, 0);
    
    ElaText* studentTitle = new ElaText("📚 学生账号", this);
    studentTitle->setTextPixelSize(16);
    studentTitle->setStyleSheet("font-weight: bold; color: #2c3e50;");
    studentLayout->addWidget(studentTitle);
    
    ElaText* studentInfo = new ElaText(
        "• 登录方式：使用学号登录（7位数字）\n"
        "• 密码：与学号相同\n"
        "• 示例：学号 2021001，密码 2021001\n"
        "• 权限：只能查看和修改自己的信息\n"
        "• 说明：首次登录时系统会自动创建账户", this);
    studentInfo->setTextPixelSize(14);
    studentInfo->setStyleSheet("color: #34495e; line-height: 1.6;");
    studentLayout->addWidget(studentInfo);
    
    dialogLayout->addWidget(studentSection);
    dialogLayout->addSpacing(15);
    
    // 教师账号说明
    QWidget* teacherSection = new QWidget(this);
    QVBoxLayout* teacherLayout = new QVBoxLayout(teacherSection);
    teacherLayout->setSpacing(8);
    teacherLayout->setContentsMargins(0, 0, 0, 0);
    
    ElaText* teacherTitle = new ElaText("👨‍🏫 教师账号", this);
    teacherTitle->setTextPixelSize(16);
    teacherTitle->setStyleSheet("font-weight: bold; color: #2c3e50;");
    teacherLayout->addWidget(teacherTitle);
    
    ElaText* teacherInfo = new ElaText(
        "• 登录方式：使用学号登录（7位数字）\n"
        "• 密码：与学号相同\n"
        "• 默认账号：\n"
        "  - teacher / teacher（计算机科学专业）\n"
        "  - teacher2 / teacher2（软件工程专业）\n"
        "  - teacher3 / teacher3（信息安全专业）\n"
        "• 权限：只能查看和管理自己专业的学生", this);
    teacherInfo->setTextPixelSize(14);
    teacherInfo->setStyleSheet("color: #34495e; line-height: 1.6;");
    teacherLayout->addWidget(teacherInfo);
    
    dialogLayout->addWidget(teacherSection);
    dialogLayout->addSpacing(15);
    
    // 管理员账号说明
    QWidget* adminSection = new QWidget(this);
    QVBoxLayout* adminLayout = new QVBoxLayout(adminSection);
    adminLayout->setSpacing(8);
    adminLayout->setContentsMargins(0, 0, 0, 0);
    
    ElaText* adminTitle = new ElaText("👑 管理员账号", this);
    adminTitle->setTextPixelSize(16);
    adminTitle->setStyleSheet("font-weight: bold; color: #2c3e50;");
    adminLayout->addWidget(adminTitle);
    
    ElaText* adminInfo = new ElaText(
        "• 登录方式：使用用户名登录\n"
        "• 默认账号：root / root\n"
        "• 权限：拥有全部权限，可以管理所有学生", this);
    adminInfo->setTextPixelSize(14);
    adminInfo->setStyleSheet("color: #34495e; line-height: 1.6;");
    adminLayout->addWidget(adminInfo);
    
    dialogLayout->addWidget(adminSection);
    dialogLayout->addSpacing(15);
    
    // 提示信息
    ElaText* tipText = new ElaText("💡 提示：学生使用学号登录，密码与学号相同；教师和管理员使用用户名登录", this);
    tipText->setTextPixelSize(13);
    tipText->setStyleSheet("color: #e74c3c; font-weight: bold;");
    dialogLayout->addWidget(tipText);
    
    dialogLayout->addStretch();
    
    dialog->setCentralWidget(dialogWidget);
    dialog->setLeftButtonText("关闭");
    dialog->setRightButtonText("");
    
    connect(dialog, &ElaContentDialog::leftButtonClicked, dialog, &ElaContentDialog::close);
    
    dialog->exec();
    dialog->deleteLater();
}
