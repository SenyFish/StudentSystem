#ifndef OPERATIONLOG_H
#define OPERATIONLOG_H

#include <QString>
#include <QDateTime>
#include <QVector>

enum class OperationType {
    Add,           // 添加学生
    Delete,        // 删除学生
    Modify,        // 修改学生
    BatchDelete,   // 批量删除
    BatchModify,   // 批量修改
    Import,        // 导入数据
    Export         // 导出数据
};

class OperationLog
{
public:
    OperationLog();
    OperationLog(OperationType type, const QString &username, 
                 const QString &detail, const QDateTime &timestamp = QDateTime::currentDateTime());
    
    // Getter方法
    OperationType getType() const { return type; }
    QString getUsername() const { return username; }
    QString getDetail() const { return detail; }
    QDateTime getTimestamp() const { return timestamp; }
    
    // 工具方法
    QString getTypeString() const;
    static QString typeToString(OperationType type);
    static OperationType stringToType(const QString &typeStr);
    
private:
    OperationType type;
    QString username;
    QString detail;
    QDateTime timestamp;
};

class LogManager
{
public:
    static LogManager& getInstance();
    
    // 日志管理
    void addLog(OperationType type, const QString &username, const QString &detail);
    void addLog(const OperationLog &log);
    void clearLogs();
    
    // 查询
    QVector<OperationLog> getAllLogs() const { return logs; }
    QVector<OperationLog> getLogsByUsername(const QString &username) const;
    QVector<OperationLog> getLogsByType(OperationType type) const;
    QVector<OperationLog> getLogsByDateRange(const QDateTime &start, const QDateTime &end) const;
    
    // 数据持久化
    bool saveToFile(const QString &filename);
    bool loadFromFile(const QString &filename);
    bool exportToCSV(const QString &filename);
    
private:
    LogManager();
    ~LogManager() = default;
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;
    
    QVector<OperationLog> logs;
};

#endif // OPERATIONLOG_H


