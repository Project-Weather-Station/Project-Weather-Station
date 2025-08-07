from flask import Flask, request
from utils.database import execute_query, execute_select_query
from utils.response1 import create_response

app = Flask(__name__)

# ------------------ TEMPERATURE ------------------
@app.route('/temperature', methods=['POST', 'GET', 'DELETE'])
def temperature():
    if request.method == 'GET':
        query = "SELECT * FROM sensor WHERE type = 'Temperature' AND sensor = 'DHT11';"
        results = execute_select_query(query)
        return create_response(results)

    elif request.method == 'POST':
        value = request.get_json().get('value')
        type = "Temperature"
        sensor = "DHT11"
        query = f"INSERT INTO sensor(type, sensor, value) VALUES('{type}', '{sensor}', {value});"
        execute_query(query)
        return create_response("Temperature from DHT11 inserted successfully")

    elif request.method == 'DELETE':
        return create_response("Delete method not allowed for temperature", error=True)

# ------------------ HUMIDITY ------------------
@app.route('/humidity', methods=['POST', 'GET', 'DELETE'])
def humidity():
    if request.method == 'GET':
        query = "SELECT * FROM sensor WHERE type = 'Humidity' AND sensor = 'DHT11';"
        results = execute_select_query(query)
        return create_response(results)

    elif request.method == 'POST':
        value = request.get_json().get('value')
        type = "Humidity"
        sensor = "DHT11"
        query = f"INSERT INTO sensor(type, sensor, value) VALUES('{type}', '{sensor}', {value});"
        execute_query(query)
        return create_response("Humidity from DHT11 inserted successfully")

    elif request.method == 'DELETE':
        return create_response("Delete method not allowed for humidity", error=True)

# ------------------ RAIN SENSOR ------------------
@app.route('/rain', methods=['POST', 'GET', 'DELETE'])
def rain():
    if request.method == 'GET':
        query = "SELECT * FROM sensor WHERE type = 'Rain' AND sensor = 'RainSensor';"
        results = execute_select_query(query)
        return create_response(results)

    elif request.method == 'POST':
        value = request.get_json().get('value')
        type = "Rain"
        sensor = "RainSensor"
        query = f"INSERT INTO sensor(type, sensor, value) VALUES('{type}', '{sensor}', {value});"
        execute_query(query)
        return create_response("Rain sensor value inserted successfully")

    elif request.method == 'DELETE':
        return create_response("Delete method not allowed for rain", error=True)

# ------------------ SOIL SENSOR ------------------
@app.route('/soil', methods=['POST', 'GET', 'DELETE'])
def soil():
    if request.method == 'GET':
        query = "SELECT * FROM sensor WHERE type = 'Soil' AND sensor = 'SoilSensor';"
        results = execute_select_query(query)
        return create_response(results)

    elif request.method == 'POST':
        value = request.get_json().get('value')
        type = "Soil"
        sensor = "SoilSensor"
        query = f"INSERT INTO sensor(type, sensor, value) VALUES('{type}', '{sensor}', {value});"
        execute_query(query)
        return create_response("Soil moisture value inserted successfully")

    elif request.method == 'DELETE':
        return create_response("Delete method not allowed for soil", error=True)

# ------------------ MAIN ------------------
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=4000, debug=True)
