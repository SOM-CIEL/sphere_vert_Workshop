#include "mainwindow.h"
#include "energypanel.h"
#include "spherepanel.h"

#include <QHBoxLayout>
#include <QWidget>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Horizon 2080 — Sphère Énergétique");
    resize(640, 420);

    m_spherePanel = new SpherePanel(this);
    m_energyPanel = new EnergyPanel(this);

    auto *central = new QWidget(this);
    auto *layout = new QHBoxLayout(central);
    layout->addWidget(m_spherePanel);
    layout->addWidget(m_energyPanel);
    setCentralWidget(central);

    connect(m_spherePanel, &SpherePanel::doorToggled, this, &MainWindow::onDoorToggled);
    connect(m_energyPanel, &EnergyPanel::dischargeRequested, this, &MainWindow::onDischargeRequested);
    connect(m_energyPanel, &EnergyPanel::resetRequested, this, &MainWindow::onResetRequested);

    m_spherePanel->setEnvironment(22.5, 45.2, 350.0, m_co2Ppm);
    m_energyPanel->setCapacityPercent(m_capacityPercent);
    m_energyPanel->setConsumptionWatts(250);

    applyDarkTheme();
    updateAlarmState();
}

void MainWindow::onDoorToggled(bool open)
{
    m_doorOpen = open;
    updateAlarmState();
}

void MainWindow::onDischargeRequested()
{
    m_capacityPercent = std::max(0, m_capacityPercent - 15);
    m_energyPanel->setCapacityPercent(m_capacityPercent);
    updateAlarmState();
}

void MainWindow::onResetRequested()
{
    m_capacityPercent = 100;
    m_energyPanel->setCapacityPercent(m_capacityPercent);
    m_energyPanel->setConsumptionWatts(250);
    updateAlarmState();
}

void MainWindow::updateAlarmState()
{
    const bool alarm = m_doorOpen || m_capacityPercent < kCriticalCapacityPercent || m_co2Ppm > kCriticalCo2Ppm;
    m_spherePanel->setAlarm(alarm);
    m_energyPanel->setAlarm(alarm);
}

void MainWindow::applyDarkTheme()
{
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #14161c;
            color: #d8e6ee;
        }
        QProgressBar {
            border: 1px solid #2b3542;
            border-radius: 4px;
            text-align: center;
            background-color: #1c212b;
            min-height: 18px;
        }
        QProgressBar#capacityBar::chunk {
            border-radius: 3px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #39ff14, stop:1 #00d9ff);
        }
        QProgressBar#consumptionBar::chunk {
            border-radius: 3px;
            background: #00d9ff;
        }
        QPushButton {
            background-color: #1c212b;
            border: 1px solid #2b3542;
            border-radius: 6px;
            padding: 6px 10px;
            color: #d8e6ee;
        }
        QPushButton:hover {
            background-color: #232a36;
        }
        QPushButton#resetButton {
            border: 1px solid #00d9ff;
            color: #00d9ff;
        }
    )");
}
