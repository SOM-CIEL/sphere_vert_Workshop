import os
from fastapi import FastAPI
from contextlib import asynccontextmanager
from .database import get_db_connection, init_db
from fastapi_mqtt import FastMQTT, MQTTConfig

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
    print("startup termine")
    yield
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



@app.get("/")
async def root():
    return {"message": "Horizon 2080 backend"}

@app.get("/environnement")
async def getEnvironnement():
    conn = get_db_connection()
    donnees = conn.execute("SELECT * FROM environnement").fetchall()
    conn.close()

    return donnees