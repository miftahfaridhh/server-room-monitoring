import json
import serial
 
connection = serial.Serial('COM6', 57600)
connection.reset_input_buffer()
 
while True:
    data = connection.readline().decode("utf-8")
    # print(data)
    try:
        dict_json = json.loads(data)
        print(dict_json)
    except json.JSONDecodeError as e:
        print("JSON:", e)