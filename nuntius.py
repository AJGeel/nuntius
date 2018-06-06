import os
import glob
import subprocess
from time import sleep
import RPi.GPIO as GPIO
from datetime import datetime
import serial
arduinoSerialData = serial.Serial('/dev/ttyACM0', 9600)

# Defining the GPIOs
GPIO.setmode(GPIO.BCM)
GPIO.setup(14,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(15,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(18,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(23,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(24,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(26,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(19,GPIO.IN, pull_up_down=GPIO.PUD_UP)

sleep(20)

# Load news items from the directory, loads current time
os.chdir('news')
newsItems = glob.glob('*.ogg')
newsItems.sort()
cHour = datetime.now().time().hour

# Plays correct start-up theme, addresses time of day
print(" ")
if cHour > 6:
    if cHour < 12:
        print("Good morning, here's your personalized news.")
    elif cHour < 18:
        print("Good afternoon, here's your personalized news.")
    elif cHour < 22:
        print("Good evening, here's your personalized news.")
else:
    print("Good night, here's your personalized news.")
print(" ")

print(newsItems)

# Flags and status items for playback control
allNewsItems = len(newsItems)
flag = 1
index = 0
st = 0
isPlaying = True

# Toggles for lighting commands to arduino
inoCommands = ['1', '2', '3', '4', '5']
comIndex = 0

def sendNextCommand():
    global comIndex
    if comIndex < len(inoCommands) - 1:
        comIndex += 1
    else:
        comIndex = 0
    arduinoSerialData.write(inoCommands[comIndex])

# Eventlistener loop for GPIO pins 
while True:
    if flag == 1:
        arduinoSerialData.write('4')
        player = subprocess.Popen(["omxplayer",newsItems[index]],stdin=subprocess.PIPE) #,stdout=subprocess.PIPE,stderr=subprocess.PIPE
        fi = player.poll()
        flag = 0
        st = 0

    if (GPIO.input(14) == False):
        if isPlaying == True:
            arduinoSerialData.write('5')
            print("Playback paused")
            isPlaying = False
        else:
            arduinoSerialData.write('4')
            print("Playing resumed")
            isPlaying = True
            
        sleep(0.5)
        fi = player.poll()
        if fi!=0:
            player.stdin.write("p") 
            
        

    if (GPIO.input(15)==False):
        print("Button 25 pressed: STOP AUDIO PLAYBACK")
        sleep(0.5)
        fi = player.poll()
        if fi != 0:
            player.stdin.write("q")  
            st = 1
       
    if (GPIO.input(18)==False):
	arduinoSerialData.write('4')
        print("Button 18 pressed: PLAY NEXT NEWS ITEM")
        isPlaying = True
        if st == 0:
            player.stdin.write("q")      
        flag = 1
        index += 1
        if index > allNewsItems - 1:
            index = 0
        sleep(0.5)


    if (GPIO.input(26)==False):
        print("Button 26 pressed: NEXT LIGHTING STATE")
        sendNextCommand()
        sleep(0.5)


    elif (GPIO.input(23)==False):
	arduinoSerialData.write('4')
        print("Button 23 pressed: GO BACK 30 SECONDS")
        isPlaying = True
        player.stdin.write("^[[D")
        sleep(0.5)

    else:
        fi = player.poll()
        if (fi == 0 and st == 0):
            flag = 1
            index += 1
            if index > allNewsItems - 1:
                index = 0
    sleep(0.1)

