#include "spherevisualizationwidget.h"

#include <QPainter>
#include <QRadialGradient>

SphereVisualizationWidget::SphereVisualizationWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(400, 400);
}

void SphereVisualizationWidget::setRadius(double radiusMeters)
{
    m_radius = radiusMeters;
    update();
}

void SphereVisualizationWidget::setCapacity(double capacityKwh)
{
    m_capacity = capacityKwh;
    update();
}

void SphereVisualizationWidget::setTemperature(double temperatureKelvin)
{
    m_temperature = temperatureKelvin;
    update();
}

void SphereVisualizationWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawModuleOutline(painter);
    drawSphere(painter);
    drawOverlays(painter);
}

void SphereVisualizationWidget::drawBackground(QPainter &painter) const
{
    painter.fillRect(rect(), QColor("#0a0e14"));
}

void SphereVisualizationWidget::drawModuleOutline(QPainter &painter) const
{
    // Contour discret d'un module de vaisseau, purement décoratif.
    painter.setPen(QPen(QColor(80, 160, 200, 80), 1, Qt::DashLine));
    painter.setBrush(Qt::NoBrush);

    const QRectF moduleRect = rect().adjusted(20, 20, -20, -20);
    painter.drawRoundedRect(moduleRect, 24, 24);

    painter.setPen(QColor(120, 180, 210, 140));
    painter.drawText(moduleRect.adjusted(12, 8, -12, -8), Qt::AlignTop | Qt::AlignLeft, "Test module");
}

void SphereVisualizationWidget::drawSphere(QPainter &painter) const
{
    const QPointF center = rect().center();
    const qreal displayRadius = qMin(width(), height()) * 0.28;

    QRadialGradient gradient(center, displayRadius);
    gradient.setColorAt(0.0, QColor(140, 255, 230, 220));
    gradient.setColorAt(0.5, QColor(0, 200, 255, 140));
    gradient.setColorAt(0.85, QColor(30, 255, 140, 60));
    gradient.setColorAt(1.0, QColor(30, 255, 140, 0));

    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient);
    painter.drawEllipse(center, displayRadius, displayRadius);

    painter.setPen(QPen(QColor(160, 255, 240, 200), 1.5));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(center, displayRadius * 0.98, displayRadius * 0.98);

    QFont labelFont = painter.font();
    labelFont.setPointSize(11);
    painter.setFont(labelFont);
    painter.setPen(QColor("#dff9ff"));
    painter.drawText(QRectF(center.x() - 60, center.y() + displayRadius + 8, 120, 20),
                      Qt::AlignCenter, "Sphère");
}

void SphereVisualizationWidget::drawOverlays(QPainter &painter) const
{
    QFont overlayFont = painter.font();
    overlayFont.setPointSize(10);
    overlayFont.setBold(true);
    painter.setFont(overlayFont);
    painter.setPen(QColor("#7CFFCB"));

    const QString radiusText = QString("R: %1m").arg(m_radius, 0, 'f', 1);
    const QString capacityText = QString("C: %1 kWh").arg(m_capacity, 0, 'f', 1);
    const QString temperatureText = QString("T: %1 K").arg(m_temperature, 0, 'f', 0);

    painter.drawText(QRectF(30, 34, 200, 24), Qt::AlignLeft, radiusText);
    painter.drawText(QRectF(30, 58, 200, 24), Qt::AlignLeft, capacityText);
    painter.drawText(QRectF(30, 82, 200, 24), Qt::AlignLeft, temperatureText);
}
