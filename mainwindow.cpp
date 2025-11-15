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
    
    // 设置状态栏样式
    statusBar()->setStyleSheet(
        "QStatusBar {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4a90e2, stop:1 #357abd);"
        "    color: white;"
        "    font-size: 13px;"
        "    padding: 5px;"
        "}"
    );
    statusBar()->showMessage("✨ 欢迎使用学生信息管理系统！当前系统运行正常。", 5000);
    
    // 美化菜单栏
    menuBar()->setStyleSheet(
        "QMenuBar {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4a90e2, stop:1 #357abd);"
        "    color: white;"
        "    font-size: 14px;"
        "    padding: 5px;"
        "}"
        "QMenuBar::item {"
        "    padding: 5px 10px;"
        "    background: transparent;"
        "}"
        "QMenuBar::item:selected {"
        "    background: rgba(255, 255, 255, 0.2);"
        "    border-radius: 5px;"
        "}"
    );
    
    // 添加一些示例数据
    students.append(Student("2021001", "张三", "男", 20, "计算机科学"));
    students.append(Student("2021002", "李四", "女", 19, "软件工程"));
    students.append(Student("2021003", "王五", "男", 21, "信息安全"));
    updateTable();
    
    // 更新状态栏显示学生数量
    statusBar()->showMessage(QString("📊 系统已加载 %1 条学生记录").arg(students.size()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    // 设置窗口标题和大小
    setWindowTitle("🎓 学生信息管理系统");
    resize(1200, 700);
    setMinimumSize(1000, 600);
    
    // 创建中心部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 设置整体背景色
    centralWidget->setStyleSheet("QWidget#centralWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                                  "stop:0 #f5f7fa, stop:1 #e8ecf1); }");
    centralWidget->setObjectName("centralWidget");
    
    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // 左侧：输入区域
    QGroupBox *inputGroup = new QGroupBox("📝 学生信息录入", this);
    inputGroup->setStyleSheet(
        "QGroupBox {"
        "    background-color: white;"
        "    border-radius: 15px;"
        "    padding: 20px;"
        "    margin-top: 15px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    color: #2c3e50;"
        "    border: 2px solid #e0e6ed;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    subcontrol-position: top left;"
        "    padding: 5px 10px;"
        "    background-color: #4a90e2;"
        "    color: white;"
        "    border-radius: 5px;"
        "}"
    );
    
    QGridLayout *inputLayout = new QGridLayout(inputGroup);
    inputLayout->setSpacing(15);
    inputLayout->setContentsMargins(20, 30, 20, 20);
    
    // 标签样式
    QString labelStyle = "QLabel { font-size: 14px; color: #555; font-weight: 600; }";
    
    // 输入框通用样式
    QString inputStyle = 
        "QLineEdit, QComboBox, QSpinBox {"
        "    padding: 10px 15px;"
        "    border: 2px solid #e0e6ed;"
        "    border-radius: 8px;"
        "    background-color: #f8f9fa;"
        "    font-size: 14px;"
        "    color: #2c3e50;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus {"
        "    border: 2px solid #4a90e2;"
        "    background-color: white;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    padding-right: 10px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border: none;"
        "}"
        "QSpinBox::up-button, QSpinBox::down-button {"
        "    width: 20px;"
        "    border: none;"
        "    background: #4a90e2;"
        "    border-radius: 4px;"
        "}"
        "QSpinBox::up-button:hover, QSpinBox::down-button:hover {"
        "    background: #357abd;"
        "}";
    
    // 创建标签和输入控件
    QLabel *labelId = new QLabel("🆔 学号:", this);
    labelId->setStyleSheet(labelStyle);
    lineEditId = new QLineEdit(this);
    lineEditId->setPlaceholderText("请输入学号");
    lineEditId->setStyleSheet(inputStyle);
    
    QLabel *labelName = new QLabel("👤 姓名:", this);
    labelName->setStyleSheet(labelStyle);
    lineEditName = new QLineEdit(this);
    lineEditName->setPlaceholderText("请输入姓名");
    lineEditName->setStyleSheet(inputStyle);
    
    QLabel *labelGender = new QLabel("⚧ 性别:", this);
    labelGender->setStyleSheet(labelStyle);
    comboBoxGender = new QComboBox(this);
    comboBoxGender->addItems({"男", "女"});
    comboBoxGender->setStyleSheet(inputStyle);
    
    QLabel *labelAge = new QLabel("🎂 年龄:", this);
    labelAge->setStyleSheet(labelStyle);
    spinBoxAge = new QSpinBox(this);
    spinBoxAge->setRange(15, 100);
    spinBoxAge->setValue(18);
    spinBoxAge->setStyleSheet(inputStyle);
    
    QLabel *labelMajor = new QLabel("📚 专业:", this);
    labelMajor->setStyleSheet(labelStyle);
    lineEditMajor = new QLineEdit(this);
    lineEditMajor->setPlaceholderText("请输入专业");
    lineEditMajor->setStyleSheet(inputStyle);
    
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
    
    // 操作按钮 - 使用更现代的样式
    QString btnAddStyle = 
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4CAF50, stop:1 #45a049);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 12px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #45a049, stop:1 #3d8b40);"
        "}"
        "QPushButton:pressed {"
        "    background: #3d8b40;"
        "}";
    
    QString btnDeleteStyle = 
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f44336, stop:1 #da190b);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 12px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #da190b, stop:1 #c41a0a);"
        "}"
        "QPushButton:pressed {"
        "    background: #c41a0a;"
        "}";
    
    QString btnModifyStyle = 
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2196F3, stop:1 #0b7dda);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 12px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0b7dda, stop:1 #0a6bc4);"
        "}"
        "QPushButton:pressed {"
        "    background: #0a6bc4;"
        "}";
    
    QString btnClearStyle = 
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #9E9E9E, stop:1 #757575);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 12px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #757575, stop:1 #616161);"
        "}"
        "QPushButton:pressed {"
        "    background: #616161;"
        "}";
    
    btnAdd = new QPushButton("➕ 添加学生", this);
    btnAdd->setStyleSheet(btnAddStyle);
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setMinimumHeight(45);
    
    btnDelete = new QPushButton("🗑️ 删除学生", this);
    btnDelete->setStyleSheet(btnDeleteStyle);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setMinimumHeight(45);
    
    btnModify = new QPushButton("✏️ 修改学生", this);
    btnModify->setStyleSheet(btnModifyStyle);
    btnModify->setCursor(Qt::PointingHandCursor);
    btnModify->setMinimumHeight(45);
    
    btnClear = new QPushButton("🔄 清空输入", this);
    btnClear->setStyleSheet(btnClearStyle);
    btnClear->setCursor(Qt::PointingHandCursor);
    btnClear->setMinimumHeight(45);
    
    inputLayout->addWidget(btnAdd, 5, 0, 1, 2);
    inputLayout->addWidget(btnDelete, 6, 0, 1, 2);
    inputLayout->addWidget(btnModify, 7, 0, 1, 2);
    inputLayout->addWidget(btnClear, 8, 0, 1, 2);
    
    inputLayout->setRowStretch(9, 1);
    
    // 右侧：表格和搜索区域
    QWidget *rightWidget = new QWidget(this);
    rightWidget->setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border-radius: 15px;"
        "    border: 2px solid #e0e6ed;"
        "}"
    );
    
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(15);
    rightLayout->setContentsMargins(20, 20, 20, 20);
    
    // 搜索栏标题
    QLabel *searchTitle = new QLabel("🔍 学生信息列表", this);
    searchTitle->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #2c3e50; }");
    rightLayout->addWidget(searchTitle);
    
    // 搜索栏
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(10);
    
    QLabel *labelSearch = new QLabel("搜索:", this);
    labelSearch->setStyleSheet("QLabel { font-size: 14px; color: #555; }");
    
    lineEditSearch = new QLineEdit(this);
    lineEditSearch->setPlaceholderText("🔍 输入学号或姓名进行搜索...");
    lineEditSearch->setStyleSheet(inputStyle);
    lineEditSearch->setMinimumHeight(40);
    
    QString btnSearchStyle = 
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FF9800, stop:1 #F57C00);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 10px 20px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #F57C00, stop:1 #E65100);"
        "}"
        "QPushButton:pressed {"
        "    background: #E65100;"
        "}";
    
    QString btnRefreshStyle = 
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #607D8B, stop:1 #455A64);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 10px 20px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #455A64, stop:1 #37474F);"
        "}"
        "QPushButton:pressed {"
        "    background: #37474F;"
        "}";
    
    btnSearch = new QPushButton("🔍 搜索", this);
    btnSearch->setStyleSheet(btnSearchStyle);
    btnSearch->setCursor(Qt::PointingHandCursor);
    btnSearch->setMinimumHeight(40);
    
    btnRefresh = new QPushButton("🔄 刷新", this);
    btnRefresh->setStyleSheet(btnRefreshStyle);
    btnRefresh->setCursor(Qt::PointingHandCursor);
    btnRefresh->setMinimumHeight(40);
    
    searchLayout->addWidget(labelSearch);
    searchLayout->addWidget(lineEditSearch);
    searchLayout->addWidget(btnSearch);
    searchLayout->addWidget(btnRefresh);
    
    // 表格
    tableWidget = new QTableWidget(this);
    
    rightLayout->addLayout(searchLayout);
    rightLayout->addWidget(tableWidget);
    
    // 添加到主布局
    mainLayout->addWidget(inputGroup, 2);
    mainLayout->addWidget(rightWidget, 5);
    
    // 连接信号和槽
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteStudent);
    connect(btnModify, &QPushButton::clicked, this, &MainWindow::onModifyStudent);
    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::onSearchStudent);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearInput);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::onRefreshTable);
    connect(tableWidget, &QTableWidget::cellClicked, this, &MainWindow::onTableItemClicked);
    
    // 允许回车键触发搜索
    connect(lineEditSearch, &QLineEdit::returnPressed, this, &MainWindow::onSearchStudent);
}

void MainWindow::initTable()
{
    // 设置列数和列标题
    tableWidget->setColumnCount(5);
    QStringList headers = {"🆔 学号", "👤 姓名", "⚧ 性别", "🎂 年龄", "📚 专业"};
    tableWidget->setHorizontalHeaderLabels(headers);
    
    // 设置表格属性
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setShowGrid(true);
    
    // 设置表格样式
    QString tableStyle = 
        "QTableWidget {"
        "    background-color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    gridline-color: #e8ecf1;"
        "    font-size: 13px;"
        "    color: #2c3e50;"
        "}"
        "QTableWidget::item {"
        "    padding: 10px;"
        "    border-bottom: 1px solid #e8ecf1;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #e3f2fd;"
        "    color: #1976d2;"
        "}"
        "QTableWidget::item:hover {"
        "    background-color: #f5f7fa;"
        "}"
        "QHeaderView::section {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4a90e2, stop:1 #357abd);"
        "    color: white;"
        "    padding: 12px;"
        "    border: none;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    text-align: center;"
        "}"
        "QHeaderView::section:first {"
        "    border-top-left-radius: 8px;"
        "}"
        "QHeaderView::section:last {"
        "    border-top-right-radius: 8px;"
        "}"
        "QTableWidget::item:alternate {"
        "    background-color: #f8f9fa;"
        "}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #f5f7fa;"
        "    width: 10px;"
        "    border-radius: 5px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #c0c0c0;"
        "    min-height: 20px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #a0a0a0;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        "QScrollBar:horizontal {"
        "    border: none;"
        "    background: #f5f7fa;"
        "    height: 10px;"
        "    border-radius: 5px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:horizontal {"
        "    background: #c0c0c0;"
        "    min-width: 20px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::handle:horizontal:hover {"
        "    background: #a0a0a0;"
        "}"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {"
        "    width: 0px;"
        "}";
    
    tableWidget->setStyleSheet(tableStyle);
    
    // 设置行高
    tableWidget->verticalHeader()->setDefaultSectionSize(45);
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
        statusBar()->showMessage("❌ 输入验证失败", 3000);
        return;
    }
    
    QString id = lineEditId->text().trimmed();
    
    // 检查学号是否已存在
    if (findStudentById(id) != -1) {
        QMessageBox::warning(this, "⚠️ 添加失败", "该学号已存在！");
        statusBar()->showMessage("❌ 添加失败：学号重复", 3000);
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
    
    QMessageBox::information(this, "✅ 成功", QString("学生 %1 的信息已成功添加！").arg(name));
    statusBar()->showMessage(QString("✅ 成功添加学生：%1（学号：%2） | 当前共有 %3 条记录").arg(name).arg(id).arg(students.size()));
}

void MainWindow::onDeleteStudent()
{
    QString id = lineEditId->text().trimmed();
    
    if (id.isEmpty()) {
        QMessageBox::warning(this, "⚠️ 删除失败", "请输入要删除的学生学号！");
        statusBar()->showMessage("❌ 请先输入要删除的学号", 3000);
        return;
    }
    
    int index = findStudentById(id);
    if (index == -1) {
        QMessageBox::warning(this, "⚠️ 删除失败", "未找到该学号的学生！");
        statusBar()->showMessage("❌ 删除失败：未找到该学生", 3000);
        return;
    }
    
    QString studentName = students[index].getName();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "🗑️ 确认删除", 
                                   QString("确定要删除学生 <b>%1</b>（学号：%2）的信息吗？").arg(studentName).arg(id),
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        students.remove(index);
        updateTable();
        clearInputFields();
        QMessageBox::information(this, "✅ 成功", "学生信息已成功删除！");
        statusBar()->showMessage(QString("✅ 已删除学生：%1（学号：%2） | 当前共有 %3 条记录").arg(studentName).arg(id).arg(students.size()));
    } else {
        statusBar()->showMessage("📝 已取消删除操作", 3000);
    }
}

void MainWindow::onModifyStudent()
{
    if (!validateInput()) {
        statusBar()->showMessage("❌ 输入验证失败", 3000);
        return;
    }
    
    QString id = lineEditId->text().trimmed();
    int index = findStudentById(id);
    
    if (index == -1) {
        QMessageBox::warning(this, "⚠️ 修改失败", "未找到该学号的学生！");
        statusBar()->showMessage("❌ 修改失败：未找到该学生", 3000);
        return;
    }
    
    QString name = lineEditName->text().trimmed();
    students[index].setName(name);
    students[index].setGender(comboBoxGender->currentText());
    students[index].setAge(spinBoxAge->value());
    students[index].setMajor(lineEditMajor->text().trimmed());
    
    updateTable();
    QMessageBox::information(this, "✅ 成功", QString("学生 %1 的信息已成功更新！").arg(name));
    statusBar()->showMessage(QString("✅ 成功修改学生：%1（学号：%2）").arg(name).arg(id));
}

void MainWindow::onSearchStudent()
{
    QString keyword = lineEditSearch->text().trimmed();
    
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "⚠️ 搜索失败", "请输入搜索关键词！");
        statusBar()->showMessage("❌ 请输入搜索关键词", 3000);
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
        QMessageBox::information(this, "🔍 搜索结果", QString("未找到包含 \"%1\" 的学生信息！").arg(keyword));
        updateTable();
        statusBar()->showMessage(QString("❌ 未找到匹配 \"%1\" 的记录").arg(keyword), 3000);
    } else {
        statusBar()->showMessage(QString("🔍 找到 %1 条匹配 \"%2\" 的记录").arg(row).arg(keyword));
    }
}

void MainWindow::onClearInput()
{
    clearInputFields();
    statusBar()->showMessage("🔄 已清空输入框", 3000);
}

void MainWindow::onRefreshTable()
{
    updateTable();
    lineEditSearch->clear();
    statusBar()->showMessage(QString("🔄 已刷新列表 | 当前共有 %1 条记录").arg(students.size()));
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
            statusBar()->showMessage(QString("📝 已选择学生：%1（学号：%2）").arg(student.getName()).arg(student.getId()));
        }
    }
}
