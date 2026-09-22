import os
from fastapi import FastAPI
from contextlib import asynccontextmanager
from .database import get_db_connection, init_db
from fastapi_mqtt import FastMQTT, MQTTConfig
import asyncio

mqtt_config = MQTTConfig(
    host=os.getenv("MQTT_HOST"),
    port=int(os.getenv("MQTT_PORT")),
    username=os.getenv("MQTT_USERNAME"),
    password=os.getenv("MQTT_PASSWORD"),
    keepalive=int(os.getenv("MQTT_KEEPALIVE"))
    )
fast_mqtt = FastMQTT(config=mqtt_config)


@asynccontextmanager
async def lifespan(app: FastAPI):
    init_db()
    await fast_mqtt.mqtt_startup()
    task = asyncio.create_task(sauvegarder_mesures())
    print("startup termine")

    yield

    task.cancel()
    await fast_mqtt.mqtt_shutdown()

app = FastAPI(lifespan=lifespan)


@fast_mqtt.on_connect()
def connect(client, flags, rc, properties):
    client.subscribe("vaisseau/#")
    print("Connecte a vaisseau/#", flush=True)



@fast_mqtt.on_message()
async def message(client, topic, payload, qos, properties):
    value = float(payload.decode())

    conn = get_db_connection()
    
    if topic == "vaisseau/environnement/temperature":
        conn.execute("UPDATE environnement SET temperature = ?", (value,))

    elif topic == "vaisseau/environnement/humidite":
        conn.execute("UPDATE environnement SET humidite = ?", (value,))

    elif topic == "vaisseau/environnement/pression":
        conn.execute("UPDATE environnement SET pression = ?", (value,))

    elif topic == "vaisseau/environnement/luminosite":
        conn.execute("UPDATE environnement SET luminosite = ?", (value,))
        
    conn.commit()
    conn.close()
    print(f"Topic: {topic}, Value: {value}", flush=True)



async def sauvegarder_mesures():
    while True:
        conn = get_db_connection()
        donnees = conn.execute("SELECT * FROM environnement").fetchone()
        conn.execute("""
        INSERT INTO mesures
        (temperature, humidite, pression, luminosite)
        VALUES (?, ?, ?, ?)
        """, (donnees["temperature"], donnees["humidite"], donnees["pression"], donnees["luminosite"]))
        conn.commit()
        conn.close()
        await asyncio.sleep(500)




@app.get("/")
async def root():
    return {"message": "Horizon 2080 backend"}

@app.get("/environnement")
async def get_environnement():
    conn = get_db_connection()
    donnees = conn.execute("SELECT * FROM environnement").fetchone()
    conn.close()

    return donnees


@app.get("/mesures")
async def get_mesures():
    conn = get_db_connection()
    donnees = conn.execute("SELECT * FROM mesures").fetchall()
    conn.close()

    return donnees