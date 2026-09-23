// Couche de données : tout ce qui touche à l'API / la base SQLite passe par
// ici. Le reste du site (ui.js, charts.js, main.js) ne connaît jamais l'API
// directement — il reçoit juste l'objet renvoyé par chargerDonnees().

const API_BASE_URL = 'http://127.0.0.1:8000';

// Lit la table `environnement` de horizon_2080.db via l'API FastAPI
// (server/database/main.py -> GET /api/environnement). Chaque ligne de la
// table devient un point d'historique.
async function fetchEnvironnementHistorique() {
  const reponse = await fetch(`${API_BASE_URL}/api/environnement`);
  if (!reponse.ok) throw new Error(`API environnement indisponible (${reponse.status})`);
  const lignes = await reponse.json();
  return lignes.map((ligne) => ({
    heure: typeof ligne.timestamp === 'string' ? (ligne.timestamp.split(' ')[1] ?? ligne.timestamp) : '',
    temperature: ligne.temperature,
    humidite: ligne.humidite,
    luminosite: ligne.luminosite,
  }));
}

// -----------------------------------------------------------------------
// 🔧 TODO : ces trois capteurs n'ont pas encore de table/colonne dans
// horizon_2080.db. Quand ils existeront, remplace le corps de chaque
// fonction par un vrai fetch (même modèle que fetchEnvironnementHistorique
// ci-dessus). En attendant, elles renvoient null pour que l'interface
// affiche "--" plutôt que d'inventer une valeur.
// -----------------------------------------------------------------------
async function fetchAgritech() {
  return { humiditeSol: null };
}
async function fetchSecurite() {
  return { o2: null, porte: null }; // porte: "ouverte" | "fermee" | "verrouillee"
}
async function fetchEnergie() {
  return { batterie: null, solaire: null };
}

// Point d'entrée unique utilisé par main.js.
export async function chargerDonnees() {
  const historique = await fetchEnvironnementHistorique();
  const dernier = historique[historique.length - 1] ?? {};
  const [agritech, securite, energie] = await Promise.all([
    fetchAgritech(),
    fetchSecurite(),
    fetchEnergie(),
  ]);

  return {
    environnement: {
      temperature: dernier.temperature ?? null,
      humidite: dernier.humidite ?? null,
      luminosite: dernier.luminosite ?? null,
    },
    agritech,
    securite,
    energie,
    systemesEnLigne: true,
    historique,
  };
}
