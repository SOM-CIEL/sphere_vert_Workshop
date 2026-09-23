#include "controlpanel.h"

#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(buildSphereGroup());
    layout->addWidget(buildEnergyGroup());
    layout->addStretch();
}

QWidget *ControlPanel::buildSphereGroup()
{
    auto *group = new QGroupBox("Configuration de la Sphère");

    auto *icon = new QLabel(QString::fromUtf8("\xF0\x9F\x94\xAE")); // icône sphère lumineuse
    icon->setStyleSheet("font-size: 28px;");
    icon->setAlignment(Qt::AlignCenter);

    m_radiusSpin = new QDoubleSpinBox;
    m_radiusSpin->setRange(0.1, 100.0);
    m_radiusSpin->setSingleStep(0.1);
    m_radiusSpin->setValue(12.5);
    m_radiusSpin->setSuffix(" m");

    m_positionXSpin = new QDoubleSpinBox;
    m_positionXSpin->setRange(-1000.0, 1000.0);
    m_positionXSpin->setValue(10.0);

    m_positionYSpin = new QDoubleSpinBox;
    m_positionYSpin->setRange(-1000.0, 1000.0);
    m_positionYSpin->setValue(5.0);

    m_positionZSpin = new QDoubleSpinBox;
    m_positionZSpin->setRange(-1000.0, 1000.0);
    m_positionZSpin->setValue(0.0);

    auto *positionLayout = new QHBoxLayout;
    positionLayout->addWidget(m_positionXSpin);
    positionLayout->addWidget(m_positionYSpin);
    positionLayout->addWidget(m_positionZSpin);

    m_luminositySlider = new QSlider(Qt::Horizontal);
    m_luminositySlider->setRange(0, 100);
    m_luminositySlider->setValue(70);
    m_luminositySlider->setObjectName("luminositySlider");

    auto *form = new QFormLayout;
    form->addRow("Rayon (m)", m_radiusSpin);
    form->addRow("Position X,Y,Z", positionLayout);
    form->addRow("Luminosité", m_luminositySlider);

    auto *groupLayout = new QVBoxLayout(group);
    groupLayout->addWidget(icon);
    groupLayout->addLayout(form);

    connect(m_radiusSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::configurationChanged);
    connect(m_positionXSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::configurationChanged);
    connect(m_positionYSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::configurationChanged);
    connect(m_positionZSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::configurationChanged);
    connect(m_luminositySlider, &QSlider::valueChanged, this, &ControlPanel::configurationChanged);

    return group;
}

QWidget *ControlPanel::buildEnergyGroup()
{
    auto *group = new QGroupBox("Paramètres d'Énergie");

    auto *icon = new QLabel(QString::fromUtf8("\xF0\x9F\x94\x8B")); // icône batterie
    icon->setStyleSheet("font-size: 28px; color: #39ff14;");
    icon->setAlignment(Qt::AlignCenter);

    m_initialCapacitySpin = new QSpinBox;
    m_initialCapacitySpin->setRange(1, 1000);
    m_initialCapacitySpin->setValue(8);
    m_initialCapacitySpin->setSuffix(" kWh");

    m_consumptionRateSpin = new QDoubleSpinBox;
    m_consumptionRateSpin->setRange(0.0, 10000.0);
    m_consumptionRateSpin->setValue(250.0);
    m_consumptionRateSpin->setSuffix(" W");

    m_currentCapacityBar = new QProgressBar;
    m_currentCapacityBar->setObjectName("currentCapacityBar");
    m_currentCapacityBar->setRange(0, 100);
    m_currentCapacityBar->setValue(85);
    m_currentCapacityBar->setFormat("%p%");

    auto *form = new QFormLayout;
    form->addRow("Capacité Initiale (kWh)", m_initialCapacitySpin);
    form->addRow("Taux de Consommation (W)", m_consumptionRateSpin);
    form->addRow("Capacité Actuelle", m_currentCapacityBar);

    m_startButton = new QPushButton("Démarrer Simulation");
    m_startButton->setObjectName("startButton");

    m_stopButton = new QPushButton("Arrêter");
    m_stopButton->setObjectName("stopButton");

    m_resetButton = new QPushButton("Réinitialiser");
    m_resetButton->setObjectName("resetButton");

    auto *buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(m_startButton);
    buttonsLayout->addWidget(m_stopButton);
    buttonsLayout->addWidget(m_resetButton);

    auto *groupLayout = new QVBoxLayout(group);
    groupLayout->addWidget(icon);
    groupLayout->addLayout(form);
    groupLayout->addLayout(buttonsLayout);

    connect(m_initialCapacitySpin, &QSpinBox::valueChanged, this, &ControlPanel::configurationChanged);
    connect(m_consumptionRateSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::configurationChanged);
    connect(m_startButton, &QPushButton::clicked, this, &ControlPanel::startRequested);
    connect(m_stopButton, &QPushButton::clicked, this, &ControlPanel::stopRequested);
    connect(m_resetButton, &QPushButton::clicked, this, &ControlPanel::resetRequested);

    return group;
}

double ControlPanel::radius() const { return m_radiusSpin->value(); }
double ControlPanel::positionX() const { return m_positionXSpin->value(); }
double ControlPanel::positionY() const { return m_positionYSpin->value(); }
double ControlPanel::positionZ() const { return m_positionZSpin->value(); }
int ControlPanel::luminosity() const { return m_luminositySlider->value(); }
int ControlPanel::initialCapacityKwh() const { return m_initialCapacitySpin->value(); }
double ControlPanel::consumptionRateW() const { return m_consumptionRateSpin->value(); }

void ControlPanel::setCurrentCapacityPercent(int percent)
{
    m_currentCapacityBar->setValue(percent);
}
