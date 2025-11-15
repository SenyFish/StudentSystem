#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ElaWindow.h"
#include "ElaTableView.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaComboBox.h"
#include "ElaSpinBox.h"
#include "ElaText.h"
#include "ElaMessageBar.h"
#include "ElaContentDialog.h"
#include "cardwidget.h"
#include <QStandardItemModel>
#include <QVector>
#include "student.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public ElaWindow
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
    void onSortById();          // 按学号排序
    void onTableItemClicked(int row, int column);  // 表格项点击

private:
    Ui::MainWindow *ui;
    
    // UI控件 - 使用 Ela 组件
    ElaTableView *tableWidget;
    QStandardItemModel *tableModel;
    ElaLineEdit *lineEditId;
    ElaLineEdit *lineEditName;
    ElaComboBox *comboBoxGender;
    ElaSpinBox *spinBoxAge;
    ElaLineEdit *lineEditMajor;
    ElaLineEdit *lineEditSearch;
    
    ElaPushButton *btnAdd;
    ElaPushButton *btnDelete;
    ElaPushButton *btnModify;
    ElaPushButton *btnSearch;
    ElaPushButton *btnClear;
    ElaPushButton *btnRefresh;
    ElaPushButton *btnSort;
    
    // 数据存储
    QVector<Student> students;
    QString currentEditingStudentId;  // 当前正在编辑的学生原始学号
    
    // 辅助方法
    void initUI();              // 初始化界面
    void initTable();           // 初始化表格
    void updateTable();         // 更新表格显示
    void clearInputFields();    // 清空输入框
    bool validateInput();       // 验证输入
    int findStudentById(const QString &id);  // 根据学号查找学生
};
#endif // MAINWINDOW_H
