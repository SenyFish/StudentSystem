#include "student.h"

Student::Student()
    : age(0)
{
}

Student::Student(const QString &id, const QString &name, const QString &gender,
                 int age, const QString &major)
    : id(id), name(name), gender(gender), age(age), major(major)
{
}

