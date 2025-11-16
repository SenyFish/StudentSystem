#ifndef FLATCARDWIDGET_H
#define FLATCARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include "ElaTheme.h"

class FlatCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FlatCardWidget(QWidget *parent = nullptr)
        : QWidget(parent)
        , m_shadowEnabled(true)
        , m_elevation(2)
    {
        setAutoFillBackground(false);
        
        // 监听主题变化，自动重绘
        connect(ElaTheme::getInstance(), &ElaTheme::themeModeChanged, this, [this]() {
            update();
        });
    }

    void setElevation(int elevation)
    {
        m_elevation = elevation;
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
        
        // 实际绘制区域
        QRect cardRect = rect();
        
        // 绘制扁平化阴影
        if (m_shadowEnabled && m_elevation > 0)
        {
            // 底部阴影条
            QLinearGradient shadowGradient(0, cardRect.bottom() - 2, 0, cardRect.bottom() + m_elevation * 3);
            
            if (themeMode == ElaThemeType::Light) {
                shadowGradient.setColorAt(0, QColor(0, 0, 0, 15 * m_elevation));
                shadowGradient.setColorAt(1, QColor(0, 0, 0, 0));
            } else {
                shadowGradient.setColorAt(0, QColor(0, 0, 0, 30 * m_elevation));
                shadowGradient.setColorAt(1, QColor(0, 0, 0, 0));
            }
            
            QRect shadowRect = cardRect.adjusted(0, cardRect.height(), 0, m_elevation * 3);
            painter.fillRect(shadowRect, shadowGradient);
        }
        
        // 绘制卡片背景 - 无圆角
        QRect mainRect = cardRect;
        
        // 根据主题设置卡片颜色
        QColor cardColor;
        if (themeMode == ElaThemeType::Light)
        {
            cardColor = QColor(255, 255, 255, 255);
        }
        else
        {
            cardColor = QColor(30, 30, 30, 255);  // 暗色模式下更深的背景
        }
        
        painter.fillRect(mainRect, cardColor);
        
        // 绘制顶部装饰条
        if (m_elevation >= 1) {
            QLinearGradient topGradient(0, 0, mainRect.width(), 0);
            if (themeMode == ElaThemeType::Light) {
                topGradient.setColorAt(0, QColor(102, 126, 234, 180));
                topGradient.setColorAt(1, QColor(118, 75, 162, 180));
            } else {
                // 暗色模式下使用更亮的渐变
                topGradient.setColorAt(0, QColor(102, 126, 234, 220));
                topGradient.setColorAt(1, QColor(118, 75, 162, 220));
            }
            
            QRect topBar(0, 0, mainRect.width(), 3);
            painter.fillRect(topBar, topGradient);
        }
        
        // 绘制边框
        painter.setPen(QPen(
            (themeMode == ElaThemeType::Light) 
                ? QColor(230, 230, 230, 255) 
                : QColor(50, 50, 50, 255),  // 暗色模式下的边框
            1
        ));
        painter.drawRect(mainRect.adjusted(0, 0, -1, -1));
    }

private:
    bool m_shadowEnabled;
    int m_elevation;  // 0-3, 控制阴影深度
};

#endif // FLATCARDWIDGET_H

