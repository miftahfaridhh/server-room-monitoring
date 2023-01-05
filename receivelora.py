import serial
from datetime import datetime as time
import json
import serial
import numpy as np
from sklearn.ensemble import IsolationForest
from joblib import load, dump
import threading
from collections import Counter
import warnings
warnings.filterwarnings('ignore')
import mariadb


t = time.now()

timenow = time.now().strftime("%Y-%m-%d %H:%M:%S")

print(t)
if __name__ == '__main__':
    ser = serial.Serial('COM8', 9600)
    ser.reset_input_buffer()
    while True:

        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            # print(type(line))
            t1 = time.now()
            # print(line,t1-t)
            t = t1
            data = (ser.readline().decode('utf-8').rstrip())
            data = data.replace("'", '"')
            data = data.replace('"{', "{")
            data = data.replace('}"', "}")
            jsonData = json.loads(data)
            print(jsonData, type(jsonData))

            ID = 2
            humidity = jsonData['Humidity']
            temperature = jsonData['Temperature C']
            heat = jsonData['Heat Index C']
            pm25= jsonData['PM25']
            pm10 = jsonData['PM10']
            analogvib = jsonData['Analog Vib']
            digitalvib= jsonData['Digital Vib']
            analogsound= jsonData['Digital Sound']
            
            # for key, value in jsonData.items():
            #     print(value)

            db_conn = mariadb.connect(host="113.198.211.95", user="admina", password="admina", database="SCIHUB", port=3306)
            db_cursor = db_conn.cursor()
            db_command1 = f"INSERT INTO `scihub` (`Date`, `ID`, `humidity`, `temperature`, `heat`, `pm25`, `pm10`, `analogvib`, `digitalvib`, `analogsound`) VALUES ('{timenow}', \
                {ID},{humidity},{temperature},{heat},{pm25},{pm10},{analogvib},{digitalvib},{analogsound})"
            # print(db_command1)
            db_cursor.execute(db_command1)
            db_conn.commit()

    db_cursor.close()
    db_conn.close()
# value =1