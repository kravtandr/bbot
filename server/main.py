from fastapi import FastAPI
from fastapi.responses import HTMLResponse

app = FastAPI()

@app.get("/")
async def root():
    dic = open("/home/anya/abot/server/abot_dictionary.txt", "r")
    gram = open("/home/anya/abot/server/abot_gram.gram", "r")
    coords = open("/home/anya/abot/server/coordinates.csv", "r")
    return {"commands": dic.read(), "grammar": gram.read(), "coordinates": coords.read()}
