# Qt应用程序部署说明

## 问题说明

在Qt Creator中可以正常运行程序，但直接双击exe文件时会提示找不到DLL文件，如：
- Qt6Core.dll
- Qt6Widgets.dll
- Qt6Gui.dll

这是因为Qt Creator会自动设置环境变量，而直接运行exe时找不到Qt的动态链接库。

## 解决方案：使用windeployqt工具

### 方法1：使用完整路径（推荐）

```powershell
# 进入exe所在目录
cd E:\File\MyFirstQT\StudentSystem\build\Desktop_Qt_6_9_0_MinGW_64_bit-Release

# 使用完整路径运行windeployqt
C:\Qt\6.9.0\mingw_64\bin\windeployqt.exe StudentSystem.exe
```

### 方法2：添加Qt到系统PATH

1. 将 `C:\Qt\6.9.0\mingw_64\bin` 添加到系统环境变量PATH
2. 打开命令提示符或PowerShell
3. 运行：
```powershell
cd E:\File\MyFirstQT\StudentSystem\build\Desktop_Qt_6_9_0_MinGW_64_bit-Release
windeployqt StudentSystem.exe
```

## windeployqt做了什么？

该工具会自动：
1. 分析exe文件的依赖
2. 复制所有需要的Qt DLL文件到exe所在目录
3. 复制Qt插件（platforms、styles等）
4. 复制必要的运行时库
5. 复制翻译文件

## 部署后的文件结构

```
Desktop_Qt_6_9_0_MinGW_64_bit-Release/
├── StudentSystem.exe          # 主程序
├── Qt6Core.dll               # Qt核心库
├── Qt6Gui.dll                # Qt GUI库
├── Qt6Widgets.dll            # Qt Widgets库
├── Qt6Network.dll            # Qt网络库
├── Qt6Svg.dll                # Qt SVG库
├── libgcc_s_seh-1.dll        # MinGW运行时
├── libstdc++-6.dll           # C++标准库
├── libwinpthread-1.dll       # 线程库
├── opengl32sw.dll            # OpenGL软件渲染
├── D3Dcompiler_47.dll        # DirectX编译器
├── platforms/                 # 平台插件
│   └── qwindows.dll          # Windows平台插件
├── styles/                    # 样式插件
│   └── qmodernwindowsstyle.dll
├── imageformats/              # 图像格式插件
│   ├── qgif.dll
│   ├── qico.dll
│   ├── qjpeg.dll
│   └── qsvg.dll
├── iconengines/               # 图标引擎
│   └── qsvgicon.dll
├── generic/                   # 通用插件
├── networkinformation/        # 网络信息插件
├── tls/                       # TLS插件
└── translations/              # 翻译文件
    ├── qt_zh_CN.qm           # 中文简体
    ├── qt_zh_TW.qm           # 中文繁体
    └── ...
```

## 发布应用程序

如果要将应用程序分发给其他用户：

### 选项1：打包整个目录
将整个Release目录打包成ZIP文件，用户解压后直接运行StudentSystem.exe即可。

### 选项2：使用安装程序制作工具
可以使用以下工具创建安装程序：
- **Inno Setup**（免费）
- **NSIS**（免费）
- **Qt Installer Framework**（Qt官方）
- **Advanced Installer**（商业软件）

## 优化部署大小

如果觉得文件太大，可以使用以下参数：

```powershell
# 只部署必需的插件
windeployqt --no-translations --no-svg --no-opengl-sw StudentSystem.exe

# 查看所有选项
windeployqt --help
```

## 调试版本 vs 发布版本

- **Debug版本**：用于开发和调试，文件较大
  - 路径：`build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/`
  
- **Release版本**：用于发布，已优化，文件较小
  - 路径：`build/Desktop_Qt_6_9_0_MinGW_64_bit-Release/`

**建议**：发布给用户时使用Release版本。

## 常见问题

### Q1: 运行时提示"无法启动程序，因为计算机中丢失libgcc_s_seh-1.dll"
**A**: 运行windeployqt即可，它会自动复制MinGW的运行时库。

### Q2: 程序可以启动，但中文显示乱码
**A**: 确保translations目录中有qt_zh_CN.qm文件。

### Q3: 想要更小的安装包
**A**: 使用UPX压缩工具压缩exe和dll文件，或使用上述优化参数。

### Q4: 在没有Qt的电脑上无法运行
**A**: 使用windeployqt部署后，目标电脑不需要安装Qt即可运行。

## 已完成的部署

✅ 本项目的Release版本已经完成部署，可以直接运行：
```
E:\File\MyFirstQT\StudentSystem\build\Desktop_Qt_6_9_0_MinGW_64_bit-Release\StudentSystem.exe
```

现在可以将整个Release目录复制到任何Windows电脑上运行！

