import os
import glob
import subprocess
from time import sleep
import RPi.GPIO as GPIO
from datetime import datetime


# Defining the GPIOs
GPIO.setmode(GPIO.BCM)
GPIO.setup(14,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(15,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(18,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(23,GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(24,GPIO.IN, pull_up_down=GPIO.PUD_UP)

sleep(20)

# Load news items from the directory, loads current time
os.chdir('news')
newsItems = glob.glob('*.ogg')
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

# Eventlistener loop for GPIO pins 
while True:
    if flag == 1:
        player = subprocess.Popen(["omxplayer",newsItems[index]],stdin=subprocess.PIPE) #,stdout=subprocess.PIPE,stderr=subprocess.PIPE
        fi = player.poll()
        flag = 0
        st = 0

    # Toggles play/pause.
    if (GPIO.input(14) == False):
        if isPlaying == True:
            print("Playback paused")
            isPlaying = False
        else:
            print("Playing resumed")
            isPlaying = True
            
        #print(pt)
        sleep(0.5)
        fi = player.poll()
        if fi!=0:
            player.stdin.write("p")      # pin 15 pause
            
        
    # Stops audio from playing. Press twice to stop program.
    if (GPIO.input(15)==False):
        print("15 HAS BEEN PRESSED // STOP")
        sleep(0.5)
        fi = player.poll()
        if fi != 0:
            player.stdin.write("q")      # pin 16 stop
            st = 1

    # Plays next item in queue.
    if (GPIO.input(18)==False):
        print("18 HAS BEEN PRESSED // NEXT")
        if st == 0:
            player.stdin.write("q")      # pin 18 Next Audio
        flag = 1
        index += 1
        if index > allNewsItems - 1:
            index = 0
        sleep(0.5)


    # Unfinished. Adds current news item to favourites.
    if (GPIO.input(24)==False):
        print("ITEM ADDED TO FAVOURITES")
        sleep(0.5)

    # Plays previous item in queue.
    elif (GPIO.input(23)==False):
        print("23 HAS BEEN PRESSED // PREVIOUS")
        if st == 0:
            player.stdin.write("q")      # pin 22 Next Audio
        flag = 1
        index -= 1
        if index < 0:
            index = allNewsItems - 1
        sleep(0.5)

    # Restarts playlist if at the very end of the queue.
    else:
        fi = player.poll()
        if (fi == 0 and st == 0):
            flag = 1
            index += 1
            if index > allNewsItems - 1:
                index = 0
    sleep(0.1)
