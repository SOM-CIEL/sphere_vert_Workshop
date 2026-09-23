#ifndef SPHEREPANEL_H
#define SPHEREPANEL_H

#include <QWidget>

class QLabel;
class QPushButton;

// Panneau de gauche : indicateur visuel de la sphère, valeurs
// environnementales (température / humidité / luminosité) et statut
// de la porte. Affichage uniquement ; la décision d'alarme reste dans
// MainWindow, ce panneau se contente de la refléter visuellement.
class SpherePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SpherePanel(QWidget *parent = nullptr);

public slots:
    void setEnvironment(double temperatureC, double humidityPercent, double luminosityLux, double co2Ppm);
    void setAlarm(bool active);

signals:
    void doorToggled(bool open);

private:
    void updateDoorLabel();

    class GlowCircle;
    GlowCircle *m_glowCircle;

    QLabel *m_temperatureValue;
    QLabel *m_humidityValue;
    QLabel *m_luminosityValue;
    QLabel *m_co2Value;
    QLabel *m_doorIcon;
    QLabel *m_doorStatusLabel;
    QPushButton *m_doorToggleButton;

    bool m_doorOpen = false;
};

#endif // SPHEREPANEL_H
