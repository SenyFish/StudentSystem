# 学生信息管理系统

[![Build Status](https://github.com/SenyFish/StudentSystem/actions/workflows/build.yml/badge.svg)](https://github.com/SenyFish/StudentSystem/actions)
[![Qt](https://img.shields.io/badge/Qt-6.7.3%20LTS-green.svg)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/platform-Windows-blue.svg)](https://github.com/SenyFish/StudentSystem)
[![Release](https://img.shields.io/github/v/release/SenyFish/StudentSystem)](https://github.com/SenyFish/StudentSystem/releases)

一个基于Qt 6的C++学生信息管理系统，提供完整的学生信息增删改查功能。

## 🚀 快速开始

**直接使用**（无需安装Qt）：
1. 下载 `StudentSystem-Windows.zip`（推荐下载"latest"标签）
2. 解压后双击 `StudentSystem.exe` 即可运行

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

- **Qt 6.7.3 LTS** - GUI框架（长期支持版本）
- **C++17** - 编程语言
- **CMake** - 构建系统
- **MinGW/MSVC** - 编译器（本地开发使用MinGW，GitHub Actions使用MSVC）

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

## 示例数据

程序启动时会自动加载以下示例数据：
- 2021001 - 张三 - 男 - 20岁 - 计算机科学
- 2021002 - 李四 - 女 - 19岁 - 软件工程
- 2021003 - 王五 - 男 - 21岁 - 信息安全

## 开发者

本项目作为Qt学习和实践项目开发。

## 许可证

本项目仅供学习和教育用途。

