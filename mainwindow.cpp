#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "AntMessage.h"
#include "AntMessageManager.h"
#include "DesignSystem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // 设置窗口属性
    setWindowTitle("学生信息管理系统 - Ant Design 风格");
    resize(1400, 800);
    
    // 初始化设计系统
    DesignSystem::instance()->setMainWindow(this);
    
    initUI();
    initTable();
    
    // 添加示例数据
    students.append(Student("2021001", "张三", "男", 20, "计算机科学"));
    students.append(Student("2021002", "李四", "女", 19, "软件工程"));
    students.append(Student("2021003", "王五", "男", 21, "信息安全"));
    students.append(Student("2021004", "赵六", "女", 20, "数据科学"));
    students.append(Student("2021005", "钱七", "男", 22, "人工智能"));
    updateTable();
    
    qDebug() << "数据加载完成";
    
    // 延迟显示欢迎消息，等窗口完全初始化
    QTimer::singleShot(100, this, [this]() {
        showMessage(AntMessage::Success, QString("欢迎使用！已加载 %1 条记录").arg(students.size()));
    });
    
    qDebug() << "MainWindow 构造函数完成";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    qDebug() << "initUI 开始";
    
    // 创建中心部件
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("QWidget { background-color: #f0f2f5; }");
    setCentralWidget(centralWidget);
    
    qDebug() << "中心部件创建完成";
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    qDebug() << "开始创建左侧卡片";
    
    // ===== 左侧：输入表单区域 =====
    CardWidget* leftCard = new CardWidget(this);
    qDebug() << "CardWidget 创建完成";
    
    leftCard->setMaximumWidth(420);
    leftCard->setBorderRadius(15);
    
    QWidget* leftWidget = new QWidget(leftCard);
    leftWidget->setStyleSheet("QWidget { background-color: transparent; }");
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(12);
    leftLayout->setContentsMargins(25, 25, 25, 25);
    
    // 标题
    QLabel* titleLabel = new QLabel("学生信息录入", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPixelSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #1f1f1f;");
    leftLayout->addWidget(titleLabel);
    
    leftLayout->addSpacing(10);
    
    // 输入样式
    QString inputStyle = R"(
        QLineEdit, QComboBox, QSpinBox {
            border: 1px solid #d9d9d9;
            border-radius: 4px;
            padding: 8px 12px;
            background-color: white;
            color: #262626;
            font-size: 14px;
        }
        QLineEdit:hover, QComboBox:hover, QSpinBox:hover {
            border-color: #4096ff;
        }
        QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
            border-color: #1677ff;
            outline: none;
        }
    )";
    
    // 标签样式
    QString labelStyle = "QLabel { color: #262626; font-size: 14px; font-weight: 500; }";
    
    // 学号输入
    QLabel* labelId = new QLabel("学号", this);
    labelId->setStyleSheet(labelStyle);
    lineEditId = new QLineEdit(this);
    lineEditId->setPlaceholderText("请输入学号");
    lineEditId->setFixedHeight(38);
    lineEditId->setStyleSheet(inputStyle);
    
    // 姓名输入
    QLabel* labelName = new QLabel("姓名", this);
    labelName->setStyleSheet(labelStyle);
    lineEditName = new QLineEdit(this);
    lineEditName->setPlaceholderText("请输入姓名");
    lineEditName->setFixedHeight(38);
    lineEditName->setStyleSheet(inputStyle);
    
    // 性别选择
    QLabel* labelGender = new QLabel("性别", this);
    labelGender->setStyleSheet(labelStyle);
    comboBoxGender = new QComboBox(this);
    comboBoxGender->addItem("男");
    comboBoxGender->addItem("女");
    comboBoxGender->setFixedHeight(38);
    comboBoxGender->setStyleSheet(inputStyle);
    
    // 年龄输入
    QLabel* labelAge = new QLabel("年龄", this);
    labelAge->setStyleSheet(labelStyle);
    spinBoxAge = new QSpinBox(this);
    spinBoxAge->setRange(16, 100);
    spinBoxAge->setValue(20);
    spinBoxAge->setFixedHeight(38);
    spinBoxAge->setStyleSheet(inputStyle);
    
    // 专业输入
    QLabel* labelMajor = new QLabel("专业", this);
    labelMajor->setStyleSheet(labelStyle);
    lineEditMajor = new QLineEdit(this);
    lineEditMajor->setPlaceholderText("请输入专业");
    lineEditMajor->setFixedHeight(38);
    lineEditMajor->setStyleSheet(inputStyle);
    
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
    
    leftLayout->addSpacing(10);
    
    // 操作按钮（左侧）
    QHBoxLayout* btnLayoutLeft1 = new QHBoxLayout();
    btnLayoutLeft1->setSpacing(10);
    
    btnAdd = new AntButton("添加", 14, this);
    btnModify = new AntButton("修改", 14, this);
    btnDelete = new AntButton("删除", 14, this);
    
    btnLayoutLeft1->addWidget(btnAdd);
    btnLayoutLeft1->addWidget(btnModify);
    btnLayoutLeft1->addWidget(btnDelete);
    
    QHBoxLayout* btnLayoutLeft2 = new QHBoxLayout();
    btnLayoutLeft2->setSpacing(10);
    
    btnClear = new AntButton("清空", 14, this);
    btnLayoutLeft2->addWidget(btnClear);
    
    leftLayout->addLayout(btnLayoutLeft1);
    leftLayout->addLayout(btnLayoutLeft2);
    leftLayout->addStretch();
    
    leftCard->layout()->addWidget(leftWidget);
    
    // ===== 右侧：表格显示区域 =====
    CardWidget* rightCard = new CardWidget(this);
    rightCard->setBorderRadius(15);
    
    QWidget* rightWidget = new QWidget(rightCard);
    rightWidget->setStyleSheet("QWidget { background-color: transparent; }");
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(12);
    rightLayout->setContentsMargins(25, 25, 25, 25);
    
    // 表格标题和搜索
    QHBoxLayout* topLayout = new QHBoxLayout();
    
    QLabel* tableTitle = new QLabel("学生列表", this);
    QFont tableTitleFont = tableTitle->font();
    tableTitleFont.setPixelSize(24);
    tableTitleFont.setBold(true);
    tableTitle->setFont(tableTitleFont);
    tableTitle->setStyleSheet("color: #1f1f1f;");
    
    lineEditSearch = new QLineEdit(this);
    lineEditSearch->setPlaceholderText("输入学号搜索");
    lineEditSearch->setFixedHeight(38);
    lineEditSearch->setFixedWidth(200);
    lineEditSearch->setStyleSheet(inputStyle);
    
    btnSearch = new AntButton("搜索", 14, this);
    btnRefresh = new AntButton("刷新", 14, this);
    btnSort = new AntButton("排序", 14, this);
    
    topLayout->addWidget(tableTitle);
    topLayout->addStretch();
    topLayout->addWidget(lineEditSearch);
    topLayout->addWidget(btnSearch);
    topLayout->addWidget(btnRefresh);
    topLayout->addWidget(btnSort);
    
    rightLayout->addLayout(topLayout);
    
    // 表格
    tableWidget = new QTableWidget(this);
    tableWidget->setStyleSheet(R"(
        QTableWidget {
            background-color: white;
            border: 1px solid #f0f0f0;
            border-radius: 4px;
            gridline-color: #f0f0f0;
        }
        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid #f0f0f0;
        }
        QTableWidget::item:selected {
            background-color: #e6f4ff;
            color: #1677ff;
        }
        QHeaderView::section {
            background-color: #fafafa;
            color: #262626;
            padding: 10px;
            border: none;
            border-bottom: 1px solid #f0f0f0;
            font-weight: 600;
        }
    )");
    rightLayout->addWidget(tableWidget);
    
    rightCard->layout()->addWidget(rightWidget);
    
    // 添加到主布局
    mainLayout->addWidget(leftCard, 0);
    mainLayout->addWidget(rightCard, 1);
    
    // 连接信号与槽
    connect(btnAdd, &AntButton::clicked, this, &MainWindow::onAddStudent);
    connect(btnDelete, &AntButton::clicked, this, &MainWindow::onDeleteStudent);
    connect(btnModify, &AntButton::clicked, this, &MainWindow::onModifyStudent);
    connect(btnSearch, &AntButton::clicked, this, &MainWindow::onSearchStudent);
    connect(btnClear, &AntButton::clicked, this, &MainWindow::onClearInput);
    connect(btnRefresh, &AntButton::clicked, this, &MainWindow::onRefreshTable);
    connect(btnSort, &AntButton::clicked, this, &MainWindow::onSortById);
    connect(tableWidget, &QTableWidget::cellClicked, this, &MainWindow::onTableItemClicked);
}

void MainWindow::initTable()
{
    tableWidget->setColumnCount(5);
    tableWidget->setHorizontalHeaderLabels({"学号", "姓名", "性别", "年龄", "专业"});
    
    // 设置表格属性
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->verticalHeader()->setVisible(false);
}

void MainWindow::updateTable()
{
    tableWidget->setRowCount(0);
    
    for (const Student& student : students) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        
        tableWidget->setItem(row, 0, new QTableWidgetItem(student.getId()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(student.getName()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(student.getGender()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(student.getAge())));
        tableWidget->setItem(row, 4, new QTableWidgetItem(student.getMajor()));
    }
}

void MainWindow::clearInputFields()
{
    lineEditId->clear();
    lineEditName->clear();
    comboBoxGender->setCurrentIndex(0);
    spinBoxAge->setValue(20);
    lineEditMajor->clear();
    currentEditingStudentId.clear();
}

bool MainWindow::validateInput()
{
    if (lineEditId->text().isEmpty()) {
        showMessage(AntMessage::Warning, "学号不能为空！");
        return false;
    }
    
    if (lineEditName->text().isEmpty()) {
        showMessage(AntMessage::Warning, "姓名不能为空！");
        return false;
    }
    
    if (lineEditMajor->text().isEmpty()) {
        showMessage(AntMessage::Warning, "专业不能为空！");
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

void MainWindow::showMessage(AntMessage::Type type, const QString& message)
{
    AntMessageManager::instance()->showMessage(type, message, 3000);
}

void MainWindow::onAddStudent()
{
    if (!validateInput()) {
        return;
    }
    
    QString id = lineEditId->text().trimmed();
    
    // 检查学号是否已存在
    if (findStudentById(id) != -1) {
        showMessage(AntMessage::Warning, QString("学号 %1 已存在！").arg(id));
        return;
    }
    
    // 创建新学生
    Student newStudent(
        id,
        lineEditName->text().trimmed(),
        comboBoxGender->currentText(),
        spinBoxAge->value(),
        lineEditMajor->text().trimmed()
    );
    
    students.append(newStudent);
    updateTable();
    clearInputFields();
    
    showMessage(AntMessage::Success, "添加成功！");
}

void MainWindow::onDeleteStudent()
{
    QString id = lineEditId->text().trimmed();
    
    if (id.isEmpty()) {
        showMessage(AntMessage::Warning, "请输入要删除的学号！");
        return;
    }
    
    int index = findStudentById(id);
    if (index == -1) {
        showMessage(AntMessage::Warning, QString("未找到学号为 %1 的学生！").arg(id));
        return;
    }
    
    // 确认删除
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "确认删除",
        QString("确定要删除学号为 %1 的学生吗？").arg(id),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        students.removeAt(index);
        updateTable();
        clearInputFields();
        
        showMessage(AntMessage::Success, "删除成功！");
    }
}

void MainWindow::onModifyStudent()
{
    if (!validateInput()) {
        return;
    }
    
    QString id = lineEditId->text().trimmed();
    QString searchId = currentEditingStudentId.isEmpty() ? id : currentEditingStudentId;
    
    int index = findStudentById(searchId);
    if (index == -1) {
        showMessage(AntMessage::Warning, QString("未找到学号为 %1 的学生！").arg(searchId));
        return;
    }
    
    // 如果修改了学号，检查新学号是否已存在
    if (id != searchId && findStudentById(id) != -1) {
        showMessage(AntMessage::Warning, QString("学号 %1 已存在！").arg(id));
        return;
    }
    
    // 更新学生信息
    students[index] = Student(
        id,
        lineEditName->text().trimmed(),
        comboBoxGender->currentText(),
        spinBoxAge->value(),
        lineEditMajor->text().trimmed()
    );
    
    updateTable();
    clearInputFields();
    
    showMessage(AntMessage::Success, "修改成功！");
}

void MainWindow::onSearchStudent()
{
    QString id = lineEditSearch->text().trimmed();
    
    if (id.isEmpty()) {
        showMessage(AntMessage::Warning, "请输入要搜索的学号！");
        return;
    }
    
    int index = findStudentById(id);
    if (index == -1) {
        showMessage(AntMessage::Info, QString("未找到学号为 %1 的学生！").arg(id));
        return;
    }
    
    // 高亮显示找到的行
    tableWidget->selectRow(index);
    
    showMessage(AntMessage::Success, QString("找到学生：%1").arg(students[index].getName()));
}

void MainWindow::onClearInput()
{
    clearInputFields();
    showMessage(AntMessage::Info, "已清空输入框");
}

void MainWindow::onRefreshTable()
{
    updateTable();
    showMessage(AntMessage::Success, "刷新成功！");
}

void MainWindow::onSortById()
{
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.getId() < b.getId();
    });
    
    updateTable();
    showMessage(AntMessage::Success, "按学号排序完成！");
}

void MainWindow::onTableItemClicked(int row, int column)
{
    Q_UNUSED(column);
    
    if (row < 0 || row >= students.size()) {
        return;
    }
    
    const Student& student = students[row];
    
    // 填充输入框
    lineEditId->setText(student.getId());
    lineEditName->setText(student.getName());
    comboBoxGender->setCurrentText(student.getGender());
    spinBoxAge->setValue(student.getAge());
    lineEditMajor->setText(student.getMajor());
    
    // 记录当前编辑的学生学号
    currentEditingStudentId = student.getId();
}
