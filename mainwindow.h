#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QMessageBox>
#include <QVector>
#include "student.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddStudent();        // 添加学生
    void onDeleteStudent();     // 删除学生
    void onModifyStudent();     // 修改学生
    void onSearchStudent();     // 搜索学生
    void onClearInput();        // 清空输入框
    void onRefreshTable();      // 刷新表格
    void onTableItemClicked(int row, int column);  // 表格项点击

private:
    Ui::MainWindow *ui;
    
    // UI控件
    QTableWidget *tableWidget;
    QLineEdit *lineEditId;
    QLineEdit *lineEditName;
    QComboBox *comboBoxGender;
    QSpinBox *spinBoxAge;
    QLineEdit *lineEditMajor;
    QLineEdit *lineEditSearch;
    
    QPushButton *btnAdd;
    QPushButton *btnDelete;
    QPushButton *btnModify;
    QPushButton *btnSearch;
    QPushButton *btnClear;
    QPushButton *btnRefresh;
    
    // 数据存储
    QVector<Student> students;
    
    // 辅助方法
    void initUI();              // 初始化界面
    void initTable();           // 初始化表格
    void updateTable();         // 更新表格显示
    void clearInputFields();    // 清空输入框
    bool validateInput();       // 验证输入
    int findStudentById(const QString &id);  // 根据学号查找学生
};
#endif // MAINWINDOW_H
