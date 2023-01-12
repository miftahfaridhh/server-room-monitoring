import serial
from datetime import datetime
import time
import json
import serial
from threading import Thread
import warnings
warnings.filterwarnings('ignore')
import mariadb
# from tools.preprocess_lib import * #library bang kris
from telegrambotalarm import TelegramBot

TOKEN = '5984463318:AAG714F-PCHhutZwkCXI-vSboctmwbtGhAU'
MYID = '-711603736' #scipaper group
# MYID = '1246189043' #personal

bot = TelegramBot(TOKEN, MYID)

t = int(datetime.now().strftime('%H%M%S'))
print(t)


def tele(jsonData) : 
    bot.send_message(jsonData)


def update(jsonData) :
    ID = jsonData['ID']
    humidity = jsonData['A']
    temperature = jsonData['B']
    heat = jsonData['C']
    concentration25= jsonData['D']
    pm25= jsonData['E']
    concentration10= jsonData['F']
    pm10 = jsonData['G']
    analogvib = jsonData['H']    
    digitalvib= jsonData['I']
    analogsound= jsonData['J']
    timenow = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    # for key, value in jsonData.items():
    #     print(value)

    # db_conn = mariadb.connect(host="113.198.211.95", user="admina", password="admina", database="SCIHUB", port=3306)
    db_conn = mariadb.connect(host="210.123.42.64", user="login", password="123", database="server_room_data", port=3306)
    db_cursor = db_conn.cursor() #scihub
    db_command1 = f"INSERT INTO `monitoring` (`Date`, `ID`, `humidity`, `temperature`, `heat`, `concentration25`, `pm25`, `concentration10`, `pm10`, `analogvib`, `digitalvib`, `analogsound`) VALUES ('{timenow}', \
        {ID},{humidity},{temperature},{heat},{concentration25},{pm25},{concentration10},{pm10},{analogvib},{digitalvib},{analogsound})"
    # print(db_command1)
    db_cursor.execute(db_command1)
    db_conn.commit()
    db_cursor.close()
    db_conn.close()    
    time.sleep(1)
    

if __name__ == '__main__':
    ser = serial.Serial('COM5', 9600)
    ser.reset_input_buffer()
    a=1

    while True:

        waktuuji = int(datetime.now().strftime('%H%M%S'))
        # print(waktuuji-t)
        if (waktuuji-t)%60==0:
            # print(waktuuji-t)
            # a+=1
            ser.close()
            ser = serial.Serial('COM5', 9600) #'/dev/ttyACM0'
            ser.reset_input_buffer()

        if ser.in_waiting > 0:
            # print(ser.in_waiting)
            t1 = int(datetime.now().strftime('%H%M%S'))

            try:
                data = ser.readline().decode('utf-8').rstrip()
                # print(data)
                data = data.replace("'", '"')
                data = data.replace('"{', "{")
                data = data.replace('}"', "}")
                jsonData = json.loads(data)
                tre = Thread(target=update, args=(jsonData,))
                tre.start()
                tre.join()
                tre2 = Thread(target=tele, args=(jsonData,))
                tre2.start()
                tre2.join()
                beda = t1-t
                t = t1
                print(jsonData, "delay:",beda)
                
            except:
                jsonData = {
                    'Data': "Kosong",
                }

            if (t1-t)>50:
                ser.reset_input_buffer()