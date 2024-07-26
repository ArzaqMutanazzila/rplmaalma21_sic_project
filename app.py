from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
from flask_socketio import SocketIO
import pandas as pd
from model import model, lb

app = Flask(__name__)
CORS(app)
socketio = SocketIO(app)

sensor_data = {
    "ambient_temp_c": 0,
    "ambient_temp_f": 0,
    "object_temp_c": 0,
    "object_temp_f": 0,
    "alarm": False
}

@app.route('/', methods=["GET"])
def index():
    return render_template("index.html")

@app.route('/sensor', methods=["POST"])
def post_sensor_values():
    global sensor_data
    data = request.json
    sensor_data.update(data)
    socketio.emit("update", sensor_data)
    return "1" if sensor_data["alarm"] else "0"

@app.route('/sensor', methods=["GET"])
def get_sensor_values():
    result = sensor_data.copy()
    pred = model.predict(pd.DataFrame({
        "ambient_c": [result["ambient_temp_c"]],
        "object_c": [result["object_temp_c"]]
        })).tolist()
    result['detect'] = pred[0]
    sensor_data["alarm"] = result['detect'] == 0
    return jsonify(result)

if __name__ == "__main__":
    socketio.run(app, debug=True, host="0.0.0.0")
