# Dashboard Qt — Environnement

Widget de bureau (PySide6) affichant les dernières mesures du système Environnement (température, humidité, pression, luminosité), lues via l'API FastAPI du backend.

## Installation

```bash
pip install -r requirements.txt
```

## Lancement

1. Démarrer l'API (depuis `server/database`, là où vit `main.py`) :

```bash
uvicorn main:app --reload
```

2. Démarrer le widget :

```bash
python environnement_dashboard.py
```

Le widget interroge `http://127.0.0.1:8000/api/environnement` toutes les 5 secondes et affiche la dernière mesure de chaque capteur.
