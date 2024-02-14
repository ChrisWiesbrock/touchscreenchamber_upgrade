# -*- coding: utf-8 -*-
"""
Created on Tue Jan 16 15:37:10 2024

@author: wiesbrock

Logic of this script:

The Hallway is initalized to receive the ID from the RFID-chip. Once this happened, the ID is looked up in the "schedule.xslx"-file.
If there is a paradigm assigned to the mouse id, the next check is performed, otherwise the exit-routine starts (not programmed yet) 
If there is an assignment for the mouse id, it is checked by visually by a YOLO-algorithm if there is only one mouse in the hallway. 
If there are more mice in the hallway, the exit routine starts. 
If there is only one mouse, the first door will close, the second door will open and the paradigm is called. 
The links to the paradigm-scripts need to be included in the schedule.xlsx. 

I have to write the system call. Code for opening and closing doors is missing. 

To do: 

Import hallway functions instead of copying. 
Implement door opening and closing
Exit routine for now: Let first door open and play a sound?
Testing and adapting...
"""

from ultralytics import YOLO
import cv2
import math 

import numpy as np
import matplotlib.pylab as plt
import seaborn as sns
import pandas as pd
import scipy.stats as stats
import glob





import pandas as pd
from datetime import datetime

def __init__(self): # let's see what do we need for our inputs
    # just to have them declared
    self.serial_obj = object
    self.teensy_port = object
    self.ID = None

def call_hallway(self):
    self.initialize_teensy(moduleName="HallwayTeensy4")  # not sure if I need to use this
    self.teensy_port = find_teensy_port()
    self.ID = self.receive_bytes_from_teensy(self.teensy_port)

def initialize_teensy(self, moduleName="HallwayTeensy4"):
    # For now, I assume that only one chamber is connected per PC. If that changes then we would need to give each
    # Microcontroller a unique identifier and search for this specific setup.
    # run setup without Microcontroller
    test_mode = False  # for debugging
    if not test_mode:
        # If memory serves me well, this just throws an error if the module is not found and the program aborts with
        # this as intended
        self.serial_obj = search_for_microcontroller_by_name(moduleName)
        print('ready')
    else:
        for _ in range(25):
            print("WARNING! The setup is running in test mode. Only for debugging!")
        #
    #
#

def receive_bytes_from_teensy(self, teensy_port):
    id_from_teensy = receive_bytes_from_teensy(teensy_port)
    print("Mouse ID", id_from_teensy)
    return id_from_teensy

def get_signal():
    pass

def call_script(action):
    pass

def classification_and_schedule(mouse_id, schedule):
    path = schedule
    
    # Excel-Datei einlesen
    schedule = pd.read_excel(path, sheet_name='Schedule')
    log_sheet = pd.read_excel(path, sheet_name='Log')
    names=schedule.columns
    mouse_id=schedule[names[0]]
    paradigms=schedule[names[1]]
    
    # Neue Daten definieren
    timestamp = datetime.now()  # Aktueller Zeitstempel
    no_known_mouse=0
    id_=get_signal()
    id_ = input("Please add mouse id (comes via api) ")
    id_=int(id_)  # Ersetzen Sie dies durch die tatsächliche Mouse ID
    if len(paradigms[mouse_id==id_])==0:
        no_known_mouse=1
        print('Start Exit routine...')
        action=str('no paradigm assigned')
        action=np.array(action)
        print('Mouse ID not found')
        
    if no_known_mouse==0:
        action = paradigms[mouse_id==id_]
        action=np.array(action)
    #action=str(action)
    # Neue Datenreihe erstellen
    
    
    
        
    # start webcam
    cap = cv2.VideoCapture(0)
    cap.set(3, 640)
    cap.set(4, 480)
    
    # model
    model = YOLO("yolo-Weights/best.pt")
    
    # object classes
    classNames = [{"mouse"}]
    
    success, img = cap.read()
    results = model(img, stream=True)
    
    # coordinates
    for r in results:
        boxes = r.boxes
    
        for box in boxes:
            # bounding box
            x1, y1, x2, y2 = box.xyxy[0]
            x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2) # convert to int values
    
            # put box in cam
            cv2.rectangle(img, (x1, y1), (x2, y2), (255, 0, 255), 3)
    
            # confidence
            confidence = math.ceil((box.conf[0]*100))/100
            print("Confidence --->",confidence)
    
            # class name
            cls = int(box.cls[0])
            print("Class name -->", classNames[cls])
            check_number=len(boxes)
    
            # object details
            org = [x1, y1]
            font = cv2.FONT_HERSHEY_SIMPLEX
            fontScale = 1
            color = (255, 0, 0)
            thickness = 2
    
            cv2.putText(img, classNames[cls], org, font, fontScale, color, thickness)
            
            if check_number==1:
                print('close first door...')
                # Aktualisierte Excel-Datei speichern
                new_data = {'Timestamp': timestamp, 'Mouse ID': id_, 'Action': action}
                log_sheet = log_sheet.append(new_data, ignore_index=True)
                
                    
            if check_number!=1:
                print('Start exit routine...')
                action=str('multiple mice in hallway')
                action=np.array(action)
                new_data = {'Timestamp': timestamp, 'Mouse ID': id_, 'Action': action}
                log_sheet = log_sheet.append(new_data, ignore_index=True)
    
    
    with pd.ExcelWriter(path, engine='openpyxl', mode='a', if_sheet_exists='replace') as writer:
        log_sheet.to_excel(writer, sheet_name='Log', index=False)
                        
                
    
        #cv2.imshow('Webcam', img)
        #if cv2.waitKey(1) == ord('q'):
        #    break
    
    #cap.release()
    #cv2.destroyAllWindows()
    


path='schedule.xlsx'
teensy_port='COM11'
while True:
    __init__()
    call_hallway()
    initialize_teensy()
    mouse_id=receive_bytes_from_teensy(teensy_port)
    action=classification_and_schedule(mouse_id, path)
    call_script(action)
