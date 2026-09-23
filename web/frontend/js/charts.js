// Tout ce qui touche à Chart.js : le plugin de texte centré pour les
// jauges, la fabrique de jauges, et les 3 graphiques d'historique.

Chart.defaults.font.family = '"Space Grotesk", sans-serif';
Chart.defaults.color = '#4B6C5D';

const centerTextPlugin = {
  id: 'centerText',
  afterDraw(chart) {
    const opts = chart.options.plugins.centerText;
    if (!opts || !opts.text) return;
    const { ctx, chartArea } = chart;
    ctx.save();
    ctx.font = '700 18px "JetBrains Mono", monospace';
    ctx.fillStyle = opts.color || '#0B1F16';
    ctx.textAlign = 'center';
    ctx.textBaseline = 'middle';
    const x = (chartArea.left + chartArea.right) / 2;
    const y = (chartArea.top + chartArea.bottom) / 2;
    ctx.fillText(opts.text, x, y - 4);
    if (opts.subtext) {
      ctx.font = '600 9px "Space Grotesk", sans-serif';
      ctx.fillStyle = '#4B6C5D';
      ctx.fillText(opts.subtext, x, y + 10);
    }
    ctx.restore();
  },
};
Chart.register(centerTextPlugin);

// value peut être null (capteur pas encore branché) : la jauge s'affiche
// alors vide/grise avec "--" au centre, plutôt que de planter ou d'inventer
// un chiffre.
export function createGauge(canvasId, value, max, color, subtext) {
  const hasValue = typeof value === 'number' && !Number.isNaN(value);
  const filled = hasValue ? value : 0;
  const ctx = document.getElementById(canvasId).getContext('2d');
  return new Chart(ctx, {
    type: 'doughnut',
    data: {
      datasets: [{
        data: [filled, Math.max(max - filled, 0)],
        backgroundColor: [hasValue ? color : '#DCEAE2', '#EDF6F0'],
        borderWidth: 0,
      }],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      rotation: -90,
      circumference: 180,
      cutout: '78%',
      animation: { duration: 500 },
      plugins: {
        legend: { display: false },
        tooltip: { enabled: false },
        centerText: {
          text: hasValue ? `${Math.round(value)}%` : '--',
          color: hasValue ? color : '#4B6C5D',
          subtext,
        },
      },
    },
  });
}

const lineOptionsBase = {
  responsive: true,
  maintainAspectRatio: false,
  animation: { duration: 300 },
  interaction: { intersect: false, mode: 'index' },
  elements: { point: { radius: 0 }, line: { tension: 0.35, borderWidth: 2 } },
  plugins: { legend: { display: false } },
};
const tickFont = { family: '"JetBrains Mono"', size: 9 };

export function createEnvChart() {
  return new Chart(document.getElementById('envChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: [],
      datasets: [
        { data: [], borderColor: '#16B876', backgroundColor: 'rgba(22,184,118,0.08)', fill: true, yAxisID: 'yTemp' },
        { data: [], borderColor: '#052E1D', yAxisID: 'yHum' },
      ],
    },
    options: {
      ...lineOptionsBase,
      scales: {
        x: { grid: { display: false }, ticks: { font: tickFont, maxTicksLimit: 5 } },
        yTemp: { position: 'left', grid: { color: '#F1F8F4' }, ticks: { font: tickFont, maxTicksLimit: 4 } },
        yHum: { position: 'right', grid: { display: false }, ticks: { font: tickFont, maxTicksLimit: 4 } },
      },
    },
  });
}

export function createEnergyChart() {
  return new Chart(document.getElementById('energyChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: [],
      datasets: [
        { data: [], borderColor: '#16B876', backgroundColor: 'rgba(22,184,118,0.08)', fill: true, yAxisID: 'yBatt' },
        { data: [], borderColor: '#052E1D', yAxisID: 'ySolar' },
      ],
    },
    options: {
      ...lineOptionsBase,
      scales: {
        x: { grid: { display: false }, ticks: { font: tickFont, maxTicksLimit: 5 } },
        yBatt: { position: 'left', min: 0, max: 100, grid: { color: '#F1F8F4' }, ticks: { font: tickFont, maxTicksLimit: 4 } },
        ySolar: { position: 'right', min: 0, max: 2.5, grid: { display: false }, ticks: { font: tickFont, maxTicksLimit: 4 } },
      },
    },
  });
}

export function createGlobalChart() {
  return new Chart(document.getElementById('globalChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: [],
      datasets: [
        { data: [], borderColor: '#16B876', yAxisID: 'yTemp' },
        { data: [], borderColor: '#052E1D', yAxisID: 'yPercent' },
        { data: [], borderColor: '#22FF88', yAxisID: 'yLux' },
      ],
    },
    options: {
      ...lineOptionsBase,
      scales: {
        x: { grid: { display: false }, ticks: { font: tickFont, maxTicksLimit: 6 } },
        yTemp: { position: 'left', grid: { color: '#F1F8F4' }, ticks: { font: tickFont, maxTicksLimit: 4 } },
        yPercent: { position: 'right', grid: { display: false }, ticks: { font: tickFont, maxTicksLimit: 4 } },
        yLux: { display: false },
      },
    },
  });
}

// data.historique ne contient que temperature/humidite/luminosite pour
// l'instant (seul module réellement en base) : batterie/solaire retombent
// sur la valeur actuelle (constante) tant qu'ils n'ont pas leur propre
// historique.
export function updateCharts({ envChart, energyChart, globalChart }, data) {
  const points = data.historique && data.historique.length > 0
    ? data.historique
    : [{
        heure: '--',
        temperature: data.environnement.temperature,
        humidite: data.environnement.humidite,
        luminosite: data.environnement.luminosite,
      }];

  const labels = points.map((p) => p.heure);

  envChart.data.labels = labels;
  envChart.data.datasets[0].data = points.map((p) => p.temperature);
  envChart.data.datasets[1].data = points.map((p) => p.humidite);
  envChart.update('none');

  energyChart.data.labels = labels;
  energyChart.data.datasets[0].data = points.map(() => data.energie.batterie);
  energyChart.data.datasets[1].data = points.map(() => data.energie.solaire);
  energyChart.update('none');

  globalChart.data.labels = labels;
  globalChart.data.datasets[0].data = points.map((p) => p.temperature);
  globalChart.data.datasets[1].data = points.map((p) => p.humidite);
  globalChart.data.datasets[2].data = points.map((p) => p.luminosite);
  globalChart.update('none');
}
