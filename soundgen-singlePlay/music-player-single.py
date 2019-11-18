# Library import
import serial
import os
from SoundGenerator import SoundGenerator   # (optional) Generate frequency-based signal
import time

# Cleaning received data from arduino
def clean_data(arduino_data):  # data is a list
    new_list = []  # initialising the new list
    temp = arduino_data[0][2:]  # cut junk characters at the beginning
    new_list.append(temp[:-5])  # cut junk characters at the end
    return new_list    

# Global variable initialization
data = []
BAUD_RATE = 57600
arduino = serial.Serial("COM5", timeout=None, baudrate=BAUD_RATE)  # Check port, waiting for serial input (no timeout)
path = os.getcwd()

# Main program (infinite loop)
print("Starting program")
while True:
    # Append serial read to data
    data.append(str(arduino.readline()))
    
    # Convert from raw serial data
    data = clean_data(data)
    note = data[0]
    print("note: ",note)
    #notePath = path + "\\soundgen\\note\\" + note + ".m4a" # Set playing file name relative to working project directory ==> ${WORKFOLDER}/soundgen/note/*.m4a

    # Play music from file
    """ try:
        os.startfile(notePath)   # Open file with name declared in notePath
    except:
        print("File not found!") """

    # Using SoundGenerator library to generate signal -> SoundGenerator(signal freq, sampling freq, duration(s))
    # Option:   "skip" to start the sound and continue the code
    #           "wait" to start the sound and wait until it finish
    if note == 'A':
        sg = SoundGenerator(440, 44100, 1)
        sg.play("skip")
    elif note == 'B':
        sg = SoundGenerator(494, 44100, 1)
        sg.play("skip")
    elif note == 'C':
        sg = SoundGenerator(523, 44100, 1)
        sg.play("skip")
    elif note == 'D':
        sg = SoundGenerator(587, 44100, 1)
        sg.play("skip")
    elif note == 'E':
        sg = SoundGenerator(659, 44100, 1)
        sg.play("skip")
    elif note == 'F':
        sg = SoundGenerator(698, 44100, 1)
        sg.play("skip")
    elif note == 'G':
        sg = SoundGenerator(784, 44100, 1)
        sg.play("skip")
    elif note == 'H':
        sg = SoundGenerator(880, 44100, 1)
        sg.play("skip")

    # Reset data buffer
    data = []