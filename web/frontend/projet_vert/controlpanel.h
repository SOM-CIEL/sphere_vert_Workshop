#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>

class QDoubleSpinBox;
class QSpinBox;
class QSlider;
class QProgressBar;
class QPushButton;

// Panneau de gauche : configuration de la sphère + paramètres d'énergie.
// N'exécute aucune logique de simulation lui-même : il expose l'état
// des champs via des accesseurs et des signaux que MainWindow relie
// à la logique et à la visualisation.
class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);

    double radius() const;
    double positionX() const;
    double positionY() const;
    double positionZ() const;
    int luminosity() const;
    int initialCapacityKwh() const;
    double consumptionRateW() const;

    void setCurrentCapacityPercent(int percent);

signals:
    void configurationChanged();
    void startRequested();
    void stopRequested();
    void resetRequested();

private:
    QWidget *buildSphereGroup();
    QWidget *buildEnergyGroup();

    QDoubleSpinBox *m_radiusSpin = nullptr;
    QDoubleSpinBox *m_positionXSpin = nullptr;
    QDoubleSpinBox *m_positionYSpin = nullptr;
    QDoubleSpinBox *m_positionZSpin = nullptr;
    QSlider *m_luminositySlider = nullptr;

    QSpinBox *m_initialCapacitySpin = nullptr;
    QDoubleSpinBox *m_consumptionRateSpin = nullptr;
    QProgressBar *m_currentCapacityBar = nullptr;

    QPushButton *m_startButton = nullptr;
    QPushButton *m_stopButton = nullptr;
    QPushButton *m_resetButton = nullptr;
};

#endif // CONTROLPANEL_H
