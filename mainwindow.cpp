#include "mainwindow.h"
#include "./ui_mainwindow.h"
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
    
    qDebug() << "显示欢迎消息";
    // 显示欢迎消息
    ElaMessageBar::success(ElaMessageBarType::TopRight, "欢迎", 
                          QString("系统已加载 %1 条学生记录").arg(students.size()), 3000, this);
    
    qDebug() << "MainWindow 构造函数完成";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    // 创建主布局容器
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // ===== 左侧：输入表单区域 =====
    // 创建卡片容器
    CardWidget* leftCard = new CardWidget(this);
    leftCard->setMaximumWidth(420);
    leftCard->setBorderRadius(15);
    
    QWidget* leftWidget = new QWidget(leftCard);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(12);
    leftLayout->setContentsMargins(25, 25, 25, 25);
    
    // 标题
    ElaText* titleText = new ElaText("学生信息录入", this);
    titleText->setTextPixelSize(24);
    leftLayout->addWidget(titleText);
    
    leftLayout->addSpacing(10);
    
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
    
    // 操作按钮 - 使用更现代的设计
    btnAdd = new ElaPushButton("添加学生", this);
    btnAdd->setFixedHeight(45);
    btnAdd->setStyleSheet("font-size: 14px; font-weight: bold;");
    connect(btnAdd, &ElaPushButton::clicked, this, &MainWindow::onAddStudent);
    
    btnModify = new ElaPushButton("修改学生", this);
    btnModify->setFixedHeight(45);
    btnModify->setStyleSheet("font-size: 14px;");
    connect(btnModify, &ElaPushButton::clicked, this, &MainWindow::onModifyStudent);
    
    btnDelete = new ElaPushButton("删除学生", this);
    btnDelete->setFixedHeight(45);
    btnDelete->setStyleSheet("font-size: 14px;");
    connect(btnDelete, &ElaPushButton::clicked, this, &MainWindow::onDeleteStudent);
    
    btnClear = new ElaPushButton("清空输入", this);
    btnClear->setFixedHeight(45);
    btnClear->setStyleSheet("font-size: 14px;");
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
    // 创建卡片容器
    CardWidget* rightCard = new CardWidget(this);
    rightCard->setBorderRadius(15);
    
    QWidget* rightWidget = new QWidget(rightCard);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(15);
    rightLayout->setContentsMargins(25, 25, 25, 25);
    
    // 标题和搜索栏
    QHBoxLayout* topLayout = new QHBoxLayout();
    
    ElaText* tableTitle = new ElaText("学生信息列表", this);
    tableTitle->setTextPixelSize(24);
    
    lineEditSearch = new ElaLineEdit(this);
    lineEditSearch->setPlaceholderText("搜索学号或姓名...");
    lineEditSearch->setMaximumWidth(300);
    lineEditSearch->setFixedHeight(38);
    connect(lineEditSearch, &ElaLineEdit::returnPressed, this, &MainWindow::onSearchStudent);
    
    btnSearch = new ElaPushButton("搜索", this);
    btnSearch->setFixedSize(95, 38);
    connect(btnSearch, &ElaPushButton::clicked, this, &MainWindow::onSearchStudent);
    
    btnRefresh = new ElaPushButton("刷新", this);
    btnRefresh->setFixedSize(95, 38);
    connect(btnRefresh, &ElaPushButton::clicked, this, &MainWindow::onRefreshTable);
    
    btnSort = new ElaPushButton("排序", this);
    btnSort->setFixedSize(95, 38);
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
    
    // 将左右卡片添加到主布局
    mainLayout->addWidget(leftCard, 2);
    mainLayout->addWidget(rightCard, 5);
    
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
