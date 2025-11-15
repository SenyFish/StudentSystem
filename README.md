# 学生信息管理系统

[![Build Status](https://github.com/SenyFish/StudentSystem/actions/workflows/build.yml/badge.svg)](https://github.com/SenyFish/StudentSystem/actions)
[![Qt](https://img.shields.io/badge/Qt-6.9.0-green.svg)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/platform-Windows-blue.svg)](https://github.com/SenyFish/StudentSystem)
[![Release](https://img.shields.io/github/v/release/SenyFish/StudentSystem)](https://github.com/SenyFish/StudentSystem/releases)

一个基于Qt 6的C++学生信息管理系统，提供完整的学生信息增删改查功能。

## 功能特性

### 主要功能
- ✅ **添加学生**：录入新学生的信息（学号、姓名、性别、年龄、专业）
- ✅ **删除学生**：根据学号删除学生信息
- ✅ **修改学生**：更新已存在学生的信息
- ✅ **查询学生**：支持按学号或姓名搜索学生
- ✅ **表格显示**：以表格形式展示所有学生信息
- ✅ **数据验证**：输入数据的完整性检查
- ✅ **交互操作**：点击表格行自动填充输入框

### 界面特点
- 左侧：学生信息输入区域和操作按钮
- 右侧：学生信息表格显示和搜索功能
- 友好的用户提示和确认对话框
- 现代化的UI布局

## 技术栈

- **Qt 6.9.0** - GUI框架
- **C++17** - 编程语言
- **CMake** - 构建系统
- **MinGW** - 编译器

## 项目结构

```
StudentSystem/
├── main.cpp              # 程序入口
├── mainwindow.h          # 主窗口头文件
├── mainwindow.cpp        # 主窗口实现
├── mainwindow.ui         # UI设计文件
├── student.h             # 学生类头文件
├── student.cpp           # 学生类实现
├── CMakeLists.txt        # CMake配置文件
└── README.md             # 项目说明文档
```

## 编译和运行

### 前置条件
- Qt 6.x
- CMake 3.16或更高版本
- C++17兼容的编译器（MinGW/MSVC/GCC/Clang）

### 构建步骤

1. **创建构建目录**
```bash
mkdir build
cd build
```

2. **配置项目**
```bash
cmake ..
```

3. **编译项目**
```bash
cmake --build .
```

4. **运行程序**
```bash
./StudentSystem.exe  # Windows
./StudentSystem      # Linux/macOS
```

### 使用Qt Creator
1. 打开Qt Creator
2. 选择"文件" > "打开文件或项目"
3. 选择`CMakeLists.txt`文件
4. 配置项目（选择合适的Kit）
5. 点击"运行"按钮

## 使用说明

### 添加学生
1. 在左侧输入框中填写学生信息
2. 点击"添加学生"按钮
3. 系统会检查学号是否重复

### 删除学生
1. 在"学号"输入框中输入要删除的学号，或点击表格中的行自动填充
2. 点击"删除学生"按钮
3. 确认删除操作

### 修改学生
1. 点击表格中的学生行，信息会自动填充到左侧输入框
2. 修改需要更改的信息
3. 点击"修改学生"按钮

### 搜索学生
1. 在右上方搜索框中输入学号或姓名关键词
2. 点击"搜索"按钮
3. 表格将显示匹配的结果
4. 点击"刷新显示"恢复显示所有学生

### 清空输入
- 点击"清空输入"按钮可清空所有输入框

## 数据模型

### Student类
```cpp
class Student {
    QString id;       // 学号
    QString name;     // 姓名
    QString gender;   // 性别
    int age;          // 年龄
    QString major;    // 专业
};
```

## 示例数据

程序启动时会自动加载以下示例数据：
- 2021001 - 张三 - 男 - 20岁 - 计算机科学
- 2021002 - 李四 - 女 - 19岁 - 软件工程
- 2021003 - 王五 - 男 - 21岁 - 信息安全

## 自动化构建

本项目配置了GitHub Actions自动化构建流程：

### 🚀 自动构建特性
- ✅ 每次推送代码自动编译
- ✅ 自动使用windeployqt打包所有依赖
- ✅ 生成可直接运行的Windows安装包
- ✅ 推送版本标签自动创建Release

### 📥 下载构建产物
1. 访问 [Actions页面](https://github.com/SenyFish/StudentSystem/actions)
2. 点击最新成功的构建
3. 下载 `StudentSystem-Windows.zip`
4. 解压后即可直接运行，无需安装Qt！

### 🏷️ 发布新版本
```bash
# 创建版本标签
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0

# 自动触发构建并创建Release
```

查看详细说明：[GitHub Actions使用指南](GITHUB_ACTIONS.md)

## 未来改进计划

- [ ] 数据持久化（保存到文件或数据库）
- [ ] 导入/导出功能（CSV/Excel）
- [ ] 数据统计和报表
- [ ] 更多的搜索过滤条件
- [ ] 学生成绩管理
- [ ] 用户权限管理

## 开发者

本项目作为Qt学习和实践项目开发。

## 许可证

本项目仅供学习和教育用途。

