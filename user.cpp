#include "user.h"

User::User()
    : role(UserRole::Student)
{
}

User::User(const QString &username, const QString &password, 
           UserRole role, const QString &realName, const QString &studentId, const QString &major)
    : username(username)
    , password(password)
    , role(role)
    , realName(realName)
    , studentId(studentId)
    , major(major)
{
}

QString User::getRoleString() const
{
    return roleToString(role);
}

QString User::roleToString(UserRole role)
{
    switch (role) {
        case UserRole::Student:
            return "学生";
        case UserRole::Teacher:
            return "教师";
        case UserRole::Admin:
            return "管理员";
        default:
            return "未知";
    }
}

UserRole User::stringToRole(const QString &roleStr)
{
    if (roleStr == "学生") {
        return UserRole::Student;
    } else if (roleStr == "教师") {
        return UserRole::Teacher;
    } else if (roleStr == "管理员") {
        return UserRole::Admin;
    }
    return UserRole::Student;  // 默认角色
}


