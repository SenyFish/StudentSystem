# 🚨 运行程序遇到问题？请阅读此文档

## 问题：程序无法运行或没有界面显示

### ✅ 解决方案

#### 方案 1：使用 Qt Creator 运行（最简单）

1. 打开 Qt Creator
2. 打开项目：`文件` → `打开文件或项目` → 选择 `CMakeLists.txt`
3. **重要**：选择 **Release** 构建模式（不要用 Debug）
4. 点击运行按钮（绿色三角形▶️）

#### 方案 2：命令行编译 Release 版本

```bash
# 1. 创建 Release 构建目录
mkdir build-release
cd build-release

# 2. 配置项目（Release 模式）
cmake .. -DCMAKE_BUILD_TYPE=Release

# 3. 编译
cmake --build . --config Release

# 4. 复制 DLL
copy ElaWidgetTools\ElaWidgetTools\ElaWidgetTools.dll .

# 5. 运行
.\StudentSystem.exe
```

#### 方案 3：使用现成的 Release 包

如果编译太麻烦，可以从 GitHub Actions 下载自动编译的 Release 版本：

1. 访问：https://github.com/YourUsername/StudentSystem/releases
2. 下载 `StudentSystem-Windows.zip`
3. 解压后直接运行 `StudentSystem.exe`

## 🐛 Debug 模式的问题

Debug 模式需要 Qt 的 Debug DLL（Qt6Cored.dll, Qt6Widgetsd.dll 等），这些DLL通常很大且不包含在标准安装中。

**建议**：开发和测试都使用 Release 模式。

## 📝 如果还是不行

1. 确保安装了 Qt 6.6.3 或更高版本
2. 确保 `ElaWidgetTools.dll` 在程序同一目录
3. 在命令行运行查看错误信息：
   ```bash
   cd build-release
   .\StudentSystem.exe
   ```

## 🆘 需要帮助

如果以上方法都不行，请提供：
- 错误消息（如果有）
- Qt 版本
- 编译器版本
- 操作系统版本

