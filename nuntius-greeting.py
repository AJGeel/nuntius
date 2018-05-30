''' 
This program ensures Nuntius is able to greet its users with relevant and dynamic (pre-recorded) voice-overs. It fetches
.ogg soundbytes by constructing a semi-random url, based on the current time of the day.

By Arthur Geel, 30-05-2018                                                                                              
'''

from datetime import datetime
import random

ext = '.ogg'
num = 1
file = ''

# String variables for the four time slots of the day.
gm = 'good-morning-'
ga = 'good-afternoon-'
ge = 'good-evening-'
gn = 'good-night-'

# Function that generates a new number from one to five.
def newNum():
    global num
    num = random.randint(1, 5)

# Compiles the file name by looking at time of day, random number and filetype extension.
def getFile(timeOfDay):
    newNum()
    global file
    file = timeOfDay + str(num) + ext

# Gets the current system time.
cHour = datetime.now().time().hour

# Use the time of day to tailor the greeting. Time of day is used as an argument in the getFile function.
if cHour > 6:
    if cHour < 12:
        getFile(gm)
    elif cHour < 18:
        getFile(ga)
    elif cHour < 22:
        getFile(ge)
else:
    getFile(gn)

# prints filename to console.
print(file)
