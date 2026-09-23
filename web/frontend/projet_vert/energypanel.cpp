#include "energypanel.h"

#include <QFormLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

EnergyPanel::EnergyPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *title = new QLabel("Énergie");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #7cd9ff;");
    title->setAlignment(Qt::AlignCenter);

    m_capacityBar = new QProgressBar;
    m_capacityBar->setObjectName("capacityBar");
    m_capacityBar->setRange(0, 100);
    m_capacityBar->setValue(85);
    m_capacityBar->setFormat("%p%");

    m_consumptionBar = new QProgressBar;
    m_consumptionBar->setObjectName("consumptionBar");
    m_consumptionBar->setRange(0, 1000);
    m_consumptionBar->setValue(250);
    m_consumptionBar->setFormat("%v W");

    auto *form = new QFormLayout;
    form->addRow("Capacité batterie", m_capacityBar);
    form->addRow("Consommation", m_consumptionBar);

    m_alarmBanner = new QLabel("Tout est normal");
    m_alarmBanner->setAlignment(Qt::AlignCenter);
    m_alarmBanner->setStyleSheet("font-weight: bold; color: #39ff14; padding: 8px;");

    auto *dischargeButton = new QPushButton("Simuler décharge");
    auto *resetButton = new QPushButton("Réinitialiser");
    resetButton->setObjectName("resetButton");

    connect(dischargeButton, &QPushButton::clicked, this, &EnergyPanel::dischargeRequested);
    connect(resetButton, &QPushButton::clicked, this, &EnergyPanel::resetRequested);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addLayout(form);
    layout->addWidget(m_alarmBanner);
    layout->addWidget(dischargeButton);
    layout->addWidget(resetButton);
    layout->addStretch();
}

void EnergyPanel::setCapacityPercent(int percent)
{
    m_capacityBar->setValue(percent);
}

void EnergyPanel::setConsumptionWatts(int watts)
{
    m_consumptionBar->setValue(watts);
}

void EnergyPanel::setAlarm(bool active)
{
    if (active) {
        m_alarmBanner->setText(QString::fromUtf8("\xE2\x9A\xA0 ALARME \xE2\x80\x94 valeur critique"));
        m_alarmBanner->setStyleSheet(
            "font-weight: bold; color: #ff5c5c; background-color: #3a1414; padding: 8px; border-radius: 6px;");
    } else {
        m_alarmBanner->setText("Tout est normal");
        m_alarmBanner->setStyleSheet("font-weight: bold; color: #39ff14; padding: 8px;");
    }
}
