#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "flatcardwidget.h"
#include "ElaScrollPage.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaContentDialog.h"
#include "ElaMessageBar.h"
#include "ElaTheme.h"
#include "ElaLineEdit.h"
#include "ElaComboBox.h"
#include "ElaSpinBox.h"
#include "usermanager.h"
#include "operationlog.h"
#include "ModernAvatar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QDebug>
#include <QIcon>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QRegularExpression>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <QDateTime>
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
    , ui(new Ui::MainWindow)
    , currentUserRole(UserRole::Student)  // 默认角色
    , m_leftCardWidget(nullptr)
    , m_rightCardWidget(nullptr)
    , m_cardViewWidget(nullptr)
    , m_cardScrollArea(nullptr)
    , m_cardContainer(nullptr)
    , m_cardLayout(nullptr)
    , m_isThemeChanging(false)
    , m_lastThemeChangeTime(0)
{
    qDebug() << "MainWindow 构造函数开始";
    
    ui->setupUi(this);
    
    // ===== 配置 ElaWindow =====
    setWindowTitle("学生信息管理系统");
    setWindowIcon(QIcon(":/icon.ico"));  // 设置窗口图标
    resize(1400, 800);
    
    // 先禁用窗口交互，等待完全初始化后再启用
    setEnabled(false);
    
    // 设置用户信息卡（可选，如果不需要可以隐藏）
    setUserInfoCardVisible(false);  // 隐藏用户信息卡
    
    // 或者设置用户信息卡内容
    // setUserInfoCardTitle("学生管理系统");
    // setUserInfoCardSubTitle("管理员");
    
    qDebug() << "开始初始化UI";
    initUI();  // 这里会创建 tableWidget
    
    qDebug() << "开始初始化表格";
    initTable();  // 现在可以配置表格了
    
    qDebug() << "添加示例数据";
    // 添加更多测试数据
    // 计算机科学专业
    students.append(Student("2021001", "张三", "男", 20, "计算机科学"));
    students.append(Student("2021006", "孙八", "男", 19, "计算机科学"));
    students.append(Student("2021011", "周九", "女", 21, "计算机科学"));
    students.append(Student("2021016", "吴十", "男", 20, "计算机科学"));
    
    // 软件工程专业
    students.append(Student("2021002", "李四", "女", 19, "软件工程"));
    students.append(Student("2021007", "郑一", "女", 20, "软件工程"));
    students.append(Student("2021012", "王二", "男", 22, "软件工程"));
    students.append(Student("2021017", "冯三", "女", 19, "软件工程"));
    
    // 信息安全专业
    students.append(Student("2021003", "王五", "男", 21, "信息安全"));
    students.append(Student("2021008", "陈四", "男", 20, "信息安全"));
    students.append(Student("2021013", "褚五", "女", 21, "信息安全"));
    students.append(Student("2021018", "卫六", "男", 22, "信息安全"));
    
    // 数据科学专业
    students.append(Student("2021004", "赵六", "女", 20, "数据科学"));
    students.append(Student("2021009", "蒋七", "女", 19, "数据科学"));
    students.append(Student("2021014", "沈八", "男", 21, "数据科学"));
    students.append(Student("2021019", "韩九", "女", 20, "数据科学"));
    
    // 人工智能专业
    students.append(Student("2021005", "钱七", "男", 22, "人工智能"));
    students.append(Student("2021010", "杨十", "男", 20, "人工智能"));
    students.append(Student("2021015", "朱一", "女", 19, "人工智能"));
    students.append(Student("2021020", "秦二", "男", 21, "人工智能"));
    
    // 网络工程专业
    students.append(Student("2021021", "尤三", "男", 20, "网络工程"));
    students.append(Student("2021022", "许四", "女", 19, "网络工程"));
    students.append(Student("2021023", "何五", "男", 22, "网络工程"));
    
    // 物联网工程专业
    students.append(Student("2021024", "吕六", "女", 20, "物联网工程"));
    students.append(Student("2021025", "施七", "男", 21, "物联网工程"));
    students.append(Student("2021026", "张八", "女", 19, "物联网工程"));
    
    // 电子信息工程专业
    students.append(Student("2021027", "孔九", "男", 20, "电子信息工程"));
    students.append(Student("2021028", "曹十", "女", 22, "电子信息工程"));
    students.append(Student("2021029", "严一", "男", 21, "电子信息工程"));
    
    // 通信工程专业
    students.append(Student("2021030", "华二", "女", 19, "通信工程"));
    students.append(Student("2021031", "金三", "男", 20, "通信工程"));
    students.append(Student("2021032", "魏四", "女", 21, "通信工程"));
    
    // 自动化专业
    students.append(Student("2021033", "陶五", "男", 22, "自动化"));
    students.append(Student("2021034", "姜六", "女", 20, "自动化"));
    students.append(Student("2021035", "戚七", "男", 19, "自动化"));
    
    updateTable();
    
    qDebug() << "MainWindow 构造函数完成";
    
    // 延迟显示欢迎消息，避免在窗口初始化时显示
    QTimer::singleShot(1200, this, [this]() {
        if (this->isVisible() && this->isEnabled()) {
        ElaMessageBar::success(ElaMessageBarType::TopRight, "欢迎", 
                              QString("系统已加载 %1 条学生记录").arg(students.size()), 3000, this);
        }
    });
    
    // 监听主题变化，添加防抖机制
    connect(ElaTheme::getInstance(), &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode) {
        // 主题切换完成，重置标志并更新时间戳
        m_isThemeChanging = false;
        m_lastThemeChangeTime = QDateTime::currentMSecsSinceEpoch();
        qDebug() << "主题切换完成";
    });
    
    // 确保窗口完全初始化后再允许主题切换
    // 通过延迟处理来避免主题切换按钮在窗口初始化时导致卡死
    QTimer::singleShot(1500, this, [this]() {
        // 强制更新一次，确保所有布局和渲染都完成
        if (this->isVisible()) {
            this->update();
            this->repaint();
            QApplication::processEvents();
        }
    });
}

void MainWindow::setUserRole(UserRole role, const QString &studentId, const QString &major)
{
    currentUserRole = role;
    currentUserStudentId = studentId;
    currentUserMajor = major;
    
    // 根据角色设置权限
    bool canModify = (role == UserRole::Admin);  // 只有管理员可以修改
    bool canDelete = (role == UserRole::Admin);
    bool isAdmin = (role == UserRole::Admin);
    bool isStudent = (role == UserRole::Student);
    bool isTeacher = (role == UserRole::Teacher);
    
    // 学生角色：隐藏输入表单区域和表格，显示信息卡片
    if (m_leftCardWidget) {
        m_leftCardWidget->setVisible(!isStudent && !isTeacher);  // 学生和教师都隐藏输入表单
    }
    if (m_rightCardWidget) {
        m_rightCardWidget->setVisible(isAdmin);  // 只有管理员显示表格
    }
    if (m_cardViewWidget) {
        m_cardViewWidget->setVisible(isTeacher || isStudent);  // 教师和学生显示卡片视图
        if (isTeacher || isStudent) {
            updateCardView();  // 更新卡片视图
        }
    }
    
    // 学生角色：只能查看自己的信息，无法编辑
    // 教师角色：可以查看、添加、修改，不能删除
    // 管理员角色：全部权限
    
    btnAdd->setEnabled(canModify && !isStudent);  // 只有管理员可以添加
    btnModify->setEnabled(canModify && !isStudent);  // 只有管理员可以修改
    btnDelete->setEnabled(canDelete);  // 只有管理员可以删除
    btnClear->setEnabled(canModify && !isStudent);  // 只有管理员可以清空
    btnImport->setEnabled(isAdmin);  // 只有管理员可以导入
    btnBatchDelete->setEnabled(canDelete);  // 只有管理员可以批量删除
    btnBatchModify->setEnabled(canModify && !isStudent);  // 只有管理员可以批量修改
    
    // 学生角色：隐藏搜索和排序按钮
    if (btnSearch) btnSearch->setVisible(!isStudent);
    if (btnSort) btnSort->setVisible(!isStudent);
    if (lineEditSearch) lineEditSearch->setVisible(!isStudent);
    
    // 输入框根据权限启用/禁用
    lineEditId->setEnabled(canModify);
    lineEditName->setEnabled(canModify);
    comboBoxGender->setEnabled(canModify);
    spinBoxAge->setEnabled(canModify);
    
    // 如果是教师角色，限制专业选择框只能选择自己的专业
    if (role == UserRole::Teacher && !major.isEmpty()) {
        // 清空并只添加自己的专业
        comboBoxMajor->clear();
        comboBoxMajor->addItem(major);
        comboBoxMajor->setEnabled(false);  // 禁用专业选择，只能是自己专业
    } else {
        // 管理员或其他角色，恢复所有专业选项（如果被清空了）
        if (comboBoxMajor->count() == 0 || (comboBoxMajor->count() == 1 && comboBoxMajor->itemText(0) != "计算机科学")) {
            comboBoxMajor->clear();
            comboBoxMajor->addItem("计算机科学");
            comboBoxMajor->addItem("软件工程");
            comboBoxMajor->addItem("信息安全");
            comboBoxMajor->addItem("数据科学");
            comboBoxMajor->addItem("人工智能");
            comboBoxMajor->addItem("网络工程");
            comboBoxMajor->addItem("物联网工程");
            comboBoxMajor->addItem("电子信息工程");
            comboBoxMajor->addItem("通信工程");
            comboBoxMajor->addItem("自动化");
        }
        comboBoxMajor->setEnabled(canModify);
    }
    
    // 显示权限提示（延迟到窗口展开动画完成后）
    QString roleMsg;
    if (role == UserRole::Student) {
        if (!studentId.isEmpty()) {
            roleMsg = QString("您当前是【学生】角色，只能查看自己的信息（学号：%1）。").arg(studentId);
        } else {
            roleMsg = "您当前是【学生】角色，只能查看学生信息。";
        }
    } else if (role == UserRole::Teacher) {
        if (!major.isEmpty()) {
            roleMsg = QString("您当前是【教师】角色，只能查看【%1】专业的学生信息，无法修改。").arg(major);
        } else {
            roleMsg = "您当前是【教师】角色，只能查看学生信息，无法修改。";
        }
    } else {
        roleMsg = "您当前是【管理员】角色，拥有全部权限。";
    }
    
    // 延迟700ms，确保窗口展开动画（600ms）完成后显示
    QTimer::singleShot(700, this, [this, roleMsg]() {
        if (this->isVisible() && this->isEnabled()) {
            ElaMessageBar::information(ElaMessageBarType::TopRight, "权限提示", roleMsg, 4000, this);
        }
    });
    
    // 更新表格显示（根据角色过滤）
    updateTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    // 创建主布局容器
    QWidget* centralWidget = new QWidget(this);
    
    // 根据主题设置背景色
    auto updateBackground = [centralWidget]() {
        ElaThemeType::ThemeMode themeMode = ElaTheme::getInstance()->getThemeMode();
        QString bgColor = (themeMode == ElaThemeType::Light) ? "#f5f7fa" : "#1a1a1a";
        centralWidget->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    };
    
    // 初始设置背景
    updateBackground();
    
    // 监听主题变化
    connect(ElaTheme::getInstance(), &ElaTheme::themeModeChanged, centralWidget, updateBackground);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    
    // 获取屏幕的逻辑 DPI 缩放因子，用于计算自适应尺寸
    QScreen *screen = QGuiApplication::primaryScreen();
    qreal screenDpiX = screen ? screen->logicalDotsPerInch() : 96.0;
    qreal dpiScale = screenDpiX / 96.0;  // 96 DPI 是标准基准
    
    // 对于高 DPI 屏幕 (>150%)，使用更保守的宽度计算
    // 175% DPI 时 dpiScale ≈ 1.75
    qreal widthScale = dpiScale;
    if (dpiScale > 1.5) {
        // 对于超过 150% 的缩放，额外增加 30% 宽度以确保文字不被截断
        widthScale = dpiScale * 1.3;
    }
    
    // 计算自适应尺寸（基于 DPI 缩放）
    int inputHeight = qRound(38 * dpiScale);  // 输入框高度
    int buttonHeight = qRound(45 * dpiScale);  // 按钮高度
    int separatorHeight = qMax(2, qRound(2 * dpiScale));  // 分隔线高度，至少2像素
    int spacing12 = qRound(12 * dpiScale);  // 小间距
    int spacing15 = qRound(15 * dpiScale);  // 中间距
    int spacing20 = qRound(20 * dpiScale);  // 大间距
    
    // 对于高DPI，减小内边距以节省空间
    int contentPadding;
    if (dpiScale > 1.5) {
        contentPadding = qRound(15 * dpiScale);  // 高DPI下减小到15
    } else {
        contentPadding = qRound(25 * dpiScale);
    }
    
    // 调整最小宽度以适应高 DPI 屏幕，使用更宽的宽度
    // 对于小屏幕高DPI，使用更激进的宽度计算
    int minCardWidth, maxCardWidth;
    if (dpiScale > 1.5) {
        // 175% DPI下，使用足够大的固定宽度
        minCardWidth = qRound(450 * widthScale);  // 进一步增加基础宽度
        maxCardWidth = qRound(650 * widthScale);  // 增加最大宽度
    } else {
        minCardWidth = qRound(380 * dpiScale);
        maxCardWidth = qRound(480 * dpiScale);
    }
    
    qDebug() << "屏幕逻辑DPI:" << screenDpiX << "DPI缩放因子:" << dpiScale 
             << "宽度缩放:" << widthScale << "卡片宽度:" << minCardWidth << "-" << maxCardWidth;
    
    // ===== 左侧：输入表单区域 =====
    // 创建扁平卡片容器
    FlatCardWidget* leftCard = new FlatCardWidget(this);
    leftCard->setMinimumWidth(minCardWidth);
    // 在高DPI下不设置最大宽度限制，让内容自适应
    if (dpiScale <= 1.5) {
        leftCard->setMaximumWidth(maxCardWidth);
    }
    // 使用 Fixed 策略确保宽度稳定，但允许垂直扩展
    leftCard->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    leftCard->setElevation(1);
    
    // 在卡片内创建一个容器 widget 来放置布局（重要！避免直接在 QWidget 上设置复杂布局）
    QWidget* leftContentWidget = new QWidget(leftCard);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftContentWidget);
    leftLayout->setSpacing(spacing12);
    leftLayout->setContentsMargins(contentPadding, contentPadding, contentPadding, contentPadding);
    
    // 在卡片上设置布局以容纳内容 widget
    QVBoxLayout* leftCardLayout = new QVBoxLayout(leftCard);
    leftCardLayout->setContentsMargins(0, 0, 0, 0);
    leftCardLayout->setSpacing(0);
    leftCardLayout->addWidget(leftContentWidget);
    
    // 标题 - 添加渐变效果（根据DPI缩放字体大小）
    // 高DPI下使用较小的字体缩放比例，避免文字过大
    int titleFontSize;
    if (dpiScale > 1.5) {
        titleFontSize = qRound(20 * dpiScale);  // 从22减小到20
    } else {
        titleFontSize = qRound(22 * dpiScale);
    }
    ElaText* titleText = new ElaText("学生信息录入", leftContentWidget);
    titleText->setTextPixelSize(titleFontSize);
    // 设置自动换行，避免文字超出
    titleText->setWordWrap(false);
    titleText->setStyleSheet(R"(
        ElaText {
            color: #667eea;
            font-weight: bold;
            padding-bottom: 10px;
        }
    )");
    leftLayout->addWidget(titleText);
    
    // 添加分隔线
    QWidget* separatorLine = new QWidget(leftContentWidget);
    separatorLine->setFixedHeight(separatorHeight);
    separatorLine->setStyleSheet(R"(
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                   stop:0 #667eea, stop:1 transparent);
    )");
    leftLayout->addWidget(separatorLine);
    
    leftLayout->addSpacing(spacing15);
    
    // 计算标签高度和字体大小（根据DPI缩放）
    int labelHeight = qRound(20 * dpiScale);
    int labelFontSize;
    if (dpiScale > 1.5) {
        labelFontSize = qRound(13 * dpiScale);  // 高DPI下从15减小到13
    } else {
        labelFontSize = qRound(15 * dpiScale);
    }
    
    // 学号输入
    ElaText* labelId = new ElaText("学号", leftContentWidget);
    labelId->setTextPixelSize(labelFontSize);
    labelId->setMinimumHeight(labelHeight);
    lineEditId = new ElaLineEdit(leftContentWidget);
    lineEditId->setPlaceholderText("请输入学号");
    lineEditId->setFixedHeight(inputHeight);
    
    // 姓名输入
    ElaText* labelName = new ElaText("姓名", leftContentWidget);
    labelName->setTextPixelSize(labelFontSize);
    labelName->setMinimumHeight(labelHeight);
    lineEditName = new ElaLineEdit(leftContentWidget);
    lineEditName->setPlaceholderText("请输入姓名");
    lineEditName->setFixedHeight(inputHeight);
    
    // 性别选择
    ElaText* labelGender = new ElaText("性别", leftContentWidget);
    labelGender->setTextPixelSize(labelFontSize);
    labelGender->setMinimumHeight(labelHeight);
    comboBoxGender = new ElaComboBox(leftContentWidget);
    comboBoxGender->addItem("男");
    comboBoxGender->addItem("女");
    comboBoxGender->setFixedHeight(inputHeight);
    
    // 年龄输入
    ElaText* labelAge = new ElaText("年龄", leftContentWidget);
    labelAge->setTextPixelSize(labelFontSize);
    labelAge->setMinimumHeight(labelHeight);
    spinBoxAge = new ElaSpinBox(leftContentWidget);
    spinBoxAge->setRange(15, 100);
    spinBoxAge->setValue(18);
    spinBoxAge->setFixedHeight(inputHeight);
    
    // 专业选择（改为下拉框）
    ElaText* labelMajor = new ElaText("专业", leftContentWidget);
    labelMajor->setTextPixelSize(labelFontSize);
    labelMajor->setMinimumHeight(labelHeight);
    comboBoxMajor = new ElaComboBox(leftContentWidget);
    comboBoxMajor->addItem("计算机科学");
    comboBoxMajor->addItem("软件工程");
    comboBoxMajor->addItem("信息安全");
    comboBoxMajor->addItem("数据科学");
    comboBoxMajor->addItem("人工智能");
    comboBoxMajor->addItem("网络工程");
    comboBoxMajor->addItem("物联网工程");
    comboBoxMajor->addItem("电子信息工程");
    comboBoxMajor->addItem("通信工程");
    comboBoxMajor->addItem("自动化");
    comboBoxMajor->setFixedHeight(inputHeight);
    
    // 添加到布局
    leftLayout->addWidget(labelId);
    leftLayout->addWidget(lineEditId);
    leftLayout->addWidget(labelName);
    leftLayout->addWidget(lineEditName);
    leftLayout->addWidget(labelGender);
    leftLayout->addWidget(comboBoxGender);
    leftLayout->addWidget(labelAge);
    leftLayout->addWidget(spinBoxAge);
    leftLayout->addWidget(labelMajor);
    leftLayout->addWidget(comboBoxMajor);
    
    leftLayout->addSpacing(spacing20);
    
    // 操作按钮 - 使用渐变色和阴影效果
    btnAdd = new ElaPushButton("添加学生", leftContentWidget);
    btnAdd->setFixedHeight(buttonHeight);
    btnAdd->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #667eea, stop:1 #764ba2);
            color: white;
            font-size: 14px;
            font-weight: bold;
            border-radius: 10px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #7a8df5, stop:1 #8a5bb8);
        }
    )");
    QGraphicsDropShadowEffect *addShadow = new QGraphicsDropShadowEffect(btnAdd);
    addShadow->setBlurRadius(20);
    addShadow->setColor(QColor(102, 126, 234, 100));
    addShadow->setOffset(0, 4);
    btnAdd->setGraphicsEffect(addShadow);
    connect(btnAdd, &ElaPushButton::clicked, this, &MainWindow::onAddStudent);
    
    btnModify = new ElaPushButton("修改学生", leftContentWidget);
    btnModify->setFixedHeight(buttonHeight);
    btnModify->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #43e97b, stop:1 #38f9d7);
            color: white;
            font-size: 14px;
            font-weight: bold;
            border-radius: 10px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #55f58d, stop:1 #4affea);
        }
    )");
    QGraphicsDropShadowEffect *modifyShadow = new QGraphicsDropShadowEffect(btnModify);
    modifyShadow->setBlurRadius(20);
    modifyShadow->setColor(QColor(67, 233, 123, 100));
    modifyShadow->setOffset(0, 4);
    btnModify->setGraphicsEffect(modifyShadow);
    connect(btnModify, &ElaPushButton::clicked, this, &MainWindow::onModifyStudent);
    
    btnDelete = new ElaPushButton("删除学生", leftContentWidget);
    btnDelete->setFixedHeight(buttonHeight);
    btnDelete->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #f093fb, stop:1 #f5576c);
            color: white;
            font-size: 14px;
            font-weight: bold;
            border-radius: 10px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #ffa4ff, stop:1 #ff6a7f);
        }
    )");
    QGraphicsDropShadowEffect *deleteShadow = new QGraphicsDropShadowEffect(btnDelete);
    deleteShadow->setBlurRadius(20);
    deleteShadow->setColor(QColor(240, 147, 251, 100));
    deleteShadow->setOffset(0, 4);
    btnDelete->setGraphicsEffect(deleteShadow);
    connect(btnDelete, &ElaPushButton::clicked, this, &MainWindow::onDeleteStudent);
    
    btnClear = new ElaPushButton("清空输入", leftContentWidget);
    btnClear->setFixedHeight(buttonHeight);
    btnClear->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #a8b8d8, stop:1 #b8c8e8);
            color: white;
            font-size: 14px;
            font-weight: bold;
            border-radius: 10px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #b8c8e8, stop:1 #c8d8f8);
        }
    )");
    QGraphicsDropShadowEffect *clearShadow = new QGraphicsDropShadowEffect(btnClear);
    clearShadow->setBlurRadius(20);
    clearShadow->setColor(QColor(168, 184, 216, 100));
    clearShadow->setOffset(0, 4);
    btnClear->setGraphicsEffect(clearShadow);
    connect(btnClear, &ElaPushButton::clicked, this, &MainWindow::onClearInput);
    
    leftLayout->addWidget(btnAdd);
    leftLayout->addWidget(btnModify);
    leftLayout->addWidget(btnDelete);
    leftLayout->addWidget(btnClear);
    
    leftLayout->addSpacing(spacing15);
    
    // 分隔线和批量操作标题
    QWidget* batchSeparator = new QWidget(leftContentWidget);
    batchSeparator->setFixedHeight(separatorHeight);
    batchSeparator->setStyleSheet(R"(
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                   stop:0 transparent, stop:0.5 #667eea, stop:1 transparent);
    )");
    leftLayout->addWidget(batchSeparator);
    
    ElaText* batchTitle = new ElaText("批量操作", leftContentWidget);
    batchTitle->setTextPixelSize(labelFontSize);
    batchTitle->setStyleSheet("ElaText { color: #667eea; font-weight: bold; }");
    leftLayout->addWidget(batchTitle);
    
    // 批量删除按钮
    btnBatchDelete = new ElaPushButton("批量删除", leftContentWidget);
    btnBatchDelete->setFixedHeight(buttonHeight);
    btnBatchDelete->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #eb3349, stop:1 #f45c43);
            color: white;
            font-size: 14px;
            font-weight: bold;
            border-radius: 10px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #ff4559, stop:1 #ff6c53);
        }
    )");
    QGraphicsDropShadowEffect *batchDeleteShadow = new QGraphicsDropShadowEffect(btnBatchDelete);
    batchDeleteShadow->setBlurRadius(20);
    batchDeleteShadow->setColor(QColor(235, 51, 73, 100));
    batchDeleteShadow->setOffset(0, 4);
    btnBatchDelete->setGraphicsEffect(batchDeleteShadow);
    connect(btnBatchDelete, &ElaPushButton::clicked, this, &MainWindow::onBatchDelete);
    leftLayout->addWidget(btnBatchDelete);
    
    // 批量修改专业按钮
    btnBatchModify = new ElaPushButton("批量改专业", leftContentWidget);
    btnBatchModify->setFixedHeight(buttonHeight);
    btnBatchModify->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #f857a6, stop:1 #ff5858);
            color: white;
            font-size: 14px;
            font-weight: bold;
            border-radius: 10px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #ff67b6, stop:1 #ff6868);
        }
    )");
    QGraphicsDropShadowEffect *batchModifyShadow = new QGraphicsDropShadowEffect(btnBatchModify);
    batchModifyShadow->setBlurRadius(20);
    batchModifyShadow->setColor(QColor(248, 87, 166, 100));
    batchModifyShadow->setOffset(0, 4);
    btnBatchModify->setGraphicsEffect(batchModifyShadow);
    connect(btnBatchModify, &ElaPushButton::clicked, this, &MainWindow::onBatchModifyMajor);
    leftLayout->addWidget(btnBatchModify);
    
    leftLayout->addStretch();
    
    // ===== 右侧：表格显示区域 =====
    // 创建扁平卡片容器
    FlatCardWidget* rightCard = new FlatCardWidget(this);
    rightCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightCard->setElevation(1);
    
    // 在卡片内创建一个容器 widget 来放置布局
    QWidget* rightContentWidget = new QWidget(rightCard);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightContentWidget);
    rightLayout->setSpacing(spacing15);
    rightLayout->setContentsMargins(contentPadding, contentPadding, contentPadding, contentPadding);
    
    // 在卡片上设置布局以容纳内容 widget
    QVBoxLayout* rightCardLayout = new QVBoxLayout(rightCard);
    rightCardLayout->setContentsMargins(0, 0, 0, 0);
    rightCardLayout->setSpacing(0);
    rightCardLayout->addWidget(rightContentWidget);
    
    // 标题和搜索栏
    QHBoxLayout* topLayout = new QHBoxLayout();
    
    ElaText* tableTitle = new ElaText("学生信息列表", rightContentWidget);
    tableTitle->setTextPixelSize(titleFontSize);
    tableTitle->setStyleSheet(R"(
        ElaText {
            color: #667eea;
            font-weight: bold;
        }
    )");
    
    // 计算按钮宽度（根据DPI缩放）
    int searchBoxWidth = qRound(300 * dpiScale);
    int buttonWidth = qRound(95 * dpiScale);
    
    lineEditSearch = new ElaLineEdit(rightContentWidget);
    lineEditSearch->setPlaceholderText("搜索学号或姓名...");
    lineEditSearch->setMaximumWidth(searchBoxWidth);
    lineEditSearch->setFixedHeight(inputHeight);
    connect(lineEditSearch, &ElaLineEdit::returnPressed, this, &MainWindow::onSearchStudent);
    
    btnSearch = new ElaPushButton("搜索", rightContentWidget);
    btnSearch->setFixedSize(buttonWidth, inputHeight);
    btnSearch->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #4facfe, stop:1 #00f2fe);
            color: white;
            font-size: 13px;
            font-weight: bold;
            border-radius: 8px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #5fbdff, stop:1 #20ffff);
        }
    )");
    QGraphicsDropShadowEffect *searchShadow = new QGraphicsDropShadowEffect(btnSearch);
    searchShadow->setBlurRadius(15);
    searchShadow->setColor(QColor(79, 172, 254, 100));
    searchShadow->setOffset(0, 3);
    btnSearch->setGraphicsEffect(searchShadow);
    connect(btnSearch, &ElaPushButton::clicked, this, &MainWindow::onSearchStudent);
    
    btnRefresh = new ElaPushButton("刷新", rightContentWidget);
    btnRefresh->setFixedSize(buttonWidth, inputHeight);
    btnRefresh->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #fa709a, stop:1 #fee140);
            color: white;
            font-size: 13px;
            font-weight: bold;
            border-radius: 8px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #ff82ac, stop:1 #fff160);
        }
    )");
    QGraphicsDropShadowEffect *refreshShadow = new QGraphicsDropShadowEffect(btnRefresh);
    refreshShadow->setBlurRadius(15);
    refreshShadow->setColor(QColor(250, 112, 154, 100));
    refreshShadow->setOffset(0, 3);
    btnRefresh->setGraphicsEffect(refreshShadow);
    connect(btnRefresh, &ElaPushButton::clicked, this, &MainWindow::onRefreshTable);
    
    btnSort = new ElaPushButton("排序", rightContentWidget);
    btnSort->setFixedSize(buttonWidth, inputHeight);
    btnSort->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #667eea, stop:1 #764ba2);
            color: white;
            font-size: 13px;
            font-weight: bold;
            border-radius: 8px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #7a8df5, stop:1 #8a5bb8);
        }
    )");
    QGraphicsDropShadowEffect *sortShadow = new QGraphicsDropShadowEffect(btnSort);
    sortShadow->setBlurRadius(15);
    sortShadow->setColor(QColor(102, 126, 234, 100));
    sortShadow->setOffset(0, 3);
    btnSort->setGraphicsEffect(sortShadow);
    connect(btnSort, &ElaPushButton::clicked, this, &MainWindow::onSortById);
    
    btnExport = new ElaPushButton("导出", rightContentWidget);
    btnExport->setFixedSize(buttonWidth, inputHeight);
    btnExport->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #38ef7d, stop:1 #11998e);
            color: white;
            font-size: 13px;
            font-weight: bold;
            border-radius: 8px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #48ff8d, stop:1 #21aa9e);
        }
    )");
    QGraphicsDropShadowEffect *exportShadow = new QGraphicsDropShadowEffect(btnExport);
    exportShadow->setBlurRadius(15);
    exportShadow->setColor(QColor(56, 239, 125, 100));
    exportShadow->setOffset(0, 3);
    btnExport->setGraphicsEffect(exportShadow);
    connect(btnExport, &ElaPushButton::clicked, this, &MainWindow::onExportCSV);
    
    btnImport = new ElaPushButton("导入", rightContentWidget);
    btnImport->setFixedSize(buttonWidth, inputHeight);
    btnImport->setStyleSheet(R"(
        ElaPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #fc466b, stop:1 #3f5efb);
            color: white;
            font-size: 13px;
            font-weight: bold;
            border-radius: 8px;
        }
        ElaPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #ff567b, stop:1 #4f6eff);
        }
    )");
    QGraphicsDropShadowEffect *importShadow = new QGraphicsDropShadowEffect(btnImport);
    importShadow->setBlurRadius(15);
    importShadow->setColor(QColor(252, 70, 107, 100));
    importShadow->setOffset(0, 3);
    btnImport->setGraphicsEffect(importShadow);
    connect(btnImport, &ElaPushButton::clicked, this, &MainWindow::onImportCSV);
    
    topLayout->addWidget(tableTitle);
    topLayout->addStretch();
    topLayout->addWidget(lineEditSearch);
    topLayout->addWidget(btnSearch);
    topLayout->addWidget(btnRefresh);
    topLayout->addWidget(btnSort);
    topLayout->addWidget(btnExport);
    topLayout->addWidget(btnImport);
    
    // 表格
    tableWidget = new ElaTableView(rightContentWidget);
    
    rightLayout->addLayout(topLayout);
    rightLayout->addWidget(tableWidget);
    
    // ===== 创建卡片视图（用于教师和学生） =====
    m_cardViewWidget = new QWidget(centralWidget);
    QVBoxLayout* cardViewLayout = new QVBoxLayout(m_cardViewWidget);
    cardViewLayout->setContentsMargins(0, 0, 0, 0);
    cardViewLayout->setSpacing(0);
    
    // 创建滚动区域
    m_cardScrollArea = new ElaScrollArea(m_cardViewWidget);
    m_cardScrollArea->setWidgetResizable(true);
    m_cardScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // 创建卡片容器
    m_cardContainer = new QWidget();
    // 学生角色使用垂直布局，教师角色使用网格布局
    m_cardLayout = new QGridLayout(m_cardContainer);
    m_cardLayout->setSpacing(25);  // 增加间距，防止展开时重叠
    m_cardLayout->setContentsMargins(25, 25, 25, 25);
    m_cardLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);  // 水平居中
    
    m_cardScrollArea->setWidget(m_cardContainer);
    cardViewLayout->addWidget(m_cardScrollArea);
    m_cardViewWidget->setVisible(false);  // 初始隐藏
    
    // 将左右卡片添加到主布局（使用更合理的比例）
    mainLayout->addWidget(leftCard, 2);  // 左侧占 2 份
    m_leftCardWidget = leftCard;  // 保存引用，用于学生角色隐藏
    mainLayout->addWidget(rightCard, 5);  // 右侧占 5 份（管理员使用表格）
    m_rightCardWidget = rightCard;  // 保存引用，用于学生角色隐藏
    mainLayout->addWidget(m_cardViewWidget, 5);  // 卡片视图（教师和学生使用）
    
    qDebug() << "centralWidget 大小:" << centralWidget->size();
    qDebug() << "准备添加页面到窗口";
    
    // 方式1：不使用导航栏，直接设置中心部件（推荐用于单页面应用）
    setIsNavigationBarEnable(false);  // 禁用导航栏
    setCentralWidget(centralWidget);
    
    // 方式2：使用导航栏（如果需要多个页面）
    // addPageNode("主页", centralWidget, ElaIconType::House);
    
    qDebug() << "initUI 完成";
}

void MainWindow::initTable()
{
    // 创建表格模型
    tableModel = new QStandardItemModel(this);
    tableModel->setColumnCount(5);
    
    // 设置表头
    QStringList headers;
    headers << "学号" << "姓名" << "性别" << "年龄" << "专业";
    tableModel->setHorizontalHeaderLabels(headers);
    
    // 设置模型到视图
    tableWidget->setModel(tableModel);
    
    // 设置表格属性
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->verticalHeader()->setVisible(false);
    
    // 设置列宽为可交互模式，允许用户拖拽调整列宽
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    
    // 设置初始列宽（可根据需要调整）
    tableWidget->setColumnWidth(0, 120);  // 学号
    tableWidget->setColumnWidth(1, 100);  // 姓名
    tableWidget->setColumnWidth(2, 80);   // 性别
    tableWidget->setColumnWidth(3, 80);   // 年龄
    tableWidget->setColumnWidth(4, 150);  // 专业
    
    // 启用表头的拖拽调整功能
    tableWidget->horizontalHeader()->setStretchLastSection(true);  // 最后一列自动填充剩余空间
    
    // 连接单击信号
    connect(tableWidget, &ElaTableView::clicked, [this](const QModelIndex &index){
        onTableItemClicked(index.row(), index.column());
    });
}

void MainWindow::updateTable()
{
    tableModel->removeRows(0, tableModel->rowCount());
    
    int displayCount = 0;
    
    for (int i = 0; i < students.size(); ++i) {
        const Student &student = students[i];
        
        // 如果是学生角色，只显示自己的信息
        if (currentUserRole == UserRole::Student) {
            if (!currentUserStudentId.isEmpty() && student.getId() != currentUserStudentId) {
                continue;  // 跳过不是自己的记录
            }
        }
        // 如果是教师角色，只显示自己专业的学生
        else if (currentUserRole == UserRole::Teacher) {
            if (!currentUserMajor.isEmpty() && student.getMajor() != currentUserMajor) {
                continue;  // 跳过不是自己专业的学生
            }
        }
        
        QList<QStandardItem*> items;
        items << new QStandardItem(student.getId());
        items << new QStandardItem(student.getName());
        items << new QStandardItem(student.getGender());
        items << new QStandardItem(QString::number(student.getAge()));
        items << new QStandardItem(student.getMajor());
        
        // 设置项目不可编辑
        for (auto item : items) {
            item->setEditable(false);
        }
        
        tableModel->appendRow(items);
        displayCount++;
    }
    
    // 如果是学生角色且没有找到自己的记录，显示提示
    if (currentUserRole == UserRole::Student && !currentUserStudentId.isEmpty() && displayCount == 0) {
        QList<QStandardItem*> items;
        items << new QStandardItem("未找到");
        items << new QStandardItem("您的学生信息");
        items << new QStandardItem("-");
        items << new QStandardItem("-");
        items << new QStandardItem("-");
        
        for (auto item : items) {
            item->setEditable(false);
        }
        
        tableModel->appendRow(items);
    }
    
    // 如果是教师或学生角色，同时更新卡片视图
    if (currentUserRole == UserRole::Teacher || currentUserRole == UserRole::Student) {
        updateCardView();
    }
}

void MainWindow::updateCardView()
{
    if (!m_cardLayout || !m_cardContainer) {
        return;
    }
    
    // 清除布局中的所有widget（包括学生角色的大卡片和教师角色的小卡片）
    while (QLayoutItem* item = m_cardLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
    // m_studentCards.clear();
    
    // 收集要显示的学生
    QVector<const Student*> displayStudents;
    for (int i = 0; i < students.size(); ++i) {
        const Student &student = students[i];
        
        // 如果是学生角色，只显示自己的信息
        if (currentUserRole == UserRole::Student) {
            if (!currentUserStudentId.isEmpty() && student.getId() != currentUserStudentId) {
                continue;
            }
        }
        // 如果是教师角色，只显示自己专业的学生
        else if (currentUserRole == UserRole::Teacher) {
            if (!currentUserMajor.isEmpty() && student.getMajor() != currentUserMajor) {
                continue;
            }
        }
        
        displayStudents.append(&student);
    }
    
    bool isStudent = (currentUserRole == UserRole::Student);
    int colCount = isStudent ? 1 : 3;  // 学生角色每行1个，教师角色每行3个
    
    // 学生角色：使用全新的现代化布局设计
    if (isStudent && !displayStudents.isEmpty()) {
        const Student* student = displayStudents[0];
        
        // 创建主卡片容器
        FlatCardWidget* mainCard = new FlatCardWidget(m_cardContainer);
        mainCard->setElevation(3);
        mainCard->setMinimumHeight(500);
        
        QVBoxLayout* mainCardLayout = new QVBoxLayout(mainCard);
        mainCardLayout->setContentsMargins(50, 40, 50, 40);
        mainCardLayout->setSpacing(30);
        
        // === 顶部区域：头像和姓名 ===
        QHBoxLayout* headerLayout = new QHBoxLayout();
        headerLayout->setSpacing(30);
        
        // 创建大尺寸头像（150x150） - 使用现代动画头像
        ModernAvatar* avatarWidget = new ModernAvatar(mainCard);
        avatarWidget->setGender(student->getGender());
        
        // 姓名区域
        QVBoxLayout* nameLayout = new QVBoxLayout();
        nameLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        nameLayout->setSpacing(10);
        
        ElaText* nameText = new ElaText(student->getName(), mainCard);
        nameText->setTextPixelSize(36);
        nameText->setStyleSheet("font-weight: bold; color: #667eea;");
        
        ElaText* idText = new ElaText(QString("学号：%1").arg(student->getId()), mainCard);
        idText->setTextPixelSize(18);
        idText->setStyleSheet("color: #95A5A6;");
        
        nameLayout->addWidget(nameText);
        nameLayout->addWidget(idText);
        nameLayout->addStretch();
        
        headerLayout->addWidget(avatarWidget);
        headerLayout->addLayout(nameLayout);
        headerLayout->addStretch();
        
        mainCardLayout->addLayout(headerLayout);
        
        // === 分隔线 ===
        QWidget* separator = new QWidget(mainCard);
        separator->setFixedHeight(1);
        separator->setStyleSheet("background-color: #E0E0E0;");
        mainCardLayout->addWidget(separator);
        
        // === 信息区域：使用网格布局展示信息块 ===
        QGridLayout* infoGridLayout = new QGridLayout();
        infoGridLayout->setSpacing(25);
        infoGridLayout->setContentsMargins(0, 20, 0, 20);
        
        // 创建信息块函数
        auto createInfoBlock = [mainCard](const QString& label, const QString& value, const QString& iconColor = "#667eea") -> QWidget* {
            FlatCardWidget* infoCard = new FlatCardWidget(mainCard);
            infoCard->setElevation(1);
            infoCard->setFixedHeight(100);
            
            QVBoxLayout* infoLayout = new QVBoxLayout(infoCard);
            infoLayout->setContentsMargins(20, 15, 20, 15);
            infoLayout->setSpacing(8);
            
            ElaText* labelText = new ElaText(label, infoCard);
            labelText->setTextPixelSize(14);
            labelText->setStyleSheet(QString("color: %1; font-weight: bold;").arg(iconColor));
            
            ElaText* valueText = new ElaText(value, infoCard);
            valueText->setTextPixelSize(20);
            valueText->setStyleSheet("font-weight: bold;");
            
            infoLayout->addWidget(labelText);
            infoLayout->addWidget(valueText);
            infoLayout->addStretch();
            
            return infoCard;
        };
        
        // 性别信息块
        QWidget* genderBlock = createInfoBlock("性别", student->getGender(), "#f093fb");
        infoGridLayout->addWidget(genderBlock, 0, 0);
        
        // 年龄信息块
        QWidget* ageBlock = createInfoBlock("年龄", QString("%1岁").arg(student->getAge()), "#4facfe");
        infoGridLayout->addWidget(ageBlock, 0, 1);
        
        // 专业信息块（占据两列）
        FlatCardWidget* majorCard = new FlatCardWidget(mainCard);
        majorCard->setElevation(1);
        majorCard->setFixedHeight(100);
        QVBoxLayout* majorLayout = new QVBoxLayout(majorCard);
        majorLayout->setContentsMargins(20, 15, 20, 15);
        majorLayout->setSpacing(8);
        ElaText* majorLabel = new ElaText("专业", majorCard);
        majorLabel->setTextPixelSize(14);
        majorLabel->setStyleSheet("color: #43e97b; font-weight: bold;");
        ElaText* majorValue = new ElaText(student->getMajor(), majorCard);
        majorValue->setTextPixelSize(20);
        majorValue->setStyleSheet("font-weight: bold;");
        majorLayout->addWidget(majorLabel);
        majorLayout->addWidget(majorValue);
        majorLayout->addStretch();
        infoGridLayout->addWidget(majorCard, 0, 2, 1, 2);
        
        mainCardLayout->addLayout(infoGridLayout);
        
        mainCardLayout->addStretch();
        
        // 添加到网格布局（学生角色居中显示）
        m_cardLayout->addWidget(mainCard, 0, 0, 1, colCount, Qt::AlignCenter);
        
        // 保存引用（虽然类型不同，但我们需要存储）
        // 这里我们暂时不添加到m_studentCards，因为类型不匹配
    }
    // 教师角色：使用现代化的卡片列表布局
    else if (!isStudent) {
        for (int i = 0; i < displayStudents.size(); ++i) {
            const Student* student = displayStudents[i];
            
            // 创建扁平卡片容器
            FlatCardWidget* card = new FlatCardWidget(m_cardContainer);
            card->setFixedSize(320, 130);
            card->setElevation(1);
            
            QHBoxLayout* cardLayout = new QHBoxLayout(card);
            cardLayout->setContentsMargins(15, 15, 15, 15);
            cardLayout->setSpacing(15);
            
            // 动态头像
            ModernAvatar* avatarWidget = new ModernAvatar(card);
            avatarWidget->setFixedSize(80, 80);
            avatarWidget->setGender(student->getGender());
            
            // 信息布局
            QVBoxLayout* infoLayout = new QVBoxLayout();
            infoLayout->setSpacing(5);
            infoLayout->setAlignment(Qt::AlignVCenter);
            
            // 姓名
            ElaText* nameText = new ElaText(student->getName(), card);
            nameText->setTextPixelSize(18);
            nameText->setStyleSheet("font-weight: bold; color: #333;");
            
            // 学号
            ElaText* idText = new ElaText(QString("学号：%1").arg(student->getId()), card);
            idText->setTextPixelSize(13);
            idText->setStyleSheet("color: #888;");
            
            // 专业和性别
            ElaText* detailsText = new ElaText(QString("%1 | %2 | %3岁").arg(student->getMajor()).arg(student->getGender()).arg(student->getAge()), card);
            detailsText->setTextPixelSize(13);
            detailsText->setStyleSheet("color: #666;");
            
            infoLayout->addWidget(nameText);
            infoLayout->addWidget(idText);
            infoLayout->addWidget(detailsText);
            infoLayout->addStretch();
            
            cardLayout->addWidget(avatarWidget);
            cardLayout->addLayout(infoLayout);
            
            int row = i / colCount;
            int col = i % colCount;
            m_cardLayout->addWidget(card, row, col);
        }
    }
    
    // 如果没有学生，显示提示
    if (displayStudents.isEmpty()) {
        ElaText* emptyText = new ElaText("暂无学生信息", m_cardContainer);
        emptyText->setTextPixelSize(18);
        emptyText->setStyleSheet("color: #95A5A6;");
        m_cardLayout->addWidget(emptyText, 0, 0, 1, colCount, Qt::AlignCenter);
    }
}

void MainWindow::clearInputFields()
{
    lineEditId->clear();
    lineEditName->clear();
    comboBoxGender->setCurrentIndex(0);
    spinBoxAge->setValue(18);
    comboBoxMajor->setCurrentIndex(0);
    currentEditingStudentId.clear();
}

bool MainWindow::validateInput()
{
    QString id = lineEditId->text().trimmed();
    
    // 学号不能为空
    if (id.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "输入错误", "学号不能为空！", 2000, this);
        return false;
    }
    
    // 学号格式验证：必须是数字，长度7位
    QRegularExpression idPattern("^\\d{7}$");
    if (!idPattern.match(id).hasMatch()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "格式错误", 
                              "学号格式不正确！\n学号必须是7位数字，例如：2021001", 3000, this);
        return false;
    }
    
    // 姓名不能为空
    if (lineEditName->text().trimmed().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "输入错误", "姓名不能为空！", 2000, this);
        return false;
    }
    
    // 年龄范围验证（15-100）已在SpinBox中限制
    int age = spinBoxAge->value();
    if (age < 15 || age > 100) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "输入错误", 
                              "年龄范围应在15-100之间！", 2000, this);
        return false;
    }
    
    return true;
}

int MainWindow::findStudentById(const QString &id)
{
    for (int i = 0; i < students.size(); ++i) {
        if (students[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

Student* MainWindow::getStudentById(const QString &studentId)
{
    int index = findStudentById(studentId);
    if (index != -1) {
        return &students[index];
    }
    return nullptr;
}

void MainWindow::onAddStudent()
{
    if (!validateInput()) {
        return;
    }
    
    QString id = lineEditId->text().trimmed();
    
    // 检查学号是否已存在
    if (findStudentById(id) != -1) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "添加失败", "该学号已存在！", 2000, this);
        return;
    }
    
    QString name = lineEditName->text().trimmed();
    QString gender = comboBoxGender->currentText();
    int age = spinBoxAge->value();
    QString major = comboBoxMajor->currentText();
    
    // 如果是教师角色，只能添加自己专业的学生
    if (currentUserRole == UserRole::Teacher && !currentUserMajor.isEmpty()) {
        if (major != currentUserMajor) {
            ElaMessageBar::warning(ElaMessageBarType::TopRight, "添加失败", 
                                  QString("您只能添加【%1】专业的学生！").arg(currentUserMajor), 3000, this);
            return;
        }
    }
    
    Student newStudent(id, name, gender, age, major);
    students.append(newStudent);
    
    // 记录日志
    QString currentUser = UserManager::getInstance().hasUser() ? 
                         UserManager::getInstance().getCurrentUser().getUsername() : "未知用户";
    LogManager::getInstance().addLog(OperationType::Add, currentUser, 
                                    QString("添加学生：%1（学号：%2）").arg(name).arg(id));
    
    updateTable();
    clearInputFields();
    
    ElaMessageBar::success(ElaMessageBarType::TopRight, "添加成功", 
                          QString("学生 %1 已成功添加！当前共有 %2 条记录").arg(name).arg(students.size()), 
                          3000, this);
}

void MainWindow::onDeleteStudent()
{
    QString id = lineEditId->text().trimmed();
    
    if (id.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "删除失败", "请输入要删除的学生学号！", 2000, this);
        return;
    }
    
    int index = findStudentById(id);
    if (index == -1) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "删除失败", "未找到该学号的学生！", 2000, this);
        return;
    }
    
    QString studentName = students[index].getName();
    
    // 使用 ElaContentDialog 进行确认
    ElaContentDialog* dialog = new ElaContentDialog(this);
    dialog->setMiddleButtonVisible(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose); // 确保对话框关闭时自动释放内存
    dialog->setWindowTitle("确认删除");
    
    // 创建对话框内容
    QWidget* dialogWidget = new QWidget(this);
    QVBoxLayout* dialogLayout = new QVBoxLayout(dialogWidget);
    ElaText* confirmText = new ElaText(QString("确定要删除学生 %1（学号：%2）的信息吗？").arg(studentName).arg(id), this);
    confirmText->setTextPixelSize(15);
    dialogLayout->addWidget(confirmText);
    dialog->setCentralWidget(dialogWidget);
    
    dialog->setLeftButtonText("取消");
    dialog->setRightButtonText("确定");
    
    connect(dialog, &ElaContentDialog::rightButtonClicked, this, [this, id, studentName, dialog](){
        // 再次查找索引，确保安全
        int currentIndex = findStudentById(id);
        if (currentIndex != -1) {
            students.remove(currentIndex);
            
            // 记录日志
            QString currentUser = UserManager::getInstance().hasUser() ? 
                                 UserManager::getInstance().getCurrentUser().getUsername() : "未知用户";
            LogManager::getInstance().addLog(OperationType::Delete, currentUser, 
                                            QString("删除学生：%1（学号：%2）").arg(studentName).arg(id));
            
            updateTable();
            clearInputFields();
            ElaMessageBar::success(ElaMessageBarType::TopRight, "删除成功", 
                                  QString("学生 %1 已删除！当前共有 %2 条记录").arg(studentName).arg(students.size()), 
                                  3000, this);
        } else {
             ElaMessageBar::warning(ElaMessageBarType::TopRight, "删除失败", "学生记录已不存在！", 2000, this);
        }
        dialog->close(); // 关闭对话框（触发 WA_DeleteOnClose）
    });
    
    connect(dialog, &ElaContentDialog::leftButtonClicked, dialog, &ElaContentDialog::close);
    
    dialog->exec();
}

void MainWindow::onModifyStudent()
{
    if (!validateInput()) {
        return;
    }
    
    if (currentEditingStudentId.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "修改失败", "请先在表格中选择要修改的学生！", 2000, this);
        return;
    }
    
    int index = findStudentById(currentEditingStudentId);
    
    if (index == -1) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "修改失败", "未找到该学号的学生！", 2000, this);
        currentEditingStudentId.clear();
        return;
    }
    
    QString newId = lineEditId->text().trimmed();
    QString name = lineEditName->text().trimmed();
    
    // 如果是教师角色，检查要修改的学生是否属于自己专业
    if (currentUserRole == UserRole::Teacher && !currentUserMajor.isEmpty()) {
        if (students[index].getMajor() != currentUserMajor) {
            ElaMessageBar::warning(ElaMessageBarType::TopRight, "修改失败", 
                                  QString("您只能修改【%1】专业的学生！").arg(currentUserMajor), 3000, this);
            return;
        }
    }
    
    // 如果修改了学号，检查新学号是否冲突
    if (newId != currentEditingStudentId) {
        int existingIndex = findStudentById(newId);
        if (existingIndex != -1 && existingIndex != index) {
            ElaMessageBar::warning(ElaMessageBarType::TopRight, "修改失败", "新学号已被其他学生使用！", 2000, this);
            return;
        }
    }
    
    QString gender = comboBoxGender->currentText();
    int age = spinBoxAge->value();
    QString major = comboBoxMajor->currentText();
    
    // 如果是教师角色，不能将学生修改到其他专业
    if (currentUserRole == UserRole::Teacher && !currentUserMajor.isEmpty()) {
        if (major != currentUserMajor) {
            ElaMessageBar::warning(ElaMessageBarType::TopRight, "修改失败", 
                                  QString("您只能将学生设置为【%1】专业！").arg(currentUserMajor), 3000, this);
            return;
        }
    }
    
    // 更新学生信息
    students[index].setId(newId);
    students[index].setName(name);
    students[index].setGender(gender);
    students[index].setAge(age);
    students[index].setMajor(major);
    
    // 记录日志
    QString currentUser = UserManager::getInstance().hasUser() ? 
                         UserManager::getInstance().getCurrentUser().getUsername() : "未知用户";
    LogManager::getInstance().addLog(OperationType::Modify, currentUser, 
                                    QString("修改学生：%1（学号：%2）").arg(name).arg(newId));
    
    currentEditingStudentId = newId;
    
    updateTable();
    ElaMessageBar::success(ElaMessageBarType::TopRight, "修改成功", 
                          QString("学生 %1 的信息已更新！").arg(name), 3000, this);
}

void MainWindow::onSearchStudent()
{
    QString keyword = lineEditSearch->text().trimmed();
    
    if (keyword.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "搜索失败", "请输入搜索关键词！", 2000, this);
        return;
    }
    
    // 清空表格
    tableModel->removeRows(0, tableModel->rowCount());
    
    // 搜索匹配的学生
    int count = 0;
    for (const Student &student : students) {
        // 如果是学生角色，只能搜索自己的信息
        if (currentUserRole == UserRole::Student) {
            if (!currentUserStudentId.isEmpty() && student.getId() != currentUserStudentId) {
                continue;  // 跳过不是自己的记录
            }
        }
        // 如果是教师角色，只能搜索自己专业的学生
        else if (currentUserRole == UserRole::Teacher) {
            if (!currentUserMajor.isEmpty() && student.getMajor() != currentUserMajor) {
                continue;  // 跳过不是自己专业的学生
            }
        }
        
        // 匹配搜索关键词
        if (student.getId().contains(keyword, Qt::CaseInsensitive) ||
            student.getName().contains(keyword, Qt::CaseInsensitive)) {
            
            QList<QStandardItem*> items;
            items << new QStandardItem(student.getId());
            items << new QStandardItem(student.getName());
            items << new QStandardItem(student.getGender());
            items << new QStandardItem(QString::number(student.getAge()));
            items << new QStandardItem(student.getMajor());
            
            for (auto item : items) {
                item->setEditable(false);
            }
            
            tableModel->appendRow(items);
            count++;
        }
    }
    
    if (count == 0) {
        ElaMessageBar::information(ElaMessageBarType::TopRight, "搜索结果", 
                                  QString("未找到包含 \"%1\" 的学生信息！").arg(keyword), 2000, this);
        updateTable();
    } else {
        ElaMessageBar::success(ElaMessageBarType::TopRight, "搜索成功", 
                              QString("找到 %1 条匹配 \"%2\" 的记录").arg(count).arg(keyword), 3000, this);
    }
}

void MainWindow::onClearInput()
{
    clearInputFields();
    ElaMessageBar::information(ElaMessageBarType::TopRight, "已清空", "输入框已清空", 2000, this);
}

void MainWindow::onRefreshTable()
{
    updateTable();
    lineEditSearch->clear();
    ElaMessageBar::success(ElaMessageBarType::TopRight, "已刷新", 
                          QString("列表已刷新 | 当前共有 %1 条记录").arg(students.size()), 2000, this);
}

void MainWindow::onSortById()
{
    std::sort(students.begin(), students.end(), [](const Student &a, const Student &b) {
        return a.getId() < b.getId();
    });
    
    updateTable();
    ElaMessageBar::success(ElaMessageBarType::TopRight, "排序成功", 
                          "学生列表已按学号升序排列！", 2000, this);
}

void MainWindow::onTableItemClicked(int row, int column)
{
    Q_UNUSED(column);
    
    if (row >= 0 && row < tableModel->rowCount()) {
        QStandardItem* item = tableModel->item(row, 0);
        if (!item) return;  // 安全检查
        
        QString id = item->text();
        int index = findStudentById(id);
        
        if (index != -1) {
            const Student &student = students[index];
            
            currentEditingStudentId = student.getId();
            
            lineEditId->setText(student.getId());
            lineEditName->setText(student.getName());
            comboBoxGender->setCurrentText(student.getGender());
            spinBoxAge->setValue(student.getAge());
            comboBoxMajor->setCurrentText(student.getMajor());
            
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "已选择", 
                                      QString("学生：%1（学号：%2）").arg(student.getName()).arg(student.getId()), 
                                      2000, this);
        }
    }
}

void MainWindow::onExportCSV()
{
    if (students.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "导出失败", 
                              "没有学生数据可导出！", 2000, this);
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this, 
                                                    "导出学生数据", 
                                                    "students.csv",
                                                    "CSV文件 (*.csv)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ElaMessageBar::error(ElaMessageBarType::TopRight, "导出失败", 
                            "无法创建文件！", 3000, this);
        return;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);  // 设置UTF-8编码
    
    // 写入表头（带BOM，确保Excel能正确识别UTF-8）
    out << "\xEF\xBB\xBF";  // UTF-8 BOM
    out << "学号,姓名,性别,年龄,专业\n";
    
    // 写入数据
    for (const Student &student : students) {
        out << student.getId() << ","
            << student.getName() << ","
            << student.getGender() << ","
            << student.getAge() << ","
            << student.getMajor() << "\n";
    }
    
    file.close();
    
    // 记录日志
    QString currentUser = UserManager::getInstance().hasUser() ? 
                         UserManager::getInstance().getCurrentUser().getUsername() : "未知用户";
    LogManager::getInstance().addLog(OperationType::Export, currentUser, 
                                    QString("导出 %1 条学生记录到CSV文件").arg(students.size()));
    
    ElaMessageBar::success(ElaMessageBarType::TopRight, "导出成功", 
                          QString("已成功导出 %1 条学生记录到：\n%2")
                          .arg(students.size()).arg(fileName), 4000, this);
}

void MainWindow::onImportCSV()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
                                                    "导入学生数据", 
                                                    "",
                                                    "CSV文件 (*.csv)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ElaMessageBar::error(ElaMessageBarType::TopRight, "导入失败", 
                            "无法打开文件！", 3000, this);
        return;
    }
    
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);  // 设置UTF-8编码
    
    int successCount = 0;
    int skipCount = 0;
    int errorCount = 0;
    QStringList errorMessages;
    
    auto recordError = [&](const QString& message) {
        errorCount++;
        errorMessages << message;
    };
    
    auto processLine = [&](const QString& trimmedLine, int lineNumber) {
        if (trimmedLine.isEmpty()) {
            return;
        }
        
        QStringList fields = trimmedLine.split(',');
        if (fields.size() < 5) {
            recordError(QString("第%1行：字段数量不足").arg(lineNumber));
            return;
        }
        
        QString id = fields[0].trimmed();
        QString name = fields[1].trimmed();
        QString gender = fields[2].trimmed();
        QString ageStr = fields[3].trimmed();
        QString major = fields[4].trimmed();
        
        static const QRegularExpression idPattern("^\\d{7}$");
        if (!idPattern.match(id).hasMatch()) {
            recordError(QString("第%1行：学号格式错误 (%2)").arg(lineNumber).arg(id));
            return;
        }
        
        if (findStudentById(id) != -1) {
            skipCount++;
            return;
        }
        
        bool ok = false;
        int age = ageStr.toInt(&ok);
        if (!ok || age < 15 || age > 100) {
            recordError(QString("第%1行：年龄无效 (%2)").arg(lineNumber).arg(ageStr));
            return;
        }
        
        if (currentUserRole == UserRole::Teacher && !currentUserMajor.isEmpty() && major != currentUserMajor) {
            skipCount++;
            return;
        }
        
        students.append(Student(id, name, gender, age, major));
        successCount++;
    };
    
    int lineNumber = 0;
    bool headerSkipped = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        lineNumber++;
        
        if (lineNumber == 1 && line.startsWith("\xEF\xBB\xBF")) {
            line = line.mid(3);
        }
        
        QString trimmedLine = line.trimmed();
        if (trimmedLine.isEmpty()) {
            continue;
        }
        
        if (!headerSkipped && (trimmedLine.contains("学号") || trimmedLine.contains("姓名"))) {
            headerSkipped = true;
            continue;
        }
        
        processLine(trimmedLine, lineNumber);
    }
    
    file.close();
    
    updateTable();
    
    QString resultMsg = QString("成功导入：%1 条\n").arg(successCount);
    if (skipCount > 0) {
        resultMsg += QString("跳过重复：%1 条\n").arg(skipCount);
    }
    if (errorCount > 0) {
        resultMsg += QString("失败：%1 条\n").arg(errorCount);
        if (!errorMessages.isEmpty() && errorMessages.size() <= 5) {
            resultMsg += "\n错误详情：\n" + errorMessages.join("\n");
        } else if (!errorMessages.isEmpty()) {
            resultMsg += "\n部分错误：\n" + errorMessages.mid(0, 5).join("\n") + "\n...";
        }
    }
    
    if (successCount > 0) {
        QString currentUser = UserManager::getInstance().hasUser() ? 
                             UserManager::getInstance().getCurrentUser().getUsername() : "未知用户";
        LogManager::getInstance().addLog(OperationType::Import, currentUser, 
                                        QString("从CSV导入 %1 条学生记录").arg(successCount));
    }
    
    if (successCount > 0) {
        ElaMessageBar::success(ElaMessageBarType::TopRight, "导入完成", 
                              resultMsg, 5000, this);
    } else if (errorCount > 0) {
        ElaMessageBar::error(ElaMessageBarType::TopRight, "导入失败", 
                            resultMsg, 5000, this);
    } else {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "导入完成", 
                              "所有数据均已存在，未导入新数据", 3000, this);
    }
}

void MainWindow::onBatchDelete()
{
    // 获取选中的行
    QModelIndexList selectedIndexes = tableWidget->selectionModel()->selectedRows();
    
    if (selectedIndexes.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "批量删除", 
                              "请先选择要删除的学生！\n提示：按住Ctrl或Shift键可多选", 3000, this);
        return;
    }
    
    // 提前收集要删除的学号，避免在回调中使用 ModelIndex
    QStringList idsToDelete;
    for (const QModelIndex &index : selectedIndexes) {
        QString id = tableModel->item(index.row(), 0)->text();
        idsToDelete.append(id);
    }
    
    int count = idsToDelete.size();
    
    // 确认对话框
    ElaContentDialog* dialog = new ElaContentDialog(this);
    dialog->setMiddleButtonVisible(false);
    dialog->setWindowTitle("批量删除确认");
    
    QWidget* dialogWidget = new QWidget(this);
    QVBoxLayout* dialogLayout = new QVBoxLayout(dialogWidget);
    ElaText* confirmText = new ElaText(QString("确定要删除选中的 %1 条学生记录吗？\n此操作不可撤销！").arg(count), this);
    confirmText->setTextPixelSize(15);
    dialogLayout->addWidget(confirmText);
    dialog->setCentralWidget(dialogWidget);
    
    dialog->setLeftButtonText("取消");
    dialog->setRightButtonText("确定删除");
    
    connect(dialog, &ElaContentDialog::rightButtonClicked, this, [this, idsToDelete, dialog](){
        // 从students中删除
        int deletedCount = 0;
        for (const QString &id : idsToDelete) {
            int index = findStudentById(id);
            if (index != -1) {
                students.remove(index);
                deletedCount++;
            }
        }
        
        // 记录日志
        QString currentUser = UserManager::getInstance().hasUser() ? 
                             UserManager::getInstance().getCurrentUser().getUsername() : "未知用户";
        LogManager::getInstance().addLog(OperationType::BatchDelete, currentUser, 
                                        QString("批量删除 %1 条学生记录").arg(deletedCount));
        
        updateTable();
        clearInputFields();
        
        ElaMessageBar::success(ElaMessageBarType::TopRight, "批量删除成功", 
                              QString("已成功删除 %1 条学生记录！\n当前共有 %2 条记录")
                              .arg(deletedCount).arg(students.size()), 3000, this);
        dialog->close(); // 关闭对话框
    });
    
    connect(dialog, &ElaContentDialog::leftButtonClicked, dialog, &ElaContentDialog::close);
    
    dialog->exec();
    dialog->deleteLater();
}

void MainWindow::onBatchModifyMajor()
{
    // 获取选中的行
    QModelIndexList selectedIndexes = tableWidget->selectionModel()->selectedRows();
    
    if (selectedIndexes.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "批量修改", 
                              "请先选择要修改的学生！\n提示：按住Ctrl或Shift键可多选", 3000, this);
        return;
    }
    
    // 提前收集要修改的学号，避免在回调中使用 ModelIndex
    QStringList idsToModify;
    for (const QModelIndex &index : selectedIndexes) {
        QString id = tableModel->item(index.row(), 0)->text();
        idsToModify.append(id);
    }
    
    int count = idsToModify.size();
    
    // 创建对话框选择新专业
    ElaContentDialog* dialog = new ElaContentDialog(this);
    dialog->setMiddleButtonVisible(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose); // 确保自动删除
    dialog->setWindowTitle("批量修改专业");
    dialog->resize(450, 250);
    
    QWidget* dialogWidget = new QWidget(this);
    QVBoxLayout* dialogLayout = new QVBoxLayout(dialogWidget);
    dialogLayout->setSpacing(15);
    dialogLayout->setContentsMargins(20, 20, 20, 20);
    
    ElaText* titleText = new ElaText(QString("为选中的 %1 名学生修改专业：").arg(count), this);
    titleText->setTextPixelSize(16);
    dialogLayout->addWidget(titleText);
    
    // 专业选择下拉框
    ElaComboBox* majorComboBox = new ElaComboBox(this);
    majorComboBox->addItem("计算机科学");
    majorComboBox->addItem("软件工程");
    majorComboBox->addItem("信息安全");
    majorComboBox->addItem("数据科学");
    majorComboBox->addItem("人工智能");
    majorComboBox->addItem("网络工程");
    majorComboBox->addItem("物联网工程");
    majorComboBox->addItem("电子信息工程");
    majorComboBox->addItem("通信工程");
    majorComboBox->addItem("自动化");
    majorComboBox->setFixedHeight(45);
    dialogLayout->addWidget(majorComboBox);
    
    dialog->setCentralWidget(dialogWidget);
    
    dialog->setLeftButtonText("取消");
    dialog->setRightButtonText("确定修改");
    
    connect(dialog, &ElaContentDialog::rightButtonClicked, this, [this, idsToModify, majorComboBox, dialog](){
        QString newMajor = majorComboBox->currentText();
        
        // 如果是教师角色，只能修改为自己专业
        if (currentUserRole == UserRole::Teacher && !currentUserMajor.isEmpty()) {
            if (newMajor != currentUserMajor) {
                ElaMessageBar::warning(ElaMessageBarType::TopRight, "批量修改失败", 
                                      QString("您只能将学生设置为【%1】专业！").arg(currentUserMajor), 3000, this);
                return;
            }
        }
        
        // 批量修改专业
        int modifiedCount = 0;
        for (const QString &id : idsToModify) {
            int index = findStudentById(id);
            if (index != -1) {
                // 如果是教师角色，检查学生是否属于自己专业
                if (currentUserRole == UserRole::Teacher && !currentUserMajor.isEmpty()) {
                    if (students[index].getMajor() != currentUserMajor) {
                        continue;  // 跳过不是自己专业的学生
                    }
                }
                students[index].setMajor(newMajor);
                modifiedCount++;
            }
        }
        
        // 记录日志
        QString currentUser = UserManager::getInstance().hasUser() ? 
                             UserManager::getInstance().getCurrentUser().getUsername() : "未知用户";
        LogManager::getInstance().addLog(OperationType::BatchModify, currentUser, 
                                        QString("批量修改 %1 名学生的专业为：%2").arg(modifiedCount).arg(newMajor));
        
        updateTable();
        
        ElaMessageBar::success(ElaMessageBarType::TopRight, "批量修改成功", 
                              QString("已成功将 %1 名学生的专业修改为：%2")
                              .arg(modifiedCount).arg(newMajor), 4000, this);
        dialog->close(); // 关闭对话框
    });
    
    connect(dialog, &ElaContentDialog::leftButtonClicked, dialog, &ElaContentDialog::close);
    
    dialog->exec();
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    // 直接调用父类的事件过滤器，不做额外拦截
    // 之前的逻辑是多余的，因为无论条件如何都返回父类的结果
    return ElaWindow::eventFilter(watched, event);
}

void MainWindow::onStudentEditInfo()
{
    // 学生角色不允许编辑信息
    if (currentUserRole == UserRole::Student) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "权限不足", "学生角色无法编辑信息！", 2000, this);
        return;
    }
    
    if (currentUserStudentId.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "编辑失败", "未找到您的学生信息！", 2000, this);
        return;
    }
    
    // 查找学生信息
    int index = findStudentById(currentUserStudentId);
    if (index == -1) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "编辑失败", "未找到您的学生信息！", 2000, this);
        return;
    }
    
    const Student& student = students[index];
    
    // 创建编辑对话框
    ElaContentDialog* dialog = new ElaContentDialog(this);
    dialog->setMiddleButtonVisible(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose); // 确保自动删除
    dialog->setWindowTitle("编辑我的信息");
    dialog->resize(500, 400);
    
    QWidget* dialogWidget = new QWidget(dialog);
    QVBoxLayout* dialogLayout = new QVBoxLayout(dialogWidget);
    dialogLayout->setSpacing(20);
    dialogLayout->setContentsMargins(30, 30, 30, 30);
    
    // 学号（不可编辑）
    QHBoxLayout* idLayout = new QHBoxLayout();
    ElaText* idLabel = new ElaText("学号：", dialogWidget);
    idLabel->setTextPixelSize(15);
    idLabel->setFixedWidth(80);
    ElaLineEdit* idInput = new ElaLineEdit(dialogWidget);
    idInput->setText(student.getId());
    idInput->setEnabled(false);  // 学号不可修改
    idInput->setFixedHeight(40);
    idLayout->addWidget(idLabel);
    idLayout->addWidget(idInput);
    dialogLayout->addLayout(idLayout);
    
    // 姓名
    QHBoxLayout* nameLayout = new QHBoxLayout();
    ElaText* nameLabel = new ElaText("姓名：", dialogWidget);
    nameLabel->setTextPixelSize(15);
    nameLabel->setFixedWidth(80);
    ElaLineEdit* nameInput = new ElaLineEdit(dialogWidget);
    nameInput->setText(student.getName());
    nameInput->setFixedHeight(40);
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameInput);
    dialogLayout->addLayout(nameLayout);
    
    // 性别
    QHBoxLayout* genderLayout = new QHBoxLayout();
    ElaText* genderLabel = new ElaText("性别：", dialogWidget);
    genderLabel->setTextPixelSize(15);
    genderLabel->setFixedWidth(80);
    ElaComboBox* genderCombo = new ElaComboBox(dialogWidget);
    genderCombo->addItem("男");
    genderCombo->addItem("女");
    genderCombo->setCurrentText(student.getGender());
    genderCombo->setFixedHeight(40);
    genderLayout->addWidget(genderLabel);
    genderLayout->addWidget(genderCombo);
    dialogLayout->addLayout(genderLayout);
    
    // 年龄
    QHBoxLayout* ageLayout = new QHBoxLayout();
    ElaText* ageLabel = new ElaText("年龄：", dialogWidget);
    ageLabel->setTextPixelSize(15);
    ageLabel->setFixedWidth(80);
    ElaSpinBox* ageSpin = new ElaSpinBox(dialogWidget);
    ageSpin->setMinimum(15);
    ageSpin->setMaximum(100);
    ageSpin->setValue(student.getAge());
    ageSpin->setFixedHeight(40);
    ageLayout->addWidget(ageLabel);
    ageLayout->addWidget(ageSpin);
    dialogLayout->addLayout(ageLayout);
    
    // 专业
    QHBoxLayout* majorLayout = new QHBoxLayout();
    ElaText* majorLabel = new ElaText("专业：", dialogWidget);
    majorLabel->setTextPixelSize(15);
    majorLabel->setFixedWidth(80);
    ElaComboBox* majorCombo = new ElaComboBox(dialogWidget);
    majorCombo->addItem("计算机科学");
    majorCombo->addItem("软件工程");
    majorCombo->addItem("信息安全");
    majorCombo->addItem("数据科学");
    majorCombo->addItem("人工智能");
    majorCombo->addItem("网络工程");
    majorCombo->addItem("物联网工程");
    majorCombo->addItem("电子信息工程");
    majorCombo->addItem("通信工程");
    majorCombo->addItem("自动化");
    majorCombo->setCurrentText(student.getMajor());
    majorCombo->setFixedHeight(40);
    majorLayout->addWidget(majorLabel);
    majorLayout->addWidget(majorCombo);
    dialogLayout->addLayout(majorLayout);
    
    dialog->setCentralWidget(dialogWidget);
    dialog->setLeftButtonText("取消");
    dialog->setRightButtonText("保存");
    
    connect(dialog, &ElaContentDialog::rightButtonClicked, this, [this, index, nameInput, genderCombo, ageSpin, majorCombo](){
        QString name = nameInput->text().trimmed();
        QString gender = genderCombo->currentText();
        int age = ageSpin->value();
        QString major = majorCombo->currentText();
        
        if (name.isEmpty()) {
            ElaMessageBar::warning(ElaMessageBarType::TopRight, "输入错误", "姓名不能为空！", 2000, this);
            return;
        }
        
        // 更新学生信息
        if (index >= 0 && index < students.size()) {
            students[index].setName(name);
            students[index].setGender(gender);
            students[index].setAge(age);
            students[index].setMajor(major);
            
            // 记录日志
            QString currentUser = UserManager::getInstance().hasUser() ? 
                                 UserManager::getInstance().getCurrentUser().getUsername() : "未知用户";
            LogManager::getInstance().addLog(OperationType::Modify, currentUser, 
                                            QString("学生修改自己的信息：%1（学号：%2）").arg(name).arg(students[index].getId()));
            
            // 更新卡片显示
            updateCardView();
            
            ElaMessageBar::success(ElaMessageBarType::TopRight, "修改成功", 
                                  QString("信息已更新！"), 3000, this);
        }
        
    });
    
    dialog->exec();
}
