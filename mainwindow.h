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
#include "flatcardwidget.h"
// #include "ElaInteractiveCard.h"
#include "ElaScrollArea.h"
#include <QStandardItemModel>
#include <QGridLayout>
#include <QVector>
#include <QLabel>
#include "student.h"
#include "user.h"

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
    
    void setUserRole(UserRole role, const QString &studentId = "", const QString &major = "");  // 设置用户角色权限
    Student* getStudentById(const QString &studentId);  // 根据学号获取学生对象（供外部调用）

private slots:
    void onAddStudent();        // 添加学生
    void onDeleteStudent();     // 删除学生
    void onModifyStudent();     // 修改学生
    void onSearchStudent();     // 搜索学生
    void onClearInput();        // 清空输入框
    void onRefreshTable();      // 刷新表格
    void onSortById();          // 按学号排序
    void onTableItemClicked(int row, int column);  // 表格项点击
    void onExportCSV();         // 导出CSV文件
    void onImportCSV();         // 导入CSV文件
    void onBatchDelete();       // 批量删除学生
    void onBatchModifyMajor();  // 批量修改专业
    void onStudentEditInfo();   // 学生编辑自己的信息

private:
    Ui::MainWindow *ui;
    
    // UI控件 - 使用 Ela 组件
    ElaTableView *tableWidget;
    QStandardItemModel *tableModel;
    ElaLineEdit *lineEditId;
    ElaLineEdit *lineEditName;
    ElaComboBox *comboBoxGender;
    ElaSpinBox *spinBoxAge;
    ElaComboBox *comboBoxMajor;  // 改为下拉框
    ElaLineEdit *lineEditSearch;
    
    ElaPushButton *btnAdd;
    ElaPushButton *btnDelete;
    ElaPushButton *btnModify;
    ElaPushButton *btnSearch;
    ElaPushButton *btnClear;
    ElaPushButton *btnRefresh;
    ElaPushButton *btnSort;
    ElaPushButton *btnExport;
    ElaPushButton *btnImport;
    ElaPushButton *btnBatchDelete;
    ElaPushButton *btnBatchModify;
    
    // 数据存储
    QVector<Student> students;
    QString currentEditingStudentId;  // 当前正在编辑的学生原始学号
    UserRole currentUserRole;  // 当前用户角色
    QString currentUserStudentId;  // 当前用户关联的学生ID（仅学生角色）
    QString currentUserMajor;  // 当前用户关联的专业（仅教师角色）
    
    // UI容器（用于不同角色的布局控制）
    QWidget* m_leftCardWidget;  // 左侧输入表单卡片
    QWidget* m_rightCardWidget;  // 右侧表格卡片
    
    // 卡片式布局UI（用于教师和学生）
    QWidget* m_cardViewWidget;  // 卡片视图容器
    ElaScrollArea* m_cardScrollArea;  // 卡片滚动区域
    QWidget* m_cardContainer;  // 卡片容器
    QGridLayout* m_cardLayout;  // 卡片网格布局
    // QVector<ElaInteractiveCard*> m_studentCards;  // 学生卡片列表
    
    // 主题切换防抖
    bool m_isThemeChanging;  // 标记主题切换是否正在进行
    qint64 m_lastThemeChangeTime;  // 上次主题切换的时间戳
    
    // 辅助方法
    void initUI();              // 初始化界面
    void initTable();           // 初始化表格
    void updateTable();         // 更新表格显示
    void updateCardView();      // 更新卡片视图显示
    void clearInputFields();    // 清空输入框
    bool validateInput();       // 验证输入
    int findStudentById(const QString &id);  // 根据学号查找学生
    
protected:
    // 重写事件过滤器，用于防止主题切换按钮在窗口初始化时导致卡死
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // MAINWINDOW_H
