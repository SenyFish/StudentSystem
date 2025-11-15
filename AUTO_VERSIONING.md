# 自动版本号递增系统

## 🎯 功能说明

本项目已配置自动版本号递增系统，**每次构建成功后自动发布正式版本**，版本号自动递增。

### 版本号规则

- **格式**：v主版本号.0（例如：v1.0, v2.0, v3.0...）
- **递增规则**：每次成功构建后，主版本号自动+1
- **首次发布**：如果没有任何版本，将创建 v1.0

## 🚀 自动发布流程

### 触发条件
每次推送代码到 `main` 分支：
```bash
git push origin main
```

### 自动执行的步骤

1. **📥 检出代码** - 获取最新代码
2. **🔧 编译项目** - 使用Qt 6.6.3 + MSVC构建
3. **📦 部署依赖** - windeployqt打包所有依赖
4. **📂 打包文件** - 创建完整安装包
5. **🔢 计算版本号** - 获取最新版本并+1
6. **🏷️ 创建标签** - 自动创建并推送版本tag
7. **🎉 发布版本** - 创建正式Release（非预发布）
8. **⬆️ 上传文件** - 上传安装包到Release

## 📦 版本号示例

```
第1次构建 → v1.0
第2次构建 → v2.0  
第3次构建 → v3.0
第4次构建 → v4.0
...
```

### 实际场景

```bash
# 情景1：首次构建
git push origin main
# 结果：创建 v1.0 正式版本

# 情景2：修复bug后
git commit -m "修复学生信息显示bug"
git push origin main
# 结果：自动创建 v2.0 正式版本

# 情景3：添加新功能
git commit -m "添加导出功能"
git push origin main
# 结果：自动创建 v3.0 正式版本
```

## 🎉 优势

### 对开发者
- ✅ **无需手动创建tag** - 系统自动处理
- ✅ **无需手动发布** - 推送即发布
- ✅ **版本号自动管理** - 永不重复，按序递增
- ✅ **专注代码开发** - 不用操心发布流程

### 对用户
- ✅ **始终获取最新版** - 每次构建都是新版本
- ✅ **正式版本** - 不是预发布版，可放心使用
- ✅ **版本号清晰** - v1.0、v2.0、v3.0...一目了然
- ✅ **下载方便** - 从Releases页面直接下载

## 📥 用户如何下载

### 从Releases页面
访问：https://github.com/SenyFish/StudentSystem/releases

你会看到按时间排序的版本列表：
- 🆕 **v3.0** - 最新版本（3小时前）
- **v2.0** - 前一个版本（1天前）
- **v1.0** - 初始版本（2天前）

### 直接下载最新版
```bash
# PowerShell
Invoke-WebRequest -Uri "https://github.com/SenyFish/StudentSystem/releases/latest/download/StudentSystem-Windows.zip" -OutFile "StudentSystem.zip"

# Linux/macOS
curl -L https://github.com/SenyFish/StudentSystem/releases/latest/download/StudentSystem-Windows.zip -o StudentSystem.zip
```

## 🔍 版本号计算逻辑

### PowerShell脚本
```powershell
# 获取最新版本
$latestRelease = gh release list --limit 1 --json tagName

# 提取版本号并递增
if ($latestRelease -match 'v(\d+)\.(\d+)') {
    $major = [int]$matches[1]
    $newVersion = "v$($major + 1).0"
} else {
    $newVersion = "v1.0"
}

# 创建新版本
git tag -a $newVersion -m "Auto-release $newVersion"
git push origin $newVersion
```

## 📝 Release信息

每个自动发布的Release都包含：

### 版本信息
- 📦 版本号（如 v2.0）
- 📅 构建时间
- 🔨 提交信息
- 👤 提交者
- 🔗 Commit SHA
- 🎨 UI框架版本
- 🛠️ Qt版本

### 下载内容
- `StudentSystem-Windows.zip` - 完整安装包
  - 包含所有Qt依赖
  - 包含ElaWidgetTools库
  - 包含文档
  - 解压即用

### 说明文档
- 功能特性列表
- 下载和使用说明
- 系统要求
- 技术栈信息

## ⚙️ 技术细节

### GitHub Actions配置
- **权限**：`contents: write` - 允许创建Release和tag
- **触发条件**：`push to main` - 推送到main分支
- **工具**：`gh cli` - 获取最新版本信息
- **Action**：`softprops/action-gh-release@v1` - 创建Release

### 版本标签管理
- **自动创建**：每次构建自动创建git tag
- **自动推送**：tag自动推送到GitHub
- **命名规范**：v{major}.0
- **提交信息**：Auto-release v{version}

## 🛠️ 维护说明

### 如果需要回退版本
虽然系统自动递增，但你仍可以手动管理：

```bash
# 删除远程tag（如果发布了有问题的版本）
git push --delete origin v3.0

# 删除本地tag
git tag -d v3.0

# 删除Release
# 访问 https://github.com/SenyFish/StudentSystem/releases
# 手动删除对应的Release
```

### 如果需要跳过发布
有时你可能想推送代码但不发布：

**方法1**：推送到其他分支
```bash
git push origin feature-branch
# 不会触发自动发布
```

**方法2**：在commit消息中添加 `[skip ci]`
```bash
git commit -m "文档更新 [skip ci]"
git push origin main
# 将跳过CI/CD流程
```

### 修改版本号格式
如需修改版本号格式（如改为 v1.0.0），编辑 `.github/workflows/build.yml`：

```yaml
- name: 🔢 自动计算新版本号
  run: |
    # 修改这里的逻辑
    $newVersion = "v$newMajor.0.0"  # 改为三位版本号
```

## 📊 版本历史查看

### 查看所有版本
```bash
# 命令行
gh release list

# 或访问
# https://github.com/SenyFish/StudentSystem/releases
```

### 查看特定版本
```bash
# 查看v2.0的详情
gh release view v2.0

# 下载v2.0的安装包
gh release download v2.0
```

## 🎊 总结

### 核心特点
- ✅ **完全自动化** - 无需人工干预
- ✅ **版本号递增** - v1.0 → v2.0 → v3.0...
- ✅ **正式版本** - 每个版本都是正式发布
- ✅ **简单易用** - 推送代码即发布

### 使用流程
```
开发代码 → 提交 → 推送 → 自动构建 → 自动发布 → 用户下载
```

### 适用场景
- ✅ 快速迭代开发
- ✅ 持续集成/持续部署(CI/CD)
- ✅ 自动化发布流程
- ✅ 版本管理自动化

---

**配置完成！** 现在每次推送代码都会自动创建新的正式版本！🚀

