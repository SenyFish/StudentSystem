#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include "ElaTheme.h"

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = nullptr)
        : QWidget(parent)
        , m_borderRadius(12)
        , m_shadowEnabled(true)
    {
        // 设置边距以容纳阴影
        setContentsMargins(15, 15, 15, 15);
        
        // 不设置 WA_TranslucentBackground，避免鼠标事件问题
        setAutoFillBackground(false);
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
        ElaThemeType::ThemeMode themeMode = ElaTheme::getInstance()->getThemeMode();
        
        // 计算实际绘制区域（考虑阴影边距）
        QRectF cardRect = rect().adjusted(10, 10, -10, -10);
        
        // 绘制增强的阴影效果
        if (m_shadowEnabled)
        {
            // 外层阴影 - 更柔和、更大范围
            for (int i = 0; i < 8; i++)
            {
                QPainterPath shadowPath;
                QRectF shadowRect = cardRect.adjusted(-i*2.0, -i*1.5 + i*0.5, i*2.0, i*1.5 + i*0.5);
                shadowPath.addRoundedRect(shadowRect, m_borderRadius + i, m_borderRadius + i);
                
                QColor shadowColor;
                if (themeMode == ElaThemeType::Light) {
                    // 亮色模式：柔和的蓝紫色阴影
                    shadowColor = QColor(102, 126, 234, 25 - i*3);
                } else {
                    // 暗色模式：深色阴影
                    shadowColor = QColor(0, 0, 0, 40 - i*5);
                }
                
                painter.fillPath(shadowPath, shadowColor);
            }
            
            // 内层高光 - 增加立体感
            QPainterPath highlightPath;
            QRectF highlightRect = cardRect.adjusted(1, 1, -1, -1);
            highlightPath.addRoundedRect(highlightRect, m_borderRadius - 1, m_borderRadius - 1);
            QColor highlightColor = (themeMode == ElaThemeType::Light) 
                ? QColor(255, 255, 255, 80) 
                : QColor(255, 255, 255, 15);
            painter.fillPath(highlightPath, highlightColor);
        }
        
        // 绘制卡片背景
        QPainterPath cardPath;
        cardPath.addRoundedRect(cardRect, m_borderRadius, m_borderRadius);
        
        // 根据主题设置卡片颜色
        QColor cardColor;
        if (themeMode == ElaThemeType::Light)
        {
            cardColor = QColor(255, 255, 255, 250);
        }
        else
        {
            cardColor = QColor(45, 45, 45, 250);
        }
        
        painter.fillPath(cardPath, cardColor);
        
        // 绘制边框
        painter.setPen(QPen(
            (themeMode == ElaThemeType::Light) 
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

