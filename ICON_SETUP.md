# 应用程序图标设置说明

## 📋 已完成的配置

本项目已经配置了应用程序图标 `icon.png`，包含以下几个方面：

### 1. ✅ Qt 资源文件（resources.qrc）
- 将 `icon.png` 添加到 Qt 资源系统
- 可以通过 `:/icon.png` 访问

### 2. ✅ 窗口图标设置
在代码中设置了窗口图标：
- `main.cpp`: `QApplication::setWindowIcon(QIcon(":/icon.png"))`
- `mainwindow.cpp`: `setWindowIcon(QIcon(":/icon.png"))`

### 3. ✅ Windows 资源文件（app_icon.rc）
- 为 Windows 平台的可执行文件设置图标
- 在文件资源管理器中显示图标

### 4. ✅ CMakeLists.txt 配置
- 添加 `resources.qrc` 到项目源文件
- 配置 Windows 平台的图标资源

## 🔄 如何更换图标

如果需要更换应用程序图标：

### 方法 1：替换现有图标文件
1. 准备新的图标文件（PNG 格式，推荐 256x256 或 512x512）
2. 替换项目根目录的 `icon.png` 文件
3. 重新编译项目

### 方法 2：使用 .ico 格式（推荐用于 Windows）
1. 将 PNG 转换为 ICO 格式（可使用在线工具）
2. 将 `icon.png` 转换为 `icon.ico`
3. 修改 `app_icon.rc` 文件：
   ```rc
   IDI_ICON1 ICON "icon.ico"
   ```
4. 重新编译项目

## 📐 图标尺寸建议

| 平台      | 推荐尺寸                          | 格式         |
|---------|-------------------------------|------------|
| Windows | 256x256, 128x128, 48x48, 32x32, 16x16 | .ico (多尺寸) |
| Qt 窗口  | 256x256 或更大                   | .png       |
| 任务栏    | 32x32, 48x48                   | .png/.ico  |

## 🛠️ 图标转换工具

### 在线工具
- **ICO Convert**: https://www.icoconverter.com/
- **CloudConvert**: https://cloudconvert.com/png-to-ico
- **Favicon.io**: https://favicon.io/

### 命令行工具
```bash
# 使用 ImageMagick
magick convert icon.png -define icon:auto-resize=256,128,48,32,16 icon.ico

# 使用 FFmpeg
ffmpeg -i icon.png icon.ico
```

## ✅ 验证图标是否生效

### 运行时验证
1. 编译并运行程序
2. 查看窗口标题栏左侧是否显示图标
3. 查看 Windows 任务栏是否显示图标

### 文件验证
1. 编译完成后，在文件资源管理器中查看 `StudentSystem.exe`
2. 应该能看到文件图标
3. 右键 → 属性，查看图标是否正确

## 🐛 常见问题

### 问题 1：图标在运行时不显示
**解决方案**：
- 确认 `resources.qrc` 文件已添加到 `CMakeLists.txt`
- 检查资源路径是否正确（`:/icon.png`）
- 清理并重新编译项目

### 问题 2：.exe 文件没有图标
**解决方案**：
- 确认 `app_icon.rc` 文件存在
- 确认 CMakeLists.txt 中已添加资源配置
- 使用 .ico 格式而不是 .png
- 清理并重新编译 Release 版本

### 问题 3：图标模糊或失真
**解决方案**：
- 使用更高分辨率的原始图标（至少 256x256）
- 使用 .ico 格式，包含多个尺寸
- 确保图标背景透明

## 📝 相关文件

- `icon.png` - 应用程序图标（PNG 格式）
- `resources.qrc` - Qt 资源文件
- `app_icon.rc` - Windows 资源文件
- `main.cpp` - 应用程序图标设置
- `mainwindow.cpp` - 窗口图标设置
- `CMakeLists.txt` - 构建配置

## 🔗 参考资料

- [Qt Application Icon](https://doc.qt.io/qt-6/appicon.html)
- [Qt Resource System](https://doc.qt.io/qt-6/resources.html)
- [Windows ICO Format](https://en.wikipedia.org/wiki/ICO_(file_format))

