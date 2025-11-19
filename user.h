#ifndef USER_H
#define USER_H

#include <QString>

enum class UserRole {
    Student,      // 学生 - 只能查看
    Teacher,      // 教师 - 可以查看、修改学生信息
    Admin         // 管理员 - 完全权限
};

class User
{
public:
    User();
    User(const QString &username, const QString &password, 
         UserRole role, const QString &realName = "", const QString &studentId = "", const QString &major = "");

    // Getter方法
    QString getUsername() const { return username; }
    QString getPassword() const { return password; }
    UserRole getRole() const { return role; }
    QString getRealName() const { return realName; }
    QString getStudentId() const { return studentId; }  // 关联的学生ID（仅学生角色）
    QString getMajor() const { return major; }  // 关联的专业（仅教师角色）
    
    // Setter方法
    void setUsername(const QString &username) { this->username = username; }
    void setPassword(const QString &password) { this->password = password; }
    void setRole(UserRole role) { this->role = role; }
    void setRealName(const QString &realName) { this->realName = realName; }
    void setStudentId(const QString &studentId) { this->studentId = studentId; }
    void setMajor(const QString &major) { this->major = major; }
    
    // 工具方法
    QString getRoleString() const;
    static QString roleToString(UserRole role);
    static UserRole stringToRole(const QString &roleStr);

private:
    QString username;   // 用户名
    QString password;   // 密码（实际应用中应该加密存储）
    UserRole role;      // 用户角色
    QString realName;   // 真实姓名
    QString studentId;  // 关联的学生ID（仅学生角色使用）
    QString major;      // 关联的专业（仅教师角色使用）
};

#endif // USER_H


