#ifndef SPHEREVISUALIZATIONWIDGET_H
#define SPHEREVISUALIZATIONWIDGET_H

#include <QWidget>

class QPainter;

// Zone de visualisation : sphère énergétique lumineuse et palpitante,
// avec overlays de texte pour les données clés de la simulation.
// Ne connaît rien de la logique de simulation : MainWindow la pilote
// uniquement via les slots publics ci-dessous.
class SphereVisualizationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SphereVisualizationWidget(QWidget *parent = nullptr);

public slots:
    void setRadius(double radiusMeters);
    void setCapacity(double capacityKwh);
    void setTemperature(double temperatureKelvin);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter) const;
    void drawModuleOutline(QPainter &painter) const;
    void drawSphere(QPainter &painter) const;
    void drawOverlays(QPainter &painter) const;

    double m_radius = 12.5;
    double m_capacity = 85.2;
    double m_temperature = 450.0;
};

#endif // SPHEREVISUALIZATIONWIDGET_H
