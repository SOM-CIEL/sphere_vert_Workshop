// Tout ce qui touche au DOM "statique" (hors graphiques) : horloge,
// valeurs des capteurs, statut de la porte, pastille système, alertes.

export const thresholds = { co2Max: 3000, batteryMin: 15, soilMin: 20 };

const doorLabels = {
  ouverte: { text: '🔓 Ouverte', class: 'inline-flex items-center gap-1.5 px-2.5 py-1 rounded-full bg-red-600 text-white font-mono text-xs font-semibold' },
  fermee: { text: '🔓 Fermée', class: 'inline-flex items-center gap-1.5 px-2.5 py-1 rounded-full bg-emerald-700 text-white font-mono text-xs font-semibold' },
  verrouillee: { text: '🔒 Verrouillée', class: 'inline-flex items-center gap-1.5 px-2.5 py-1 rounded-full bg-emerald-950 text-neon font-mono text-xs font-semibold' },
};
const doorInconnue = { text: '— Inconnue', class: 'inline-flex items-center gap-1.5 px-2.5 py-1 rounded-full bg-paperAlt text-inkSoft font-mono text-xs font-semibold' };

export function pad(n) {
  return n.toString().padStart(2, '0');
}

export function fmt(value, decimals) {
  return typeof value === 'number' && !Number.isNaN(value) ? value.toFixed(decimals) : '--';
}

export function startClock() {
  function update() {
    const now = new Date();
    document.getElementById('clock').textContent =
      `${pad(now.getHours())}:${pad(now.getMinutes())}:${pad(now.getSeconds())}`;
    document.getElementById('dateLabel').textContent =
      now.toLocaleDateString('fr-FR', { weekday: 'long', day: 'numeric', month: 'long', year: 'numeric' });
  }
  update();
  setInterval(update, 1000);
}

export function renderValues(data) {
  document.getElementById('valTemp').innerHTML = `${fmt(data.environnement.temperature, 1)}<span class="text-sm text-inkSoft ml-1">°C</span>`;
  document.getElementById('valHum').innerHTML = `${fmt(data.environnement.humidite, 1)}<span class="text-sm text-inkSoft ml-1">%</span>`;
  document.getElementById('valLux').innerHTML = `${fmt(data.environnement.luminosite, 0)}<span class="text-sm text-inkSoft ml-1">lux</span>`;
  document.getElementById('valSoil').innerHTML = `${fmt(data.agritech.humiditeSol, 1)}<span class="text-sm text-inkSoft ml-1">%</span>`;
  document.getElementById('soilBar').style.width = `${data.agritech.humiditeSol ?? 0}%`;
  document.getElementById('valSolar').innerHTML = `${fmt(data.energie.solaire, 2)}<span class="text-sm text-inkSoft ml-1">kW</span>`;

  const door = doorLabels[data.securite.porte] ?? doorInconnue;
  const doorEl = document.getElementById('doorState');
  doorEl.textContent = door.text;
  doorEl.className = door.class;

  const statusDot = document.getElementById('statusDot');
  const statusText = document.getElementById('statusText');
  if (data.systemesEnLigne) {
    statusDot.className = 'status-dot h-2 w-2 rounded-full bg-neon';
    statusText.className = 'text-xs font-semibold text-emerald-700';
    statusText.textContent = 'Tous les systèmes en ligne';
  } else {
    statusDot.className = 'h-2 w-2 rounded-full bg-red-500';
    statusText.className = 'text-xs font-semibold text-red-700';
    statusText.textContent = 'Système(s) hors ligne';
  }
}

export function checkAlerts(data) {
  const alertBar = document.getElementById('alertBar');
  const alertText = document.getElementById('alertText');
  const dot = alertBar.querySelector('span');
  const messages = [];
  if (typeof data.environnement.co2 === 'number' && data.environnement.co2 > thresholds.co2Max) {
    messages.push(`CO₂ élevé (${fmt(data.environnement.co2, 0)} ppm)`);
  }
  if (typeof data.energie.batterie === 'number' && data.energie.batterie < thresholds.batteryMin) {
    messages.push(`Batterie critique (${fmt(data.energie.batterie, 0)}%)`);
  }
  if (typeof data.agritech.humiditeSol === 'number' && data.agritech.humiditeSol < thresholds.soilMin) {
    messages.push(`Humidité du sol insuffisante (${fmt(data.agritech.humiditeSol, 1)}%)`);
  }

  if (messages.length > 0) {
    alertBar.className = 'border-b border-red-200 bg-red-50';
    dot.className = 'h-2 w-2 rounded-full bg-red-500';
    alertText.className = 'text-xs font-medium text-red-800';
    alertText.textContent = `⚠ ${messages.join(' — ')}`;
  } else {
    alertBar.className = 'border-b border-border bg-emerald-50';
    dot.className = 'h-2 w-2 rounded-full bg-emerald-500';
    alertText.className = 'text-xs font-medium text-emerald-800';
    alertText.textContent = 'Aucune alerte active — tous les paramètres sont dans les plages nominales.';
  }
}

export function showError(message) {
  const alertBar = document.getElementById('alertBar');
  const alertText = document.getElementById('alertText');
  const dot = alertBar.querySelector('span');
  alertBar.className = 'border-b border-red-200 bg-red-50';
  dot.className = 'h-2 w-2 rounded-full bg-red-500';
  alertText.className = 'text-xs font-medium text-red-800';
  alertText.textContent = `⚠ ${message}`;
}
