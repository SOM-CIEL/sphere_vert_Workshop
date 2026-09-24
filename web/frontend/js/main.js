// Point d'entrée : charge les données (data.js), puis dessine tout
// (ui.js pour les valeurs/textes, charts.js pour les graphiques/jauges),
// et se relance toutes les REFRESH_INTERVAL_MS pour rester à jour sans F5.

import { chargerDonnees } from './data.js';
import { startClock, renderValues, checkAlerts, showError, thresholds } from './ui.js';
import { createGauge, updateGauge, createEnvChart, updateCharts } from './charts.js';

const REFRESH_INTERVAL_MS = 1000;
const CO2_GAUGE_MIN = 1000; // bornes de la jauge, en ppm (plage réellement observée)
const CO2_GAUGE_MAX = 3000;

startClock();

const charts = {
  envChart: createEnvChart(),
};

let co2Gauge = null;

async function refresh() {
  try {
    const data = await chargerDonnees();

    renderValues(data);
    checkAlerts(data);
    updateCharts(charts, data);

    const co2Color = typeof data.environnement.co2 === 'number' && data.environnement.co2 > thresholds.co2Max
      ? '#DC2626'
      : '#16B876';

    if (co2Gauge) {
      updateGauge(co2Gauge, data.environnement.co2, CO2_GAUGE_MAX, co2Color, '', CO2_GAUGE_MIN);
    } else {
      co2Gauge = createGauge('co2Gauge', data.environnement.co2, CO2_GAUGE_MAX, co2Color, 'ppm', '', CO2_GAUGE_MIN);
    }
  } catch (erreur) {
    console.error(erreur);
    showError(`Impossible de charger les données (${erreur.message}). Vérifie que le backend tourne sur le port 8000.`);

    if (!co2Gauge) co2Gauge = createGauge('co2Gauge', null, CO2_GAUGE_MAX, '#16B876', 'ppm', '', CO2_GAUGE_MIN);
  }
}

refresh();
setInterval(refresh, REFRESH_INTERVAL_MS);
