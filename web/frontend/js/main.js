// Point d'entrée : charge les données (data.js), puis dessine tout
// (ui.js pour les valeurs/textes, charts.js pour les graphiques/jauges).

import { chargerDonnees } from './data.js';
import { startClock, renderValues, checkAlerts, showError } from './ui.js';
import {
  createGauge,
  createEnvChart,
  createEnergyChart,
  createGlobalChart,
  updateCharts,
} from './charts.js';

startClock();

const charts = {
  envChart: createEnvChart(),
  energyChart: createEnergyChart(),
  globalChart: createGlobalChart(),
};

async function init() {
  try {
    const data = await chargerDonnees();

    renderValues(data);
    checkAlerts(data);
    updateCharts(charts, data);
    createGauge('o2Gauge', data.securite.o2, 25, '#16B876', 'O₂');
    createGauge('batteryGauge', data.energie.batterie, 100, '#16B876', 'Charge');
  } catch (erreur) {
    console.error(erreur);
    showError(`Impossible de charger les données (${erreur.message}). Vérifie que le backend tourne sur le port 8000.`);
    createGauge('o2Gauge', null, 25, '#16B876', 'O₂');
    createGauge('batteryGauge', null, 100, '#16B876', 'Charge');
  }
}

init();
