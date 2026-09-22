from fastapi import FastAPI
import sqlite3

app = FastAPI()

def get_db_connection():
    conn = sqlite3.connect('horizon_2080.db')
    # Permet d'accéder aux colonnes par leur nom, idéal pour le JSON
    conn.row_factory = sqlite3.Row 
    return conn

@app.get("/api/environnement")
def lire_donnees():
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute("SELECT * FROM environnement")
    # Conversion des lignes SQLite en liste de dictionnaires
    donnees = [dict(row) for row in cursor.fetchall()] 
    
    conn.close()
    return donnees