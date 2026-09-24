# 🚀 Horizon 2080

Projet réalisé dans le cadre du **Workshop B3 – Horizon 2080**.

L'objectif du projet est de concevoir plusieurs systèmes embarqués permettant d'améliorer les conditions de vie, la sécurité et l'autonomie des humains à bord d'un **vaisseau spatial autonome effectuant un voyage de plusieurs décennies**.

---

## 🛰️ Architecture du projet

Le projet est composé de plusieurs systèmes physiques équipés de capteurs et pilotés par des **ESP32** et **ESP8266**.

Les données sont transmises par Wi-Fi vers un **Raspberry Pi**, qui centralise les communications et les données.

```text

                  ┌──────────────────────┐
                  │     Raspberry Pi     │
                  │                      │
                  │   Point d'accès Wi-Fi│
                  │   Mosquitto MQTT     │
                  │   FastAPI / Uvicorn  │
                  │   SQLite             │
                  └──────────┬───────────┘
                             │
                ┌────────────┼────────────┐
                │            │            │
              Wi-Fi        Wi-Fi        Wi-Fi
                │            │            │
          ┌─────▼─────┐ ┌────▼──────┐ ┌──▼─────────┐
          │   ESP32   │ │   ESP32   │ │  ESP8266   │
          │  Sécurité │ │           | Environnement│
          └───────────┘ └───────────┘ └────────────┘
```

---

## 🔧 Matériel

### Serveur

* Raspberry Pi 3B – 1 Go
* Carte microSD
* Alimentation USB-C
* Boîtier

### Systèmes embarqués

* 2 x ESP32
* 1 x ESP8266

### Capteurs

#### 🌍 Environnement

* BME280

  * Température
  * Humidité de l'air
  * Pression atmosphérique
* BH1750

  * Luminosité

#### 🛡️ Sécurité / surveillance

* Capteur d'CO₂

### Prototypage

* Breadboards
* Câbles Dupont
* Câbles USB

---

## 📡 Communication

La communication entre les ESP et le Raspberry Pi repose sur **MQTT**.

Chaque ESP publie les données de ses capteurs sur des topics MQTT.

Exemple :

```text
vaisseau/environnement/temperature
vaisseau/environnement/humidite
vaisseau/environnement/luminosite
vaisseau/environnement/co2
```

Le Raspberry Pi joue le rôle de serveur central et héberge le **broker MQTT Mosquitto**.

Le backend récupère ensuite les données et les enregistre dans la base de données.

---

## 🖥️ Dashboard

Une interface web permet de centraliser les informations provenant des différents systèmes.

Elle permettra notamment de :

* consulter les mesures en temps réel ;
* afficher les alertes ;
* consulter l'historique des données ;
* surveiller les conditions environnementales du vaisseau.

---

## 📁 Organisation du dépôt

```text
horizon-2080/
│
├── README.md
├── .gitignore
├── .env.example
├── compose.yaml
│
├── mosquitto/
│   └── config/
│       ├── mosquitto.conf
│       └── passwd
│
├── esp8266/
│   └── environnement/
│       ├── src/
│       └── README.md
│
├── esp32/
│   └── securite/
│       ├── src/
│       └── README.md
│
├── server/
│   ├── backend/
│   ├── database/
│   └── README.md
│
├── web/
│   ├── frontend/
│   └── README.md
│
├── docs/
    ├── architecture/
    ├── capteurs/
    ├── reseau/
    └── presentation/
```

---

## 🌿 Systèmes

### ESP8266 #1 — Environnement

Surveillance des conditions environnementales du vaisseau :

* température ;
* humidité de l'air ;
* luminosité.
* cO2

---

### ESP32 #2 — Sécurité

Surveillance de paramètres liés à la sécurité et à l'environnement :

*

---

## 🗄️ Serveur

Le Raspberry Pi centralise les différents services :

```text
Raspberry Pi
│
├── Mosquitto
│   └── MQTT
│
├── FastAPI / Uvicorn
│   ├── API
|   └── Interface Web
│
└── SQLite
    └── Stockage des mesures
```

---

## 🌐 Réseau

Les ESP32 et ESP8266 se connectent au réseau Wi-Fi fourni par le Raspberry Pi.

```text
 ESP32 / ESP8266
        │
        │ Wi-Fi
        ▼
 Raspberry Pi
        │
        └── Mosquitto MQTT
```

---

## 🌿 Structure Git

La branche `main` contient uniquement une version fonctionnelle et stable du projet.

Les développements sont réalisés sur des branches dédiées :

```text
main
│
├── feature/backend-mqtt
├── feature/frontend
└── feature/database
```

### Workflow

1. Créer une branche depuis `main`.
2. Développer la fonctionnalité.
3. Tester localement.
4. Commit les modifications.
5. Pousser la branche sur GitLab.
6. Créer une Merge Request.
7. Faire relire la modification.
8. Fusionner dans `main` si la fonctionnalité est fonctionnelle.

### Convention de commit

Exemples :

```text
feat: ajout lecture temperature @utilisateur
feat: ajout publication MQTT @utilisateur
fix: correction connexion wifi @utilisateur
fix: correction topic mqtt @utilisateur
docs: ajout schema reseau @utilisateur
refactor: separation lecture et envoi des donnees @utilisateur
```

---

## 📚 Documentation

La documentation technique est regroupée dans le dossier `docs/`.

```text
docs/
│
├── architecture/
│   └── architecture.md
│
├── capteurs/
│   └── capteurs.md
│
├── reseau/
│   └── reseau.md
│
└── presentation/
    └── ...
```

---

## 🎯 Objectifs

Le projet doit permettre de :

* surveiller les conditions de vie à bord ;
* détecter certaines situations pouvant représenter un risque ;
* centraliser les informations provenant des différents systèmes ;
* permettre une supervision depuis une interface web ;
* conserver un historique des données ;
* proposer une architecture pouvant être étendue à de futurs systèmes.

---

## 🔮 Évolutions possibles

L'architecture pourra être étendue avec de nouveaux systèmes ou capteurs :

* contrôle automatique de l'éclairage ;
* détection d'anomalies ;
* gestion énergétique ;
* nouvelles mesures environnementales ;
* système d'alertes avancé ;
* contrôle à distance des équipements ;
* analyse des données par IA.

---

## 📌 État du projet

> 🚧 Projet en cours de développement.

Les fonctionnalités et composants seront ajoutés progressivement au cours du Workshop B3.