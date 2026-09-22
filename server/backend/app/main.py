import os
from fastapi import FastAPI
from contextlib import asynccontextmanager

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
    print("demarrage mqtt")
    await fast_mqtt.mqtt_startup()
    print("startup termine")
    yield
    await fast_mqtt.mqtt_shutdown()

app = FastAPI(lifespan=lifespan)


@fast_mqtt.on_connect()
def connect(client, flags, rc, properties):
    print("CALLBACK CONNECT APPELE", flush=True)
    client.subscribe("vaisseau/#")
    print("Connecte a vaisseau/#", flush=True)

@fast_mqtt.on_message()
async def message(client, topic, payload, qos, properties):
    print(f"Message recu: {payload.decode()}", flush=True)


@app.get("/")
async def root():
    return {"message": "Horizon 2080 backend"}