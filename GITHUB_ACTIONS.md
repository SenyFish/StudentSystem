# GitHub Actions 自动化构建和发布

本项目配置了GitHub Actions自动化工作流，可以自动编译、打包和发布Qt应用程序的Windows版本。

## 功能特性

✅ **自动编译** - 每次推送到main分支时自动编译  
✅ **Windows平台** - 自动构建Windows版本（已配置MinGW编译环境）  
✅ **自动部署** - 自动使用windeployqt打包所有Qt依赖  
✅ **构建产物** - 自动生成可直接运行的Windows安装包  
✅ **自动发布** - 推送tag时自动创建GitHub Release  
✅ **完全汉化** - 工作流配置文件已完全汉化，易于理解  

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

本项目仅构建Windows版本，构建过程如下：

#### Windows自动构建流程
1. 📥 **检出代码** - 从GitHub仓库获取最新代码
2. 🔧 **安装Qt 6.9.0** - 自动安装Qt开发环境（MinGW 64位）
3. ⚙️ **配置CMake** - 使用MinGW Makefiles生成构建文件
4. 🔨 **编译项目** - 编译Release版本
5. 📦 **部署Qt依赖** - 使用windeployqt自动部署所有DLL和插件
6. 📂 **打包发布文件** - 复制所有必需文件并打包成ZIP
7. ⬆️ **上传构建产物** - 上传到GitHub Actions（保留30天）
8. 🚀 **创建Release** - 如果是版本标签，自动创建GitHub Release

## 如何使用

### 1. 查看构建状态

在GitHub仓库页面，可以看到构建状态徽章。点击 **Actions** 标签查看详细日志。

### 2. 下载构建产物

构建完成后：
1. 进入 **Actions** 标签
2. 选择一个成功的工作流运行
3. 在 **Artifacts** 部分下载：
   - `StudentSystem-Windows.zip` - Windows完整安装包（包含所有依赖）

**说明**：解压后即可在任何Windows电脑上直接运行，无需安装Qt环境！

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
- 上传Windows安装包
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
这是一个完整的Windows安装包，包含以下内容：

**主程序**：
- `StudentSystem.exe` - 学生管理系统主程序

**Qt运行时库**：
- `Qt6Core.dll` - Qt核心库
- `Qt6Gui.dll` - Qt GUI库
- `Qt6Widgets.dll` - Qt Widgets库
- `Qt6Network.dll` - Qt网络库
- `Qt6Svg.dll` - Qt SVG库

**MinGW运行时**：
- `libgcc_s_seh-1.dll` - GCC运行时
- `libstdc++-6.dll` - C++标准库
- `libwinpthread-1.dll` - 线程库

**Qt插件**：
- `platforms/` - Windows平台插件
- `styles/` - 现代Windows样式插件
- `imageformats/` - 图像格式支持（GIF、JPEG、PNG、SVG等）
- `iconengines/` - SVG图标引擎
- `generic/` - 通用插件
- `networkinformation/` - 网络信息插件
- `tls/` - TLS/SSL支持

**翻译文件**：
- `translations/` - 包含中文简体等多语言翻译

**文档**：
- `README.md` - 项目说明文档
- `DEPLOYMENT.md` - 部署说明文档

**使用方式**：
1. 解压ZIP文件到任意目录
2. 双击运行 `StudentSystem.exe`
3. 无需安装Qt或其他依赖！

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

### Q5: 想要添加Linux或macOS构建
**A**: 可以参考Qt官方文档添加对应平台的构建job。当前版本仅构建Windows版本以简化流程。

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
- 每次Windows构建大约消耗5-10分钟

## 参考资源

- [GitHub Actions 官方文档](https://docs.github.com/en/actions)
- [install-qt-action 文档](https://github.com/jurplel/install-qt-action)
- [Qt CMake 文档](https://doc.qt.io/qt-6/cmake-manual.html)

---

配置完成后，每次推送代码都会自动构建，非常方便！🚀

