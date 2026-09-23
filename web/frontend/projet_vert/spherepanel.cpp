#include "spherepanel.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QRadialGradient>
#include <QVBoxLayout>

// Petit rond lumineux représentant la sphère ; sa couleur reflète
// l'état d'alarme transmis par MainWindow.
class SpherePanel::GlowCircle : public QWidget
{
public:
    explicit GlowCircle(QWidget *parent = nullptr) : QWidget(parent)
    {
        setFixedSize(120, 120);
    }

    void setAlarm(bool active)
    {
        m_alarm = active;
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        const QPointF center = rect().center();
        const qreal radius = 50;
        const QColor glow = m_alarm ? QColor(255, 70, 70) : QColor(0, 217, 255);

        QRadialGradient gradient(center, radius);
        gradient.setColorAt(0.0, glow.lighter(160));
        gradient.setColorAt(0.6, glow);
        gradient.setColorAt(1.0, QColor(glow.red(), glow.green(), glow.blue(), 0));

        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        painter.drawEllipse(center, radius, radius);
    }

private:
    bool m_alarm = false;
};

SpherePanel::SpherePanel(QWidget *parent)
    : QWidget(parent)
{
    auto *title = new QLabel("Sphère");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #7cd9ff;");
    title->setAlignment(Qt::AlignCenter);

    m_glowCircle = new GlowCircle(this);

    m_temperatureValue = new QLabel("22.5 °C");
    m_humidityValue = new QLabel("45.2 %");
    m_luminosityValue = new QLabel("350 lux");
    m_co2Value = new QLabel("400 ppm");
    for (QLabel *value : {m_temperatureValue, m_humidityValue, m_luminosityValue, m_co2Value})
        value->setStyleSheet("font-size: 16px; font-weight: bold;");

    auto *form = new QFormLayout;
    form->addRow("Température", m_temperatureValue);
    form->addRow("Humidité", m_humidityValue);
    form->addRow("Luminosité", m_luminosityValue);
    form->addRow("CO2", m_co2Value);

    m_doorIcon = new QLabel;
    m_doorIcon->setStyleSheet("font-size: 20px;");
    m_doorStatusLabel = new QLabel;
    m_doorToggleButton = new QPushButton;
    connect(m_doorToggleButton, &QPushButton::clicked, this, [this] {
        m_doorOpen = !m_doorOpen;
        updateDoorLabel();
        emit doorToggled(m_doorOpen);
    });
    updateDoorLabel();

    auto *doorRow = new QHBoxLayout;
    doorRow->addWidget(m_doorIcon);
    doorRow->addWidget(m_doorStatusLabel);
    doorRow->addStretch();

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(m_glowCircle, 0, Qt::AlignCenter);
    layout->addLayout(form);
    layout->addLayout(doorRow);
    layout->addWidget(m_doorToggleButton);
    layout->addStretch();
}

void SpherePanel::setEnvironment(double temperatureC, double humidityPercent, double luminosityLux, double co2Ppm)
{
    m_temperatureValue->setText(QString("%1 °C").arg(temperatureC, 0, 'f', 1));
    m_humidityValue->setText(QString("%1 %").arg(humidityPercent, 0, 'f', 1));
    m_luminosityValue->setText(QString("%1 lux").arg(luminosityLux, 0, 'f', 0));
    m_co2Value->setText(QString("%1 ppm").arg(co2Ppm, 0, 'f', 0));
}

void SpherePanel::setAlarm(bool active)
{
    m_glowCircle->setAlarm(active);
}

void SpherePanel::updateDoorLabel()
{
    if (m_doorOpen) {
        m_doorIcon->setText(QString::fromUtf8("\xF0\x9F\x94\x93")); // 🔓
        m_doorStatusLabel->setText("Porte : OUVERTE");
        m_doorStatusLabel->setStyleSheet("color: #ff5c5c; font-weight: bold;");
        m_doorToggleButton->setText("Fermer la porte");
    } else {
        m_doorIcon->setText(QString::fromUtf8("\xF0\x9F\x94\x92")); // 🔒
        m_doorStatusLabel->setText("Porte : fermée");
        m_doorStatusLabel->setStyleSheet("color: #39ff14; font-weight: bold;");
        m_doorToggleButton->setText("Ouvrir la porte");
    }
}
