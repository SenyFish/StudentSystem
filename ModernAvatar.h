#ifndef MODERNAVATAR_H
#define MODERNAVATAR_H

#include <QWidget>
#include <QTimer>
#include <QColor>
#include <QString>

class ModernAvatar : public QWidget
{
    Q_OBJECT
public:
    explicit ModernAvatar(QWidget *parent = nullptr);
    void setGender(const QString &gender);

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    QString m_gender;
    qreal m_angle;
    qreal m_scale;
    bool m_scaleExpanding;
    QTimer *m_timer;
};

#endif // MODERNAVATAR_H

