// Point d'entrée : charge les données (data.js), puis dessine tout
// (ui.js pour les valeurs/textes, charts.js pour les graphiques/jauges),
// et se relance toutes les REFRESH_INTERVAL_MS pour rester à jour sans F5.

import { chargerDonnees } from './data.js';
import { startClock, renderValues, checkAlerts, showError, thresholds } from './ui.js';
import {
  createGauge,
  updateGauge,
  createEnvChart,
  createEnergyChart,
  createGlobalChart,
  updateCharts,
} from './charts.js';

const REFRESH_INTERVAL_MS = 1000;
const CO2_GAUGE_MAX = 5000; // pleine échelle de la jauge, en ppm (borne haute du capteur)

startClock();

const charts = {
  envChart: createEnvChart(),
  energyChart: createEnergyChart(),
  globalChart: createGlobalChart(),
};

let co2Gauge = null;
let batteryGauge = null;

async function refresh() {
  try {
    const data = await chargerDonnees();

    renderValues(data);
    checkAlerts(data);
    updateCharts(charts, data);

    const co2Color = typeof data.environnement.co2 === 'number' && data.environnement.co2 > thresholds.co2Max
      ? '#DC2626'
      : '#16B876';

    if (co2Gauge && batteryGauge) {
      updateGauge(co2Gauge, data.environnement.co2, CO2_GAUGE_MAX, co2Color, ' ppm');
      updateGauge(batteryGauge, data.energie.batterie, 100, '#16B876');
    } else {
      co2Gauge = createGauge('co2Gauge', data.environnement.co2, CO2_GAUGE_MAX, co2Color, 'CO₂', ' ppm');
      batteryGauge = createGauge('batteryGauge', data.energie.batterie, 100, '#16B876', 'Charge');
    }
  } catch (erreur) {
    console.error(erreur);
    showError(`Impossible de charger les données (${erreur.message}). Vérifie que le backend tourne sur le port 8000.`);

    if (!co2Gauge) co2Gauge = createGauge('co2Gauge', null, CO2_GAUGE_MAX, '#16B876', 'CO₂', ' ppm');
    if (!batteryGauge) batteryGauge = createGauge('batteryGauge', null, 100, '#16B876', 'Charge');
  }
}

refresh();
setInterval(refresh, REFRESH_INTERVAL_MS);
