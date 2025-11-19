#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include <QVector>
#include <QString>

class UserManager
{
public:
    static UserManager& getInstance();
    
    // 用户认证
    bool login(const QString &username, const QString &password, User &outUser);
    bool loginByStudentId(const QString &studentId, const QString &password, User &outUser);  // 按学号登录
    bool registerUser(const QString &username, const QString &password, 
                     UserRole role, const QString &realName = "", const QString &studentId = "", const QString &major = "");
    
    // 用户管理
    bool changePassword(const QString &username, const QString &oldPassword, 
                       const QString &newPassword);
    bool deleteUser(const QString &username);
    bool userExists(const QString &username);
    
    // 查询
    QVector<User> getAllUsers() const { return users; }
    bool getUserByUsername(const QString &username, User &outUser);
    
    // 数据持久化
    bool saveToFile(const QString &filename);
    bool loadFromFile(const QString &filename);
    
    // 当前登录用户
    void setCurrentUser(const User &user) { currentUser = user; hasCurrentUser = true; }
    User getCurrentUser() const { return currentUser; }
    bool hasUser() const { return hasCurrentUser; }
    void logout() { hasCurrentUser = false; }

private:
    UserManager();  // 单例模式
    ~UserManager() = default;
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;
    
    int findUserIndex(const QString &username);
    
    QVector<User> users;
    User currentUser;
    bool hasCurrentUser;
};

#endif // USERMANAGER_H


