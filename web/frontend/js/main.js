// Point d'entrée : charge les données (data.js), puis dessine tout
// (ui.js pour les valeurs/textes, charts.js pour les graphiques/jauges),
// et se relance toutes les REFRESH_INTERVAL_MS pour rester à jour sans F5.

import { chargerDonnees } from './data.js';
import { startClock, renderValues, checkAlerts, showError } from './ui.js';
import {
  createGauge,
  updateGauge,
  createEnvChart,
  createEnergyChart,
  createGlobalChart,
  updateCharts,
} from './charts.js';

const REFRESH_INTERVAL_MS = 1000;

startClock();

const charts = {
  envChart: createEnvChart(),
  energyChart: createEnergyChart(),
  globalChart: createGlobalChart(),
};

let o2Gauge = null;
let batteryGauge = null;

async function refresh() {
  try {
    const data = await chargerDonnees();

    renderValues(data);
    checkAlerts(data);
    updateCharts(charts, data);

    if (o2Gauge && batteryGauge) {
      updateGauge(o2Gauge, data.securite.o2, 25, '#16B876');
      updateGauge(batteryGauge, data.energie.batterie, 100, '#16B876');
    } else {
      o2Gauge = createGauge('o2Gauge', data.securite.o2, 25, '#16B876', 'O₂');
      batteryGauge = createGauge('batteryGauge', data.energie.batterie, 100, '#16B876', 'Charge');
    }
  } catch (erreur) {
    console.error(erreur);
    showError(`Impossible de charger les données (${erreur.message}). Vérifie que le backend tourne sur le port 8000.`);

    if (!o2Gauge) o2Gauge = createGauge('o2Gauge', null, 25, '#16B876', 'O₂');
    if (!batteryGauge) batteryGauge = createGauge('batteryGauge', null, 100, '#16B876', 'Charge');
  }
}

refresh();
setInterval(refresh, REFRESH_INTERVAL_MS);
