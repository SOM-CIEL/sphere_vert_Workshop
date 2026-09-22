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
            pression REAL,
            luminosite REAL
        )
    """)

    if conn.execute("SELECT COUNT(*) FROM environnement").fetchone()[0] == 0:
        conn.execute("""
            INSERT INTO environnement
            (temperature, humidite, pression, luminosite)
            VALUES (NULL, NULL, NULL, NULL)
        """)

    conn.commit()
    conn.close()