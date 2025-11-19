#include "operationlog.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

OperationLog::OperationLog()
    : type(OperationType::Add)
    , timestamp(QDateTime::currentDateTime())
{
}

OperationLog::OperationLog(OperationType type, const QString &username, 
                           const QString &detail, const QDateTime &timestamp)
    : type(type)
    , username(username)
    , detail(detail)
    , timestamp(timestamp)
{
}

QString OperationLog::getTypeString() const
{
    return typeToString(type);
}

QString OperationLog::typeToString(OperationType type)
{
    switch (type) {
        case OperationType::Add:
            return "添加学生";
        case OperationType::Delete:
            return "删除学生";
        case OperationType::Modify:
            return "修改学生";
        case OperationType::BatchDelete:
            return "批量删除";
        case OperationType::BatchModify:
            return "批量修改";
        case OperationType::Import:
            return "导入数据";
        case OperationType::Export:
            return "导出数据";
        default:
            return "未知操作";
    }
}

OperationType OperationLog::stringToType(const QString &typeStr)
{
    if (typeStr == "添加学生") return OperationType::Add;
    if (typeStr == "删除学生") return OperationType::Delete;
    if (typeStr == "修改学生") return OperationType::Modify;
    if (typeStr == "批量删除") return OperationType::BatchDelete;
    if (typeStr == "批量修改") return OperationType::BatchModify;
    if (typeStr == "导入数据") return OperationType::Import;
    if (typeStr == "导出数据") return OperationType::Export;
    return OperationType::Add;
}

// LogManager实现

LogManager::LogManager()
{
}

LogManager& LogManager::getInstance()
{
    static LogManager instance;
    return instance;
}

void LogManager::addLog(OperationType type, const QString &username, const QString &detail)
{
    logs.append(OperationLog(type, username, detail));
}

void LogManager::addLog(const OperationLog &log)
{
    logs.append(log);
}

void LogManager::clearLogs()
{
    logs.clear();
}

QVector<OperationLog> LogManager::getLogsByUsername(const QString &username) const
{
    QVector<OperationLog> result;
    for (const OperationLog &log : logs) {
        if (log.getUsername() == username) {
            result.append(log);
        }
    }
    return result;
}

QVector<OperationLog> LogManager::getLogsByType(OperationType type) const
{
    QVector<OperationLog> result;
    for (const OperationLog &log : logs) {
        if (log.getType() == type) {
            result.append(log);
        }
    }
    return result;
}

QVector<OperationLog> LogManager::getLogsByDateRange(const QDateTime &start, const QDateTime &end) const
{
    QVector<OperationLog> result;
    for (const OperationLog &log : logs) {
        if (log.getTimestamp() >= start && log.getTimestamp() <= end) {
            result.append(log);
        }
    }
    return result;
}

bool LogManager::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件保存日志:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    
    // 写入BOM和表头
    out << "\xEF\xBB\xBF";
    out << "操作类型,操作人,详细信息,时间\n";
    
    // 写入日志数据
    for (const OperationLog &log : logs) {
        out << log.getTypeString() << ","
            << log.getUsername() << ","
            << log.getDetail() << ","
            << log.getTimestamp().toString("yyyy-MM-dd hh:mm:ss") << "\n";
    }
    
    file.close();
    return true;
}

bool LogManager::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件加载日志:" << filename;
        return false;
    }
    
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    
    // 跳过BOM和表头
    QString firstLine = in.readLine();
    
    logs.clear();
    
    // 读取日志数据
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }
        
        QStringList fields = line.split(',');
        if (fields.size() < 4) {
            continue;
        }
        
        QString typeStr = fields[0].trimmed();
        QString username = fields[1].trimmed();
        QString detail = fields[2].trimmed();
        QString timeStr = fields[3].trimmed();
        
        OperationType type = OperationLog::stringToType(typeStr);
        QDateTime timestamp = QDateTime::fromString(timeStr, "yyyy-MM-dd hh:mm:ss");
        
        logs.append(OperationLog(type, username, detail, timestamp));
    }
    
    file.close();
    return true;
}

bool LogManager::exportToCSV(const QString &filename)
{
    return saveToFile(filename);  // 使用相同的格式
}


