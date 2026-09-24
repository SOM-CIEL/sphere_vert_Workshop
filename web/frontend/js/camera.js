// Bloc caméra : affiche le flux dans un <iframe> plutôt qu'une balise
// <img>, car l'URL (port 8889, probablement MediaMTX/WebRTC) sert une
// page HTML autonome avec son propre lecteur, pas une image brute.
//
// Limite connue : une iframe cross-origin ne laisse pas lire son contenu
// depuis ce script, donc on ne peut pas détecter une erreur interne à
// cette page (ex: "aucun flux" côté serveur vidéo) — seulement un échec
// réseau ou une absence de réponse (timeout). D'où un message générique
// plutôt qu'un diagnostic précis en cas de souci.

const STREAM_URL = 'http://10.42.0.1:8889/cam/';
const LOAD_TIMEOUT_MS = 8000;
const RETRY_DELAY_MS = 10000;

export function initCamera() {
  const iframe = document.getElementById('cameraStream');
  const overlay = document.getElementById('cameraOverlay');
  const liveDot = document.getElementById('cameraLiveDot');
  const liveText = document.getElementById('cameraLiveText');

  let loadTimeout;

  function setState(state) {
    if (state === 'live') {
      overlay.classList.add('hidden');
      liveDot.className = 'h-2 w-2 rounded-full bg-red-500 animate-pulse';
      liveText.className = 'text-[10px] font-mono font-semibold text-red-600 uppercase tracking-wide';
      liveText.textContent = 'Live';
    } else if (state === 'connecting') {
      overlay.classList.remove('hidden');
      overlay.textContent = 'Connexion au flux…';
      liveDot.className = 'h-2 w-2 rounded-full bg-paperAlt';
      liveText.className = 'text-[10px] font-mono font-semibold text-inkSoft uppercase tracking-wide';
      liveText.textContent = 'Connexion…';
    } else {
      overlay.classList.remove('hidden');
      overlay.textContent = 'Flux indisponible — nouvelle tentative...';
      liveDot.className = 'h-2 w-2 rounded-full bg-paperAlt';
      liveText.className = 'text-[10px] font-mono font-semibold text-inkSoft uppercase tracking-wide';
      liveText.textContent = 'Hors ligne';
    }
  }

  function connect() {
    setState('connecting');
    clearTimeout(loadTimeout);
    const cacheBust = `${STREAM_URL.includes('?') ? '&' : '?'}_=${Date.now()}`;
    iframe.src = `${STREAM_URL}${cacheBust}`;
    loadTimeout = setTimeout(() => {
      setState('offline');
      setTimeout(connect, RETRY_DELAY_MS);
    }, LOAD_TIMEOUT_MS);
  }

  iframe.addEventListener('load', () => {
    clearTimeout(loadTimeout);
    setState('live');
  });
  iframe.addEventListener('error', () => {
    clearTimeout(loadTimeout);
    setState('offline');
    setTimeout(connect, RETRY_DELAY_MS);
  });

  connect();
}
