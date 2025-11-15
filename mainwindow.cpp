#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
    initTable();
    
    // 添加一些示例数据
    students.append(Student("2021001", "张三", "男", 20, "计算机科学"));
    students.append(Student("2021002", "李四", "女", 19, "软件工程"));
    students.append(Student("2021003", "王五", "男", 21, "信息安全"));
    updateTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    // 设置窗口标题和大小
    setWindowTitle("学生信息管理系统");
    resize(1000, 600);
    
    // 创建中心部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // 左侧：输入区域
    QGroupBox *inputGroup = new QGroupBox("学生信息", this);
    QGridLayout *inputLayout = new QGridLayout(inputGroup);
    
    // 创建标签和输入控件
    QLabel *labelId = new QLabel("学号:", this);
    lineEditId = new QLineEdit(this);
    
    QLabel *labelName = new QLabel("姓名:", this);
    lineEditName = new QLineEdit(this);
    
    QLabel *labelGender = new QLabel("性别:", this);
    comboBoxGender = new QComboBox(this);
    comboBoxGender->addItems({"男", "女"});
    
    QLabel *labelAge = new QLabel("年龄:", this);
    spinBoxAge = new QSpinBox(this);
    spinBoxAge->setRange(15, 100);
    spinBoxAge->setValue(18);
    
    QLabel *labelMajor = new QLabel("专业:", this);
    lineEditMajor = new QLineEdit(this);
    
    // 添加到网格布局
    inputLayout->addWidget(labelId, 0, 0);
    inputLayout->addWidget(lineEditId, 0, 1);
    inputLayout->addWidget(labelName, 1, 0);
    inputLayout->addWidget(lineEditName, 1, 1);
    inputLayout->addWidget(labelGender, 2, 0);
    inputLayout->addWidget(comboBoxGender, 2, 1);
    inputLayout->addWidget(labelAge, 3, 0);
    inputLayout->addWidget(spinBoxAge, 3, 1);
    inputLayout->addWidget(labelMajor, 4, 0);
    inputLayout->addWidget(lineEditMajor, 4, 1);
    
    // 操作按钮
    btnAdd = new QPushButton("添加学生", this);
    btnDelete = new QPushButton("删除学生", this);
    btnModify = new QPushButton("修改学生", this);
    btnClear = new QPushButton("清空输入", this);
    
    inputLayout->addWidget(btnAdd, 5, 0, 1, 2);
    inputLayout->addWidget(btnDelete, 6, 0, 1, 2);
    inputLayout->addWidget(btnModify, 7, 0, 1, 2);
    inputLayout->addWidget(btnClear, 8, 0, 1, 2);
    
    inputLayout->setRowStretch(9, 1);
    
    // 右侧：表格和搜索区域
    QVBoxLayout *rightLayout = new QVBoxLayout();
    
    // 搜索栏
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *labelSearch = new QLabel("搜索:", this);
    lineEditSearch = new QLineEdit(this);
    lineEditSearch->setPlaceholderText("输入学号或姓名进行搜索");
    btnSearch = new QPushButton("搜索", this);
    btnRefresh = new QPushButton("刷新显示", this);
    
    searchLayout->addWidget(labelSearch);
    searchLayout->addWidget(lineEditSearch);
    searchLayout->addWidget(btnSearch);
    searchLayout->addWidget(btnRefresh);
    
    // 表格
    tableWidget = new QTableWidget(this);
    
    rightLayout->addLayout(searchLayout);
    rightLayout->addWidget(tableWidget);
    
    // 添加到主布局
    mainLayout->addWidget(inputGroup, 1);
    mainLayout->addLayout(rightLayout, 3);
    
    // 连接信号和槽
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteStudent);
    connect(btnModify, &QPushButton::clicked, this, &MainWindow::onModifyStudent);
    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::onSearchStudent);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearInput);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::onRefreshTable);
    connect(tableWidget, &QTableWidget::cellClicked, this, &MainWindow::onTableItemClicked);
    
    // 设置按钮样式
    QString buttonStyle = "QPushButton { padding: 8px; font-size: 14px; }";
    btnAdd->setStyleSheet(buttonStyle);
    btnDelete->setStyleSheet(buttonStyle);
    btnModify->setStyleSheet(buttonStyle);
    btnClear->setStyleSheet(buttonStyle);
    btnSearch->setStyleSheet(buttonStyle);
    btnRefresh->setStyleSheet(buttonStyle);
}

void MainWindow::initTable()
{
    // 设置列数和列标题
    tableWidget->setColumnCount(5);
    QStringList headers = {"学号", "姓名", "性别", "年龄", "专业"};
    tableWidget->setHorizontalHeaderLabels(headers);
    
    // 设置表格属性
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setAlternatingRowColors(true);
}

void MainWindow::updateTable()
{
    tableWidget->setRowCount(students.size());
    
    for (int i = 0; i < students.size(); ++i) {
        const Student &student = students[i];
        
        tableWidget->setItem(i, 0, new QTableWidgetItem(student.getId()));
        tableWidget->setItem(i, 1, new QTableWidgetItem(student.getName()));
        tableWidget->setItem(i, 2, new QTableWidgetItem(student.getGender()));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(student.getAge())));
        tableWidget->setItem(i, 4, new QTableWidgetItem(student.getMajor()));
    }
}

void MainWindow::clearInputFields()
{
    lineEditId->clear();
    lineEditName->clear();
    comboBoxGender->setCurrentIndex(0);
    spinBoxAge->setValue(18);
    lineEditMajor->clear();
}

bool MainWindow::validateInput()
{
    if (lineEditId->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "学号不能为空！");
        return false;
    }
    
    if (lineEditName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "姓名不能为空！");
        return false;
    }
    
    if (lineEditMajor->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "专业不能为空！");
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
        QMessageBox::warning(this, "添加失败", "该学号已存在！");
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
    
    QMessageBox::information(this, "成功", "学生信息添加成功！");
}

void MainWindow::onDeleteStudent()
{
    QString id = lineEditId->text().trimmed();
    
    if (id.isEmpty()) {
        QMessageBox::warning(this, "删除失败", "请输入要删除的学生学号！");
        return;
    }
    
    int index = findStudentById(id);
    if (index == -1) {
        QMessageBox::warning(this, "删除失败", "未找到该学号的学生！");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", 
                                   QString("确定要删除学生 %1 的信息吗？").arg(students[index].getName()),
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        students.remove(index);
        updateTable();
        clearInputFields();
        QMessageBox::information(this, "成功", "学生信息删除成功！");
    }
}

void MainWindow::onModifyStudent()
{
    if (!validateInput()) {
        return;
    }
    
    QString id = lineEditId->text().trimmed();
    int index = findStudentById(id);
    
    if (index == -1) {
        QMessageBox::warning(this, "修改失败", "未找到该学号的学生！");
        return;
    }
    
    students[index].setName(lineEditName->text().trimmed());
    students[index].setGender(comboBoxGender->currentText());
    students[index].setAge(spinBoxAge->value());
    students[index].setMajor(lineEditMajor->text().trimmed());
    
    updateTable();
    QMessageBox::information(this, "成功", "学生信息修改成功！");
}

void MainWindow::onSearchStudent()
{
    QString keyword = lineEditSearch->text().trimmed();
    
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "搜索失败", "请输入搜索关键词！");
        return;
    }
    
    // 清空表格
    tableWidget->setRowCount(0);
    
    // 搜索匹配的学生
    int row = 0;
    for (const Student &student : students) {
        if (student.getId().contains(keyword, Qt::CaseInsensitive) ||
            student.getName().contains(keyword, Qt::CaseInsensitive)) {
            
            tableWidget->insertRow(row);
            tableWidget->setItem(row, 0, new QTableWidgetItem(student.getId()));
            tableWidget->setItem(row, 1, new QTableWidgetItem(student.getName()));
            tableWidget->setItem(row, 2, new QTableWidgetItem(student.getGender()));
            tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(student.getAge())));
            tableWidget->setItem(row, 4, new QTableWidgetItem(student.getMajor()));
            row++;
        }
    }
    
    if (row == 0) {
        QMessageBox::information(this, "搜索结果", "未找到匹配的学生信息！");
        updateTable();
    }
}

void MainWindow::onClearInput()
{
    clearInputFields();
}

void MainWindow::onRefreshTable()
{
    updateTable();
    lineEditSearch->clear();
}

void MainWindow::onTableItemClicked(int row, int column)
{
    Q_UNUSED(column);
    
    if (row >= 0 && row < tableWidget->rowCount()) {
        QString id = tableWidget->item(row, 0)->text();
        int index = findStudentById(id);
        
        if (index != -1) {
            const Student &student = students[index];
            lineEditId->setText(student.getId());
            lineEditName->setText(student.getName());
            comboBoxGender->setCurrentText(student.getGender());
            spinBoxAge->setValue(student.getAge());
            lineEditMajor->setText(student.getMajor());
        }
    }
}
