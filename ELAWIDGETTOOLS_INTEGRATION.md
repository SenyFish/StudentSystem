# ElaWidgetTools 集成说明

## ✨ 概述

本项目已成功集成 [ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools) - 一个基于 Qt 的现代化 Fluent-UI 组件库。

## 🎨 主要改进

### 1. **现代化的 Fluent-UI 风格**
- 使用 Microsoft Fluent Design 设计语言
- 流畅的动画效果和过渡
- 毛玻璃（Mica）效果支持

### 2. **升级的组件**
原有的 Qt 组件全部替换为 Ela 组件：

| 原组件 | 新组件 | 改进 |
|--------|--------|------|
| QMainWindow | ElaWindow | 无边框窗口，现代化标题栏 |
| QLineEdit | ElaLineEdit | 流畅动画，更好的焦点效果 |
| QPushButton | ElaPushButton | 现代化按钮样式，悬停动画 |
| QComboBox | ElaComboBox | 下拉动画，更美观的选项列表 |
| QSpinBox | ElaSpinBox | 现代化数字输入框 |
| QTableWidget | ElaTableView | 基于 Model/View 架构的表格 |
| QMessageBox | ElaMessageBar | 非侵入式消息提示 |

### 3. **新增功能**
- **ElaMessageBar**: 顶部/右侧弹出式消息提示
- **ElaContentDialog**: 现代化对话框
- **ElaScrollPage**: 滚动页面容器
- **ElaText**: 文本标签组件

## 📦 项目结构

```
StudentSystem/
├── ElaWidgetTools/          # ElaWidgetTools 库（子模块）
│   └── ElaWidgetTools/      # 组件源码
├── main.cpp                 # 程序入口
├── mainwindow.h/cpp         # 主窗口（使用 ElaWindow）
├── student.h/cpp            # 学生数据类
└── CMakeLists.txt           # CMake 配置（已集成 Ela）
```

## 🔧 编译说明

### 前置条件
- Qt 6.x（推荐 6.6.2 或更高）
- CMake 3.16+
- C++17 编译器

### 编译步骤

#### Windows (MinGW)
```bash
cd build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug
cmake ../..
cmake --build . --config Debug -j8
```

#### Windows (MSVC)
```bash
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

### 运行程序
```bash
# 在构建目录中
./StudentSystem.exe
```

## 🎯 功能特性

### 学生信息管理
- ✅ **添加学生**: 输入学号、姓名、性别、年龄、专业
- ✅ **修改学生**: 选择表格行后可修改信息（支持修改学号）
- ✅ **删除学生**: 确认对话框防止误删
- ✅ **搜索学生**: 支持按学号或姓名搜索
- ✅ **排序功能**: 按学号排序
- ✅ **表格交互**: 点击表格行自动填充输入框

### UI 特性
- 🎨 **主题切换**: 支持亮色/暗色主题（可通过 ElaApplication 切换）
- 📱 **响应式布局**: 自适应窗口大小
- ✨ **流畅动画**: 所有操作都有平滑的过渡效果
- 💬 **友好提示**: 非侵入式的消息提示系统
- 🎭 **现代美学**: Fluent Design 设计语言

## 📝 代码示例

### 创建 ElaWindow
```cpp
class MainWindow : public ElaWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr)
        : ElaWindow(parent)
    {
        setWindowTitle("学生信息管理系统");
        setWindowButtonFlags(ElaAppBarType::MinimizeButtonHint | 
                           ElaAppBarType::MaximizeButtonHint | 
                           ElaAppBarType::CloseButtonHint);
    }
};
```

### 使用 ElaMessageBar
```cpp
// 成功消息
ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", 
                      "操作完成！", 3000, this);

// 警告消息
ElaMessageBar::warning(ElaMessageBarType::TopRight, "警告", 
                      "请检查输入！", 2000, this);

// 信息消息
ElaMessageBar::information(ElaMessageBarType::TopRight, "提示", 
                          "这是一条信息", 2000, this);
```

### 使用 ElaContentDialog
```cpp
ElaContentDialog* dialog = new ElaContentDialog(this);
dialog->setWindowTitle("确认");
dialog->setLeftButtonText("取消");
dialog->setRightButtonText("确定");

QWidget* content = new QWidget();
// ... 添加内容 ...
dialog->setCentralWidget(content);

connect(dialog, &ElaContentDialog::rightButtonClicked, [](){
    // 确定按钮点击处理
});
dialog->exec();
```

### 使用 ElaTableView (Model/View)
```cpp
// 创建模型
QStandardItemModel* model = new QStandardItemModel();
model->setHorizontalHeaderLabels({"列1", "列2", "列3"});

// 创建表格视图
ElaTableView* table = new ElaTableView();
table->setModel(model);

// 添加数据
QList<QStandardItem*> row;
row << new QStandardItem("数据1");
row << new QStandardItem("数据2");
row << new QStandardItem("数据3");
model->appendRow(row);
```

## 🎨 主题系统

### 切换主题
```cpp
// 在 main.cpp 中初始化
ElaApplication::getInstance()->init();

// 切换到暗色主题
ElaTheme::getInstance()->setThemeMode(ElaThemeType::Dark);

// 切换到亮色主题
ElaTheme::getInstance()->setThemeMode(ElaThemeType::Light);
```

## 📚 更多资源

- **ElaWidgetTools GitHub**: https://github.com/Liniyous/ElaWidgetTools
- **组件文档**: 查看 `ElaWidgetTools/ElaWidgetToolsExample` 目录中的示例
- **API 参考**: 查看头文件中的详细注释

## 🐛 已知问题

1. 某些 Qt 6.9.0 API 在 ElaWidgetTools 中有弃用警告（不影响使用）
2. 建议使用 Qt 6.6.2 或 6.6.3 以获得最佳兼容性

## 🔄 更新 ElaWidgetTools

```bash
cd ElaWidgetTools
git pull origin main
cd ..
cmake --build build --clean-first
```

## 📄 许可证

- **StudentSystem**: 教育用途
- **ElaWidgetTools**: MIT License

---

**享受现代化的学生管理系统吧！** 🎉

