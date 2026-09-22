import sqlite3

conn = sqlite3.connect('horizon_2080.db')
cursor = conn.cursor()

# Création de la table environnement
cursor.execute('''
CREATE TABLE IF NOT EXISTS environnement (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    temperature REAL,
    humidite REAL,
    pression REAL,
    luminosite REAL
)
''')

# Insertion d'une donnée de test
cursor.execute("INSERT INTO environnement (temperature, humidite, pression, luminosite) VALUES (22.5, 45.2, 1013.2, 350)")

conn.commit()
conn.close()
print("Table environnement créée et donnée de test insérée !")