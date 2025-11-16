#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include "DesignSystem.h"

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = nullptr)
        : QWidget(parent)
        , m_borderRadius(12)
        , m_shadowEnabled(true)
    {
        // 启用鼠标跟踪以实现悬停效果
        setMouseTracking(true);
        setAttribute(Qt::WA_TranslucentBackground);
        
        // 设置边距以容纳阴影
        setContentsMargins(15, 15, 15, 15);
        
        // 创建布局
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
    }

    void setBorderRadius(int radius)
    {
        m_borderRadius = radius;
        update();
    }

    void setShadowEnabled(bool enabled)
    {
        m_shadowEnabled = enabled;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);
        
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        
        // 获取主题模式
        DesignSystem::ThemeMode themeMode = DesignSystem::instance()->themeMode();
        
        // 计算实际绘制区域（考虑阴影边距）
        QRectF cardRect = rect().adjusted(10, 10, -10, -10);
        
        // 绘制阴影效果
        if (m_shadowEnabled)
        {
            for (int i = 0; i < 5; i++)
            {
                QPainterPath shadowPath;
                QRectF shadowRect = cardRect.adjusted(-i*1.5, -i*1.5, i*1.5, i*1.5);
                shadowPath.addRoundedRect(shadowRect, m_borderRadius + i, m_borderRadius + i);
                
                QColor shadowColor = (themeMode == DesignSystem::Light) 
                    ? QColor(0, 0, 0, 15 - i*3) 
                    : QColor(0, 0, 0, 25 - i*5);
                
                painter.fillPath(shadowPath, shadowColor);
            }
        }
        
        // 绘制卡片背景
        QPainterPath cardPath;
        cardPath.addRoundedRect(cardRect, m_borderRadius, m_borderRadius);
        
        // 根据主题设置卡片颜色
        QColor cardColor;
        if (themeMode == DesignSystem::Light)
        {
            cardColor = QColor(255, 255, 255, 250);
        }
        else
        {
            cardColor = QColor(45, 45, 45, 250);
        }
        
        // 悬停效果
        if (underMouse())
        {
            cardColor = cardColor.lighter(105);
        }
        
        painter.fillPath(cardPath, cardColor);
        
        // 绘制边框
        painter.setPen(QPen(
            (themeMode == DesignSystem::Light) 
                ? QColor(220, 220, 220, 150) 
                : QColor(70, 70, 70, 150),
            1
        ));
        painter.drawPath(cardPath);
    }

private:
    int m_borderRadius;
    bool m_shadowEnabled;
};

#endif // CARDWIDGET_H
