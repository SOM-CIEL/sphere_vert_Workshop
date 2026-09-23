#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SpherePanel;
class EnergyPanel;

// Fenêtre principale : assemble SpherePanel (gauche) et EnergyPanel
// (droite), et décide seule quand déclencher l'alarme (porte ouverte
// ou capacité critique) — les deux panneaux se contentent de l'afficher.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onDoorToggled(bool open);
    void onDischargeRequested();
    void onResetRequested();

private:
    void applyDarkTheme();
    void updateAlarmState();

    SpherePanel *m_spherePanel = nullptr;
    EnergyPanel *m_energyPanel = nullptr;

    bool m_doorOpen = false;
    int m_capacityPercent = 85;
    double m_co2Ppm = 400.0;

    static constexpr int kCriticalCapacityPercent = 20;
    static constexpr double kCriticalCo2Ppm = 1000.0;
};

#endif // MAINWINDOW_H
