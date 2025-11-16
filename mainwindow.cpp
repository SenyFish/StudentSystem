#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "flatcardwidget.h"
#include "ElaScrollPage.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaMessageBar.h"
#include "ElaContentDialog.h"
#include "ElaTheme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QDebug>
#include <QIcon>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow 构造函数开始";
    
    ui->setupUi(this);
    
    // ===== 配置 ElaWindow =====
    setWindowTitle("学生信息管理系统");
    setWindowIcon(QIcon(":/icon.ico"));  // 设置窗口图标
    resize(1400, 800);
    
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
    // 添加示例数据
    students.append(Student("2021001", "张三", "男", 20, "计算机科学"));
    students.append(Student("2021002", "李四", "女", 19, "软件工程"));
    students.append(Student("2021003", "王五", "男", 21, "信息安全"));
    students.append(Student("2021004", "赵六", "女", 20, "数据科学"));
    students.append(Student("2021005", "钱七", "男", 22, "人工智能"));
    updateTable();
    
    // 强制立即计算和应用布局，确保窗口在显示前完全准备好
    adjustSize();
    updateGeometry();
    
    qDebug() << "MainWindow 构造函数完成";
    
    // 延迟显示欢迎消息，避免在窗口初始化时显示
    QTimer::singleShot(800, this, [this]() {
        ElaMessageBar::success(ElaMessageBarType::TopRight, "欢迎", 
                              QString("系统已加载 %1 条学生记录").arg(students.size()), 3000, this);
    });
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
    
    // ===== 左侧：输入表单区域 =====
    // 创建扁平卡片容器
    FlatCardWidget* leftCard = new FlatCardWidget(this);
    leftCard->setMaximumWidth(420);
    leftCard->setElevation(1);
    
    QWidget* leftWidget = new QWidget(leftCard);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(16);
    leftLayout->setContentsMargins(30, 30, 30, 30);
    
    // 标题 - 添加渐变效果
    ElaText* titleText = new ElaText("学生信息录入", this);
    titleText->setTextPixelSize(22);
    titleText->setStyleSheet(R"(
        ElaText {
            color: #667eea;
            font-weight: bold;
            padding-bottom: 10px;
        }
    )");
    leftLayout->addWidget(titleText);
    
    // 添加分隔线
    QWidget* separatorLine = new QWidget(this);
    separatorLine->setFixedHeight(2);
    separatorLine->setStyleSheet(R"(
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                   stop:0 #667eea, stop:1 transparent);
    )");
    leftLayout->addWidget(separatorLine);
    
    leftLayout->addSpacing(15);
    
    // 学号输入
    ElaText* labelId = new ElaText("学号", this);
    labelId->setTextPixelSize(15);
    lineEditId = new ElaLineEdit(this);
    lineEditId->setPlaceholderText("请输入学号");
    lineEditId->setFixedHeight(38);
    
    // 姓名输入
    ElaText* labelName = new ElaText("姓名", this);
    labelName->setTextPixelSize(15);
    lineEditName = new ElaLineEdit(this);
    lineEditName->setPlaceholderText("请输入姓名");
    lineEditName->setFixedHeight(38);
    
    // 性别选择
    ElaText* labelGender = new ElaText("性别", this);
    labelGender->setTextPixelSize(15);
    comboBoxGender = new ElaComboBox(this);
    comboBoxGender->addItem("男");
    comboBoxGender->addItem("女");
    comboBoxGender->setFixedHeight(38);
    
    // 年龄输入
    ElaText* labelAge = new ElaText("年龄", this);
    labelAge->setTextPixelSize(15);
    spinBoxAge = new ElaSpinBox(this);
    spinBoxAge->setRange(15, 100);
    spinBoxAge->setValue(18);
    spinBoxAge->setFixedHeight(38);
    
    // 专业输入
    ElaText* labelMajor = new ElaText("专业", this);
    labelMajor->setTextPixelSize(15);
    lineEditMajor = new ElaLineEdit(this);
    lineEditMajor->setPlaceholderText("请输入专业");
    lineEditMajor->setFixedHeight(38);
    
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
    leftLayout->addWidget(lineEditMajor);
    
    leftLayout->addSpacing(20);
    
    // 操作按钮 - 使用渐变色和阴影效果
    btnAdd = new ElaPushButton("添加学生", this);
    btnAdd->setFixedHeight(45);
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
    
    btnModify = new ElaPushButton("修改学生", this);
    btnModify->setFixedHeight(45);
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
    
    btnDelete = new ElaPushButton("删除学生", this);
    btnDelete->setFixedHeight(45);
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
    
    btnClear = new ElaPushButton("清空输入", this);
    btnClear->setFixedHeight(45);
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
    leftLayout->addStretch();
    
    // 将左侧布局设置到卡片中
    QVBoxLayout* leftCardLayout = new QVBoxLayout(leftCard);
    leftCardLayout->setContentsMargins(0, 0, 0, 0);
    leftCardLayout->addWidget(leftWidget);
    
    // ===== 右侧：表格显示区域 =====
    // 创建扁平卡片容器
    FlatCardWidget* rightCard = new FlatCardWidget(this);
    rightCard->setElevation(1);
    
    QWidget* rightWidget = new QWidget(rightCard);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(20);
    rightLayout->setContentsMargins(30, 30, 30, 30);
    
    // 标题和搜索栏
    QHBoxLayout* topLayout = new QHBoxLayout();
    
    ElaText* tableTitle = new ElaText("学生信息列表", this);
    tableTitle->setTextPixelSize(22);
    tableTitle->setStyleSheet(R"(
        ElaText {
            color: #667eea;
            font-weight: bold;
        }
    )");
    
    lineEditSearch = new ElaLineEdit(this);
    lineEditSearch->setPlaceholderText("搜索学号或姓名...");
    lineEditSearch->setMaximumWidth(300);
    lineEditSearch->setFixedHeight(38);
    connect(lineEditSearch, &ElaLineEdit::returnPressed, this, &MainWindow::onSearchStudent);
    
    btnSearch = new ElaPushButton("搜索", this);
    btnSearch->setFixedSize(95, 38);
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
    
    btnRefresh = new ElaPushButton("刷新", this);
    btnRefresh->setFixedSize(95, 38);
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
    
    btnSort = new ElaPushButton("排序", this);
    btnSort->setFixedSize(95, 38);
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
    
    topLayout->addWidget(tableTitle);
    topLayout->addStretch();
    topLayout->addWidget(lineEditSearch);
    topLayout->addWidget(btnSearch);
    topLayout->addWidget(btnRefresh);
    topLayout->addWidget(btnSort);
    
    // 表格
    tableWidget = new ElaTableView(this);
    
    rightLayout->addLayout(topLayout);
    rightLayout->addWidget(tableWidget);
    
    // 将右侧布局设置到卡片中
    QVBoxLayout* rightCardLayout = new QVBoxLayout(rightCard);
    rightCardLayout->setContentsMargins(0, 0, 0, 0);
    rightCardLayout->addWidget(rightWidget);
    
    // 将左右卡片添加到主布局 - 扁平化堆叠
    mainLayout->addWidget(leftCard, 3);
    mainLayout->addWidget(rightCard, 7);
    
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
    
    // 设置表头 - 使用更简洁的文字
    QStringList headers;
    headers << "学号" << "姓名" << "性别" << "年龄" << "专业";
    tableModel->setHorizontalHeaderLabels(headers);
    
    // 设置模型到视图
    tableWidget->setModel(tableModel);
    
    // 设置表格属性
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->verticalHeader()->setVisible(false);
    
    // 设置列宽
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setColumnWidth(0, 120);
    tableWidget->setColumnWidth(1, 100);
    tableWidget->setColumnWidth(2, 80);
    tableWidget->setColumnWidth(3, 80);
    
    // 连接单击信号
    connect(tableWidget, &ElaTableView::clicked, [this](const QModelIndex &index){
        onTableItemClicked(index.row(), index.column());
    });
}

void MainWindow::updateTable()
{
    tableModel->removeRows(0, tableModel->rowCount());
    
    for (int i = 0; i < students.size(); ++i) {
        const Student &student = students[i];
        
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
    }
}

void MainWindow::clearInputFields()
{
    lineEditId->clear();
    lineEditName->clear();
    comboBoxGender->setCurrentIndex(0);
    spinBoxAge->setValue(18);
    lineEditMajor->clear();
    currentEditingStudentId.clear();
}

bool MainWindow::validateInput()
{
    if (lineEditId->text().trimmed().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "输入错误", "学号不能为空！", 2000, this);
        return false;
    }
    
    if (lineEditName->text().trimmed().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "输入错误", "姓名不能为空！", 2000, this);
        return false;
    }
    
    if (lineEditMajor->text().trimmed().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "输入错误", "专业不能为空！", 2000, this);
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
    QString major = lineEditMajor->text().trimmed();
    
    Student newStudent(id, name, gender, age, major);
    students.append(newStudent);
    
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
    
    connect(dialog, &ElaContentDialog::rightButtonClicked, this, [this, index, studentName, id, dialog](){
        students.remove(index);
        updateTable();
        clearInputFields();
        ElaMessageBar::success(ElaMessageBarType::TopRight, "删除成功", 
                              QString("学生 %1 已删除！当前共有 %2 条记录").arg(studentName).arg(students.size()), 
                              3000, this);
        dialog->deleteLater();
    });
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
    
    // 如果修改了学号，检查新学号是否冲突
    if (newId != currentEditingStudentId) {
        int existingIndex = findStudentById(newId);
        if (existingIndex != -1 && existingIndex != index) {
            ElaMessageBar::warning(ElaMessageBarType::TopRight, "修改失败", "新学号已被其他学生使用！", 2000, this);
            return;
        }
    }
    
    // 更新学生信息
    students[index].setId(newId);
    students[index].setName(name);
    students[index].setGender(comboBoxGender->currentText());
    students[index].setAge(spinBoxAge->value());
    students[index].setMajor(lineEditMajor->text().trimmed());
    
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
        QString id = tableModel->item(row, 0)->text();
        int index = findStudentById(id);
        
        if (index != -1) {
            const Student &student = students[index];
            
            currentEditingStudentId = student.getId();
            
            lineEditId->setText(student.getId());
            lineEditName->setText(student.getName());
            comboBoxGender->setCurrentText(student.getGender());
            spinBoxAge->setValue(student.getAge());
            lineEditMajor->setText(student.getMajor());
            
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "已选择", 
                                      QString("学生：%1（学号：%2）").arg(student.getName()).arg(student.getId()), 
                                      2000, this);
        }
    }
}
