import serial
from datetime import datetime
import time
import json
import serial
import numpy as np
from sklearn.ensemble import IsolationForest
from joblib import load, dump
from threading import Thread
from collections import Counter
import warnings
warnings.filterwarnings('ignore')
import mariadb
from tools.preprocess_lib import * #library bang kris


t = datetime.now()
print(t)

def update(jsonData) :
    ID = jsonData['ID']
    humidity = jsonData['A']
    temperature = jsonData['B']
    heat = jsonData['C']
    pm25= jsonData['D']
    pm10 = jsonData['E']
    analogvib = jsonData['F']
    digitalvib= jsonData['G']
    analogsound= jsonData['H']
    timenow = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
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
    time.sleep(1)
    

if __name__ == '__main__':
    ser = serial.Serial('COM11', 9600)
    ser.reset_input_buffer()

    while True:

        if ser.in_waiting > 0:
            # line = ser.readline().decode('utf-8').rstrip()
            # # print(type(line))
            # print(line,t1-t)
            # t = t1
            t1 = datetime.now()
            try:
                data = (ser.readline().decode('utf-8').rstrip())
                data = data.replace("'", '"')
                data = data.replace('"{', "{")
                data = data.replace('}"', "}")
                jsonData = json.loads(data)
                tre = Thread(target=update, args=(jsonData,))
                tre.start()
                tre.join()
            except:
                jsonData = {
                    'Data': "Kosong",
                }
            bot.send_message(jsonData)
            print(jsonData, type(jsonData), t1-t)
            t = t1
            

# value =1