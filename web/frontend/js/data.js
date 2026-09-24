// Couche de données : tout ce qui touche à l'API / la base SQLite passe par
// ici. Le reste du site (ui.js, charts.js, main.js) ne connaît jamais l'API
// directement — il reçoit juste l'objet renvoyé par chargerDonnees().
//
// Branché sur le vrai backend (server/backend/app/main.py, ne pas modifier
// ce fichier ici) :
//   GET /environnement -> valeurs actuelles (une seule ligne, mise à jour
//                          en direct par le backend à chaque message MQTT)
//   GET /mesures        -> historique (un instantané toutes les 30s)

const API_BASE_URL = 'http://localhost:8000';

async function fetchEnvironnementActuel() {
  const reponse = await fetch(`${API_BASE_URL}/environnement`);
  if (!reponse.ok) throw new Error(`GET /environnement : ${reponse.status}`);
  return reponse.json(); // { temperature, humidite, luminosite, co2 }
}

async function fetchMesures() {
  const reponse = await fetch(`${API_BASE_URL}/mesures`);
  if (!reponse.ok) throw new Error(`GET /mesures : ${reponse.status}`);
  const lignes = await reponse.json(); // [{ id, timestamp, temperature, humidite, luminosite }, ...]
  return lignes.map((ligne) => ({
    heure: typeof ligne.timestamp === 'string' ? (ligne.timestamp.split(' ')[1] ?? ligne.timestamp) : '',
    temperature: ligne.temperature,
    humidite: ligne.humidite,
    luminosite: ligne.luminosite,
    co2: ligne.co2,
  }));
}

// -----------------------------------------------------------------------
// 🔧 TODO : ces capteurs n'ont pas encore de table/route côté backend.
// Quand ils existeront, remplace le corps de chaque fonction par un vrai
// fetch (même modèle que fetchEnvironnementActuel ci-dessus). En
// attendant, elles renvoient null pour que l'interface affiche "--" plutôt
// que d'inventer une valeur.
// -----------------------------------------------------------------------
async function fetchAgritech() {
  return { humiditeSol: null };
}
async function fetchSecurite() {
  return { porte: null }; // porte: "ouverte" | "fermee" | "verrouillee"
}
async function fetchEnergie() {
  return { batterie: null, solaire: null };
}

// Point d'entrée unique utilisé par main.js.
export async function chargerDonnees() {
  const [actuel, historique, agritech, securite, energie] = await Promise.all([
    fetchEnvironnementActuel(),
    fetchMesures(),
    fetchAgritech(),
    fetchSecurite(),
    fetchEnergie(),
  ]);

  return {
    environnement: {
      temperature: actuel?.temperature ?? null,
      humidite: actuel?.humidite ?? null,
      luminosite: actuel?.luminosite ?? null,
      co2: actuel?.co2 ?? null,
    },
    agritech,
    securite,
    energie,
    systemesEnLigne: true,
    historique,
  };
}
