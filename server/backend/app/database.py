import sqlite3
from pathlib import Path

DB_PATH = Path("/app/database/horizon_2080.db")


def get_db_connection():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn


def init_db():
    conn = get_db_connection()

    conn.execute("""
        CREATE TABLE IF NOT EXISTS environnement (
            temperature REAL,
            humidite REAL,
            luminosite REAL,
            co2 REAL
        )
    """)

    conn.execute("""
        CREATE TABLE IF NOT EXISTS mesures (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp DATETIME DEFAULT (datetime('now', '+2 hours')),
            temperature REAL,
            humidite REAL,
            luminosite REAL,
            co2 REAL
        )
    """)

    if conn.execute("SELECT COUNT(*) FROM environnement").fetchone()[0] == 0:
        conn.execute("""
            INSERT INTO environnement
            (temperature, humidite, luminosite, co2)
            VALUES (NULL, NULL, NULL, NULL)
        """)

    conn.commit()
    conn.close()