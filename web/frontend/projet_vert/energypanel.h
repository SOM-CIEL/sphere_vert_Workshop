#ifndef ENERGYPANEL_H
#define ENERGYPANEL_H

#include <QWidget>

class QProgressBar;
class QLabel;

// Panneau de droite : capacité batterie et consommation sous forme de
// barres, plus la bannière d'alarme. Affichage uniquement, avec deux
// boutons de démo (simuler une décharge, réinitialiser) puisqu'aucun
// capteur d'énergie réel n'est encore branché.
class EnergyPanel : public QWidget
{
    Q_OBJECT

public:
    explicit EnergyPanel(QWidget *parent = nullptr);

public slots:
    void setCapacityPercent(int percent);
    void setConsumptionWatts(int watts);
    void setAlarm(bool active);

signals:
    void dischargeRequested();
    void resetRequested();

private:
    QProgressBar *m_capacityBar;
    QProgressBar *m_consumptionBar;
    QLabel *m_alarmBanner;
};

#endif // ENERGYPANEL_H
