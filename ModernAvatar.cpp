#include "ModernAvatar.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QtMath>
#include <QShowEvent>
#include <QHideEvent>

ModernAvatar::ModernAvatar(QWidget *parent)
    : QWidget(parent), m_gender("男"), m_angle(0), m_scale(1.0), m_scaleExpanding(true)
{
    setFixedSize(150, 150);
    // Set transparent background
    setAttribute(Qt::WA_TranslucentBackground);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        // Rotation animation
        m_angle += 1.0;
        if (m_angle >= 360.0) m_angle -= 360.0;

        // Breathing animation
        if (m_scaleExpanding) {
            m_scale += 0.001;
            if (m_scale >= 1.03) m_scaleExpanding = false;
        } else {
            m_scale -= 0.001;
            if (m_scale <= 0.97) m_scaleExpanding = true;
        }
        update();
    });
    // Timer starts in showEvent
}

void ModernAvatar::setGender(const QString &gender)
{
    if (m_gender != gender) {
        m_gender = gender;
        update();
    }
}

void ModernAvatar::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (m_timer) m_timer->start(16);
}

void ModernAvatar::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    if (m_timer) m_timer->stop();
}

void ModernAvatar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int size = qMin(width(), height());
    int center = size / 2;
    int padding = qRound(size * 0.1); // Proportional padding (e.g., 15px for 150px, 8px for 80px)
    int radius = (size / 2) - padding; // Leave space for glow/border and scaling

    painter.translate(center, center);
    painter.scale(m_scale, m_scale);

    // Colors based on gender
    QColor color1, color2;
    if (m_gender == "男") {
        color1 = QColor(102, 126, 234);
        color2 = QColor(118, 75, 162);
    } else {
        color1 = QColor(250, 112, 154);
        color2 = QColor(254, 225, 64);
    }

    // Define Squircle Shape (Rounded Rect with large radius)
    // A true squircle is |x|^4 + |y|^4 = r^4, but rounded rect is easier and looks similar enough
    QPainterPath path;
    double rectSize = radius * 2;
    // Using 45% of side as radius gives a nice squircle-like shape
    double cornerRadius = rectSize * 0.22; 
    path.addRoundedRect(-radius, -radius, rectSize, rectSize, cornerRadius, cornerRadius);

    // 1. Draw Glow (Outer Shadow)
    // Draw a few layers of low opacity to simulate glow
    painter.save();
    QColor glowColor = color1;
    glowColor.setAlpha(20);
    for(int i=0; i<3; ++i) {
        double s = 1.0 + (i * 0.03);
        painter.save();
        painter.scale(s, s);
        painter.setPen(Qt::NoPen);
        painter.setBrush(glowColor);
        painter.drawPath(path);
        painter.restore();
    }
    painter.restore();

    // 2. Draw Rotating Gradient Border
    painter.save();
    painter.rotate(m_angle);
    
    QLinearGradient borderGradient(-radius, -radius, radius, radius);
    borderGradient.setColorAt(0, color1);
    borderGradient.setColorAt(0.5, color2);
    borderGradient.setColorAt(1, color1);

    QPen borderPen(borderGradient, 4);
    borderPen.setCapStyle(Qt::RoundCap);
    painter.setPen(borderPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(path);
    painter.restore();

    // 3. Draw Inner Fill
    painter.save();
    // Make the fill slightly smaller to show the border clearly
    double borderGap = qMax(3.0, size * 0.04); // Proportional gap
    double innerRadius = radius - borderGap;
    double innerRectSize = innerRadius * 2;
    double innerCornerRadius = innerRectSize * 0.22;
    QPainterPath innerPath;
    innerPath.addRoundedRect(-innerRadius, -innerRadius, innerRectSize, innerRectSize, innerCornerRadius, innerCornerRadius);

    QLinearGradient fillGradient(-innerRadius, -innerRadius, innerRadius, innerRadius);
    fillGradient.setColorAt(0, color1.lighter(110));
    fillGradient.setColorAt(1, color2.lighter(110));

    painter.setPen(Qt::NoPen);
    painter.setBrush(fillGradient);
    painter.drawPath(innerPath);
    
    // Optional: Add a shine or reflection
    QLinearGradient shineGradient(-innerRadius, -innerRadius, 0, 0);
    shineGradient.setColorAt(0, QColor(255, 255, 255, 100));
    shineGradient.setColorAt(1, QColor(255, 255, 255, 0));
    painter.setBrush(shineGradient);
    painter.drawPath(innerPath);

    painter.restore();
}

