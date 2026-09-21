# 🚀 Horizon 2080

Projet réalisé dans le cadre du **Workshop B3 – Horizon 2080**.

L'objectif du projet est de concevoir plusieurs systèmes embarqués permettant d'améliorer les conditions de vie, la sécurité et l'autonomie des humains à bord d'un **vaisseau spatial autonome effectuant un voyage de plusieurs décennies**.

---

## 🛰️ Architecture du projet

Le projet est composé de plusieurs systèmes physiques équipés de capteurs et pilotés par des **ESP32**.

Les données sont transmises par Wi-Fi vers un **Raspberry Pi**, qui centralise les communications et les données.

```text
                  ┌──────────────────────────┐
                  │   ESP32 Environnement    │
                  │ Temp. / Hum. / Pression  │
                  │       / Luminosité       │
                  └────────────┬─────────────┘
                               │
                               │ Wi-Fi / MQTT
                               │
                  ┌────────────▼─────────────┐
                  │    ESP32 AgriTech        │
                  │    Humidité du sol       │
                  └────────────┬─────────────┘
                               │
                               │
                  ┌────────────▼─────────────┐
                  │    ESP32 Sécurité        │
                  │       O₂ / Ultrason      │
                  └────────────┬─────────────┘
                               │
                               │ Wi-Fi / MQTT
                               ▼
                  ┌──────────────────────────┐
                  │      Raspberry Pi        │
                  │                          │
                  │    Mosquitto MQTT        │
                  │    Backend / API         │
                  │    Base de données       │
                  │    Serveur Web           │
                  └────────────┬─────────────┘
                               │
                               │ HTTP / WebSocket
                               ▼
                  ┌──────────────────────────┐
                  │      Dashboard Web       │
                  │                          │
                  │  Données en temps réel   │
                  │  État des systèmes       │
                  │  Alertes / historique    │
                  └──────────────────────────┘
```

---

## 🔧 Matériel

### Serveur

* Raspberry Pi 4 – 2 Go
* Carte microSD
* Alimentation USB-C
* Boîtier et refroidissement

### Systèmes embarqués

* 3 × ESP32

### Capteurs

#### 🌍 Environnement

* BME280

  * Température
  * Humidité de l'air
  * Pression atmosphérique
* BH1750

  * Luminosité

#### 🌱 AgriTech

* Capteur d'humidité du sol

#### 🛡️ Sécurité / surveillance

* Capteur d'O₂
* Capteur ultrason

### Prototypage

* Breadboards
* Câbles Dupont
* Câbles USB

---

## 📡 Communication

La communication entre les ESP32 et le Raspberry Pi repose sur **MQTT**.

Chaque ESP32 publie les données de ses capteurs sur des topics MQTT.

Exemple :

```text
vaisseau/environnement/temperature
vaisseau/environnement/humidite
vaisseau/environnement/pression
vaisseau/environnement/luminosite

vaisseau/agritech/humidite_sol

vaisseau/securite/o2
vaisseau/securite/distance
```

Le Raspberry Pi joue le rôle de serveur central et héberge le **broker MQTT Mosquitto**.

Le backend récupère ensuite les données et les enregistre dans la base de données.

---

## 🖥️ Dashboard

Une interface web permet de centraliser les informations provenant des différents systèmes.

Elle permettra notamment de :

* consulter les mesures en temps réel ;
* visualiser l'état des différents systèmes ;
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
├── esp32/
│   ├── environnement/
│   │   ├── src/
│   │   └── README.md
│   │
│   └── agritech/
│       ├── src/
│       └── README.md
│
├── esp8266/
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
│   ├── architecture/
│   ├── capteurs/
│   ├── reseau/
│   └── presentation/
│
└── hardware/
    ├── schemas/
    ├── cablage/
    └── composants.md
```

---

## 🌿 Systèmes

### ESP32 #1 — Environnement

Surveillance des conditions environnementales du vaisseau :

* température ;
* humidité de l'air ;
* pression ;
* luminosité.

---

### ESP32 #2 — AgriTech

Surveillance des conditions nécessaires aux cultures :

* humidité du sol.

Les données permettent de suivre l'état des cultures et pourront servir à de futures fonctions d'automatisation.

---

### ESP32 #3 — Sécurité

Surveillance de paramètres liés à la sécurité et à l'environnement :

* concentration d'O₂ ;
* distance / présence d'un obstacle.

---

## 🗄️ Serveur

Le Raspberry Pi centralise les différents services :

```text
Raspberry Pi
│
├── Mosquitto
│   └── MQTT
│
├── Backend
│   └── API
│
├── Database
│   └── Stockage des mesures
│
└── Web Server
    └── Dashboard
```

---

## 🌐 Réseau

Les ESP32 communiquent avec le Raspberry Pi via le réseau Wi-Fi.

```text
ESP32
  │
  │ Wi-Fi
  ▼
Réseau local
  │
  ▼
Raspberry Pi
  │
  └── Mosquitto MQTT
```

Chaque ESP32 possède une identité permettant au serveur d'identifier la provenance des données.

---

## 🌿 Structure Git

La branche `main` contient uniquement une version fonctionnelle et stable du projet.

Les développements sont réalisés sur des branches dédiées :

```text
main
│
├── feature/esp32-environnement
├── feature/esp32-agritech
├── feature/esp32-securite
├── feature/backend-mqtt
├── feature/web-dashboard
└── feature/server
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

## 👥 Répartition de l'équipe

### Développement

* Développeur 1 → ESP32 Environnement
* Développeur 2 → ESP32 AgriTech
* Développeur 3 → ESP32 Sécurité
* Développeur 4 → Backend / MQTT / Base de données
* Développeur 5 → Interface Web

### Réseau

* Réseau 1 → Architecture réseau / Wi-Fi / MQTT
* Réseau 2 → Raspberry Pi / Serveur / Déploiement

La répartition peut évoluer selon les besoins du projet.

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

Les schémas électroniques et de câblage sont placés dans :

```text
hardware/
├── schemas/
├── cablage/
└── composants.md
```

---

## 🎯 Objectifs

Le projet doit permettre de :

* surveiller les conditions de vie à bord ;
* surveiller les cultures ;
* détecter certaines situations pouvant représenter un risque ;
* centraliser les informations provenant des différents systèmes ;
* permettre une supervision depuis une interface web ;
* conserver un historique des données ;
* proposer une architecture pouvant être étendue à de futurs systèmes.

---

## 🔮 Évolutions possibles

L'architecture pourra être étendue avec de nouveaux systèmes ou capteurs :

* automatisation de l'irrigation ;
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