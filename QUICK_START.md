# 快速开始指南

## 🚀 自动化构建已配置

你的项目现在支持自动化构建和发布！

### 查看构建状态

访问：https://github.com/SenyFish/StudentSystem/actions

GitHub Actions会在以下情况自动运行：
- ✅ 推送代码到main分支
- ✅ 创建Pull Request
- ✅ 推送版本标签
- ✅ 手动触发

### 下载构建产物

1. 进入 [Actions页面](https://github.com/SenyFish/StudentSystem/actions)
2. 点击最新的成功构建
3. 在底部 **Artifacts** 下载：
   - `StudentSystem-Windows.zip` - Windows版本（已包含所有DLL）
   - `StudentSystem-Linux.tar.gz` - Linux版本
   - `StudentSystem-macOS` - macOS版本

### 发布新版本

```bash
# 1. 确保代码已提交
git add .
git commit -m "准备发布v1.0.0"
git push

# 2. 创建版本标签
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0

# 3. 自动创建Release
# GitHub Actions会自动编译并创建Release
# 访问 https://github.com/SenyFish/StudentSystem/releases 查看
```

## 📦 构建产物说明

### Windows版本
- 完整打包，包含所有Qt依赖
- 解压即用，无需安装Qt
- 使用windeployqt自动部署

### Linux版本
- 编译好的二进制文件
- 需要系统安装Qt库
- 或使用AppImage格式（待实现）

### macOS版本
- 标准DMG安装包
- 包含完整.app应用
- 使用macdeployqt打包

## 🛠️ 本地开发

### 构建Release版本

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### 部署Qt依赖（Windows）

```bash
# 使用Qt的bin目录路径
C:\Qt\6.9.0\mingw_64\bin\windeployqt.exe StudentSystem.exe
```

### 手动打包

```bash
# 创建发布目录
mkdir StudentSystem-Release

# 复制文件
copy StudentSystem.exe StudentSystem-Release\
copy *.dll StudentSystem-Release\
xcopy /E /I platforms StudentSystem-Release\platforms
xcopy /E /I styles StudentSystem-Release\styles

# 压缩
Compress-Archive StudentSystem-Release StudentSystem-Release.zip
```

## 📚 文档导航

- **[README.md](README.md)** - 项目介绍和使用说明
- **[DEPLOYMENT.md](DEPLOYMENT.md)** - 本地部署指南
- **[GITHUB_ACTIONS.md](GITHUB_ACTIONS.md)** - GitHub Actions详细说明
- **[QUICK_START.md](QUICK_START.md)** - 本文档

## 🎯 下一步

### 立即可以做的：
1. ✅ 推送代码触发自动构建
2. ✅ 在Actions页面查看构建日志
3. ✅ 下载构建产物测试
4. ✅ 创建v1.0.0标签发布首个版本

### 未来改进：
- [ ] 添加单元测试到CI流程
- [ ] 配置代码签名（Windows/macOS）
- [ ] 创建Linux AppImage
- [ ] 添加代码覆盖率报告
- [ ] 设置自动发布到应用商店

## 💡 提示

**首次推送后**，去Actions页面查看构建过程：
```
https://github.com/SenyFish/StudentSystem/actions
```

**创建首个Release**：
```bash
git tag -a v1.0.0 -m "First release"
git push origin v1.0.0
```

然后访问：
```
https://github.com/SenyFish/StudentSystem/releases
```

## 🆘 需要帮助？

- 查看 [GITHUB_ACTIONS.md](GITHUB_ACTIONS.md) 了解详细配置
- 查看 [DEPLOYMENT.md](DEPLOYMENT.md) 解决部署问题
- 查看GitHub Actions运行日志定位问题

---

配置完成！现在每次推送代码都会自动构建，非常方便！🎉

