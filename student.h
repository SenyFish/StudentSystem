#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

class Student
{
public:
    Student();
    Student(const QString &id, const QString &name, const QString &gender, 
            int age, const QString &major);

    // Getter方法
    QString getId() const { return id; }
    QString getName() const { return name; }
    QString getGender() const { return gender; }
    int getAge() const { return age; }
    QString getMajor() const { return major; }

    // Setter方法
    void setId(const QString &id) { this->id = id; }
    void setName(const QString &name) { this->name = name; }
    void setGender(const QString &gender) { this->gender = gender; }
    void setAge(int age) { this->age = age; }
    void setMajor(const QString &major) { this->major = major; }

private:
    QString id;       // 学号
    QString name;     // 姓名
    QString gender;   // 性别
    int age;          // 年龄
    QString major;    // 专业
};

#endif // STUDENT_H

