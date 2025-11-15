# GitHub Actions 自动化构建和发布

本项目配置了GitHub Actions自动化工作流，可以自动编译、打包和发布Qt应用程序。

## 功能特性

✅ **自动编译** - 每次推送到main分支时自动编译  
✅ **多平台支持** - 支持Windows、Linux、macOS三个平台  
✅ **自动部署** - 自动使用windeployqt/macdeployqt打包依赖  
✅ **构建产物** - 自动生成可分发的压缩包  
✅ **自动发布** - 推送tag时自动创建GitHub Release  

## 工作流说明

### 触发条件

工作流会在以下情况下自动运行：

1. **推送到main分支**
   ```bash
   git push origin main
   ```

2. **创建Pull Request**
   ```bash
   # 当有PR指向main分支时
   ```

3. **推送版本标签**
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```

4. **手动触发**
   - 在GitHub仓库页面，进入 Actions 标签
   - 选择 "Build and Release Qt Application"
   - 点击 "Run workflow"

### 构建步骤

#### Windows构建
1. 检出代码
2. 安装Qt 6.9.0 (MinGW)
3. 使用CMake配置项目
4. 编译Release版本
5. 使用windeployqt部署Qt依赖
6. 打包成ZIP文件
7. 上传构建产物

#### Linux构建
1. 检出代码
2. 安装Qt 6.9.0
3. 安装系统依赖
4. 使用CMake配置和编译
5. 打包成tar.gz文件
6. 上传构建产物

#### macOS构建
1. 检出代码
2. 安装Qt 6.9.0
3. 使用CMake配置和编译
4. 使用macdeployqt创建DMG文件
5. 上传构建产物

## 如何使用

### 1. 查看构建状态

在GitHub仓库页面，可以看到构建状态徽章。点击 **Actions** 标签查看详细日志。

### 2. 下载构建产物

构建完成后：
1. 进入 **Actions** 标签
2. 选择一个成功的工作流运行
3. 在 **Artifacts** 部分下载：
   - `StudentSystem-Windows.zip` - Windows版本
   - `StudentSystem-Linux.tar.gz` - Linux版本
   - `StudentSystem-macOS` - macOS版本

### 3. 创建正式发布版本

当你准备发布新版本时：

```bash
# 1. 确保代码已提交
git add .
git commit -m "准备发布v1.0.0版本"

# 2. 创建并推送tag
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0
```

这将自动：
- 触发构建流程
- 创建GitHub Release
- 上传所有平台的安装包
- 生成Release说明

## 添加构建状态徽章

在 `README.md` 中添加以下代码显示构建状态：

```markdown
[![Build Status](https://github.com/SenyFish/StudentSystem/actions/workflows/build.yml/badge.svg)](https://github.com/SenyFish/StudentSystem/actions)
```

效果：
[![Build Status](https://github.com/SenyFish/StudentSystem/actions/workflows/build.yml/badge.svg)](https://github.com/SenyFish/StudentSystem/actions)

## 配置说明

### 修改Qt版本

编辑 `.github/workflows/build.yml`，修改Qt版本：

```yaml
- name: 安装Qt
  uses: jurplel/install-qt-action@v4
  with:
    version: '6.9.0'  # 改为你需要的版本，如 '6.5.3'
```

### 添加更多Qt模块

**注意**：Qt 6的基础组件（QtCore、QtGui、QtWidgets等）是默认安装的，**不需要**在modules中指定。

如果需要额外的Qt模块（如QtCharts、QtMultimedia等），只需添加**非默认**模块：

```yaml
- name: 安装Qt
  uses: jurplel/install-qt-action@v4
  with:
    version: '6.9.0'
    modules: 'qtcharts qtmultimedia qtnetworkauth'  # 只添加额外需要的模块
    cache: true
```

**常见的额外模块**：
- `qtcharts` - 图表组件
- `qtdatavisualization` - 数据可视化
- `qtmultimedia` - 多媒体功能
- `qtnetworkauth` - 网络认证
- `qtpositioning` - 定位服务
- `qtwebsockets` - WebSocket支持

**查看可用模块**：
```bash
# Linux
aqt list-qt linux desktop --modules 6.9.0 gcc_64

# Windows
aqt list-qt windows desktop --modules 6.9.0 win64_mingw

# macOS
aqt list-qt mac desktop --modules 6.9.0 clang_64
```

### 修改触发条件

只在特定分支触发：

```yaml
on:
  push:
    branches: [ main, develop ]  # 添加其他分支
```

## 使用的GitHub Actions

- **[actions/checkout@v4](https://github.com/actions/checkout)** - 检出代码
- **[jurplel/install-qt-action@v4](https://github.com/jurplel/install-qt-action)** - 安装Qt
- **[actions/upload-artifact@v4](https://github.com/actions/upload-artifact)** - 上传构建产物
- **[softprops/action-gh-release@v1](https://github.com/softprops/action-gh-release)** - 创建GitHub Release

## 构建产物说明

### Windows版本 (StudentSystem-Windows.zip)
包含内容：
- `StudentSystem.exe` - 主程序
- 所有Qt DLL文件
- `platforms/` - 平台插件
- `styles/` - 样式插件
- `imageformats/` - 图像格式插件
- `translations/` - 翻译文件
- `README.md` - 说明文档

用户只需解压即可运行，无需安装Qt。

### Linux版本 (StudentSystem-Linux.tar.gz)
包含编译好的二进制文件，需要系统安装了Qt库或使用AppImage格式。

### macOS版本 (StudentSystem.dmg)
包含完整的.app应用包，可直接安装使用。

## 本地测试工作流

使用 [act](https://github.com/nektos/act) 在本地测试GitHub Actions：

```bash
# 安装act
choco install act  # Windows
# 或
brew install act   # macOS

# 运行工作流
act push
```

## 常见问题

### Q1: 构建失败，提示找不到Qt模块 (qtbase, qtwidgets)
**A**: Qt 6的基础组件（QtCore、QtGui、QtWidgets）是默认安装的，**不要**在`modules`参数中指定它们。如果看到类似错误：
```
ERROR: The packages ['qtbase', 'qtwidgets'] were not found
```
请从配置中移除`modules`参数，或者只添加真正需要的额外模块（如qtcharts、qtmultimedia）。

### Q2: 构建失败，提示找不到Qt版本
**A**: 检查Qt版本是否正确，某些版本可能不可用。访问 [install-qt-action](https://github.com/jurplel/install-qt-action) 查看支持的版本。

### Q3: Windows构建失败，提示CMake错误
**A**: 确保CMakeLists.txt正确配置了MinGW编译器。

### Q4: Release未自动创建
**A**: 确保推送了tag，且tag名称以'v'开头（如v1.0.0）。检查GITHUB_TOKEN权限。

### Q5: 想要禁用某个平台的构建
**A**: 在build.yml中注释掉或删除对应的job即可。

### Q6: 构建产物保留时间
**A**: 默认保留30天，可修改 `retention-days` 参数。

## 进阶配置

### 添加代码签名（Windows）

```yaml
- name: 签名应用程序
  run: |
    signtool sign /f cert.pfx /p ${{ secrets.CERT_PASSWORD }} build/StudentSystem.exe
```

### 自动上传到其他平台

可以添加步骤将构建产物上传到：
- 云存储（阿里云OSS、AWS S3）
- 应用商店
- 自己的服务器

### 添加单元测试

```yaml
- name: 运行测试
  run: |
    cd build
    ctest --output-on-failure
```

## 成本说明

- GitHub Actions对**公开仓库完全免费**，无限制使用
- 私有仓库每月有免费配额（2000分钟）
- 多平台构建会消耗更多分钟数，但对公开仓库无影响

## 参考资源

- [GitHub Actions 官方文档](https://docs.github.com/en/actions)
- [install-qt-action 文档](https://github.com/jurplel/install-qt-action)
- [Qt CMake 文档](https://doc.qt.io/qt-6/cmake-manual.html)

---

配置完成后，每次推送代码都会自动构建，非常方便！🚀

