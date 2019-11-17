import serial
import os
from SoundGenerator import SoundGenerator
import time


# function for cleaning the received data from arduino
def clean_data(arduino_data):  # data is a list
    new_list = []  # initialising the new list
    temp = arduino_data[0][2:]  # cut junk characters at the beginning
    new_list.append(temp[:-5])  # cut junk characters at the end
    return new_list

# function to play the music file
def play_from_file(file_name):
    try:
        os.startfile(file_name)  # play the music with the input file name, PUT THE FILE IN 1 FOLDER WITH THIS SOURCE CODE
    except:
        pass  # do nothing if the file is not found


# variable initializing
data = []
BAUD_RATE = 57600

# opening arduino serial port
try:
    arduino = serial.Serial("COM3", timeout=1, baudrate=BAUD_RATE)  # MAKE SURE THE BAUD RATE HERE MATCHES WITH THE ONE IN ARDUINO
except:
    print("Please check the port")

# infinite loop to keep receiving data from arduino
while True:
    # reading data from arduino, save to 'data' variable
    data.append(str(arduino.readline()))

    # cleaning the data
    data = clean_data(data)
    note = data[0]

    # play music from file
    play_from_file(note + '.M4A')

    # or play the sound using SoundGenerator library (PUT THE LIBRARY SOURCE CODE IN 1 FOLDER WITH THIS SOURCE CODE)
    sg = SoundGenerator(440, 44100, 3)  # sound configuration: frequency, sampling rate (leave it as it is if you have no idea about it), duration
    sg.play("skip")  # use "skip" to start the sound and continue the code, or use "wait" to start the sound and wait until it finish

    # reset the data variable
    data = []