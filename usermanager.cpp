#include "usermanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

UserManager::UserManager()
    : hasCurrentUser(false)
{
    // 初始化默认用户
    users.append(User("root", "root", UserRole::Admin, "系统管理员"));
    // 为教师账户绑定专业
    users.append(User("teacher", "teacher", UserRole::Teacher, "教师用户", "", "计算机科学"));
    users.append(User("teacher2", "teacher2", UserRole::Teacher, "软件工程教师", "", "软件工程"));
    users.append(User("teacher3", "teacher3", UserRole::Teacher, "信息安全教师", "", "信息安全"));
}

UserManager& UserManager::getInstance()
{
    static UserManager instance;
    return instance;
}

bool UserManager::login(const QString &username, const QString &password, User &outUser)
{
    for (const User &user : users) {
        if (user.getUsername() == username && user.getPassword() == password) {
            outUser = user;
            setCurrentUser(user);
            return true;
        }
    }
    return false;
}

bool UserManager::loginByStudentId(const QString &studentId, const QString &password, User &outUser)
{
    // 查找关联该学号的用户
    for (const User &user : users) {
        if (user.getStudentId() == studentId && user.getPassword() == password) {
            outUser = user;
            setCurrentUser(user);
            return true;
        }
    }
    return false;
}

bool UserManager::registerUser(const QString &username, const QString &password, 
                               UserRole role, const QString &realName, const QString &studentId, const QString &major)
{
    // 检查用户名是否已存在
    if (userExists(username)) {
        return false;
    }
    
    // 验证用户名和密码
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        return false;
    }
    
    users.append(User(username, password, role, realName, studentId, major));
    return true;
}

bool UserManager::changePassword(const QString &username, const QString &oldPassword, 
                                const QString &newPassword)
{
    int index = findUserIndex(username);
    if (index == -1) {
        return false;
    }
    
    // 验证旧密码
    if (users[index].getPassword() != oldPassword) {
        return false;
    }
    
    // 更新密码
    users[index].setPassword(newPassword);
    
    // 如果是当前用户，更新当前用户信息
    if (hasCurrentUser && currentUser.getUsername() == username) {
        currentUser.setPassword(newPassword);
    }
    
    return true;
}

bool UserManager::deleteUser(const QString &username)
{
    int index = findUserIndex(username);
    if (index == -1) {
        return false;
    }
    
    // 不能删除root用户
    if (username == "root") {
        return false;
    }
    
    users.remove(index);
    return true;
}

bool UserManager::userExists(const QString &username)
{
    return findUserIndex(username) != -1;
}

bool UserManager::getUserByUsername(const QString &username, User &outUser)
{
    int index = findUserIndex(username);
    if (index != -1) {
        outUser = users[index];
        return true;
    }
    return false;
}

int UserManager::findUserIndex(const QString &username)
{
    for (int i = 0; i < users.size(); ++i) {
        if (users[i].getUsername() == username) {
            return i;
        }
    }
    return -1;
}

bool UserManager::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件保存用户数据:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    
    // 写入BOM和表头
    out << "\xEF\xBB\xBF";
    out << "用户名,密码,角色,真实姓名,学号,专业\n";
    
    // 写入用户数据
    for (const User &user : users) {
        out << user.getUsername() << ","
            << user.getPassword() << ","
            << user.getRoleString() << ","
            << user.getRealName() << ","
            << user.getStudentId() << ","
            << user.getMajor() << "\n";
    }
    
    file.close();
    return true;
}

bool UserManager::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件加载用户数据:" << filename;
        return false;
    }
    
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    
    // 跳过BOM和表头
    QString firstLine = in.readLine();
    if (firstLine.startsWith("\xEF\xBB\xBF")) {
        firstLine = firstLine.mid(3);
    }
    
    // 清空现有用户（保留root）
    QVector<User> tempUsers;
    for (const User &user : users) {
        if (user.getUsername() == "root") {
            tempUsers.append(user);
        }
    }
    users = tempUsers;
    
    // 读取用户数据
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }
        
        QStringList fields = line.split(',');
        if (fields.size() < 4) {
            continue;
        }
        
        QString username = fields[0].trimmed();
        QString password = fields[1].trimmed();
        QString roleStr = fields[2].trimmed();
        QString realName = fields[3].trimmed();
        QString studentId = (fields.size() >= 5) ? fields[4].trimmed() : "";  // 兼容旧格式
        QString major = (fields.size() >= 6) ? fields[5].trimmed() : "";  // 兼容旧格式
        
        UserRole role = User::stringToRole(roleStr);
        
        // 跳过root用户（已经存在）
        if (username == "root") {
            continue;
        }
        
        users.append(User(username, password, role, realName, studentId, major));
    }
    
    file.close();
    return true;
}


