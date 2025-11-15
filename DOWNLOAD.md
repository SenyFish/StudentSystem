# 下载和使用指南

## 📥 下载构建产物

### 方式1：从Releases下载（推荐）⭐

👉 [点击这里打开Releases页面](https://github.com/SenyFish/StudentSystem/releases)

#### 最新开发版 (latest)
- **自动更新**：每次代码更新都会自动发布
- **最新功能**：包含所有最新的功能和修复
- **标记**：显示为"Pre-release"（预发布版本）
- **适合**：想要体验最新功能的用户
- **下载**：点击"latest"标签，下载 `StudentSystem-Windows.zip`

#### 正式版本 (v1.0.0等)
- **稳定版本**：经过测试的稳定版本
- **推荐使用**：适合生产环境或日常使用
- **版本号**：使用语义化版本号（v1.0.0、v1.1.0等）
- **下载**：选择对应版本，下载 `StudentSystem-Windows.zip`

### 方式2：从Actions下载

如果你需要特定构建版本：

1. **访问Actions页面**
   👉 [点击这里打开Actions页面](https://github.com/SenyFish/StudentSystem/actions)

2. **选择构建**
   - 找到你需要的构建（按时间排序）
   - 点击进入详情页

3. **下载Artifacts**
   - 滚动到页面底部
   - 找到 **Artifacts** 部分
   - 点击 `StudentSystem-Windows` 下载
   - 注意：Artifacts保留30天后自动删除

### 方式3：命令行下载

使用命令行快速下载最新版本：

```bash
# Windows PowerShell
Invoke-WebRequest -Uri "https://github.com/SenyFish/StudentSystem/releases/download/latest/StudentSystem-Windows.zip" -OutFile "StudentSystem-Windows.zip"

# Linux/macOS
curl -L https://github.com/SenyFish/StudentSystem/releases/download/latest/StudentSystem-Windows.zip -o StudentSystem-Windows.zip
```

文件大小约 80-100MB

## 🚀 使用步骤

### 1. 解压文件

```
右键 StudentSystem-Windows.zip → 解压到当前文件夹
```

或使用命令行：
```powershell
# PowerShell
Expand-Archive StudentSystem-Windows.zip -DestinationPath .\StudentSystem

# 或使用7-Zip
7z x StudentSystem-Windows.zip
```

### 2. 运行程序

进入解压后的文件夹，双击 `StudentSystem.exe` 即可运行！

**快捷方式**（可选）：
- 右键 `StudentSystem.exe` → 发送到 → 桌面快捷方式
- 之后就可以从桌面直接启动

### 3. 开始使用

程序启动后会看到：
- ✅ 左侧：学生信息输入区域
- ✅ 右侧：学生信息表格
- ✅ 已包含3个示例学生数据

## 📋 功能说明

### 添加学生
1. 在左侧输入框填写学生信息
2. 点击"添加学生"按钮

### 删除学生
1. 在学号框输入要删除的学号，或点击表格行自动填充
2. 点击"删除学生"按钮
3. 确认删除

### 修改学生
1. 点击表格中的学生行（信息会自动填充到左侧）
2. 修改需要更改的信息
3. 点击"修改学生"按钮

### 搜索学生
1. 在右上方搜索框输入学号或姓名
2. 点击"搜索"按钮
3. 点击"刷新显示"恢复显示所有学生

## ⚙️ 系统要求

- **操作系统**：Windows 10/11 (64位)
- **磁盘空间**：约100MB
- **内存**：建议2GB以上
- **其他**：无需安装Qt或其他依赖

## ❓ 常见问题

### Q: 提示缺少DLL文件？
**A**: 不应该出现这个问题。如果遇到，请重新下载完整的ZIP包，确保解压了所有文件。

### Q: 程序无法启动？
**A**: 
1. 检查是否解压了完整的ZIP包
2. 确保 `platforms/qwindows.dll` 文件存在
3. 尝试以管理员身份运行
4. 检查Windows Defender是否误报

### Q: 中文显示乱码？
**A**: 确保 `translations/qt_zh_CN.qm` 文件存在，通常不会出现此问题。

### Q: 想要最新功能？
**A**: 每次代码更新后，GitHub Actions会自动构建新版本。访问Actions页面下载最新构建。

### Q: 如何卸载？
**A**: 直接删除整个 `StudentSystem-Windows` 文件夹即可，不会在系统中留下任何文件。

## 📝 数据保存说明

**当前版本**：数据仅保存在内存中，程序关闭后数据会丢失。

**未来版本计划**：
- 支持保存到文件（JSON/CSV）
- 支持数据库存储
- 支持导入/导出功能

## 🔗 相关链接

- **项目主页**：https://github.com/SenyFish/StudentSystem
- **Actions页面**：https://github.com/SenyFish/StudentSystem/actions
- **Releases页面**：https://github.com/SenyFish/StudentSystem/releases
- **问题反馈**：https://github.com/SenyFish/StudentSystem/issues

## 📧 获取帮助

如果遇到问题：
1. 查看 [DEPLOYMENT.md](DEPLOYMENT.md) 了解详细部署说明
2. 查看 [GITHUB_ACTIONS.md](GITHUB_ACTIONS.md) 了解构建流程
3. 在GitHub上提交Issue

## 🎉 享受使用！

感谢使用学生信息管理系统！如果觉得有用，欢迎给项目点个⭐Star！

