##### SOUNDBOX v1.0 #####
# To do:    1. Parallel note delay test
#           2. Performance tuning (code cleanup?)

# Library import
import serial
import os
import pygame
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
pygame.mixer.pre_init(44100, -16, 2, 4096)  # Configuration : (sampling freq, bit depth, channel, buffer size)
pygame.mixer.init()

# Sound files load
A = pygame.mixer.Sound(path + "\\SoundBox\\soundgen-multiPlay\\note\\A.wav")
B = pygame.mixer.Sound(path + "\\SoundBox\\soundgen-multiPlay\\note\\B.wav")
C = pygame.mixer.Sound(path + "\\SoundBox\\soundgen-multiPlay\\note\\C.wav")
D = pygame.mixer.Sound(path + "\\SoundBox\\soundgen-multiPlay\\note\\D.wav")
E = pygame.mixer.Sound(path + "\\SoundBox\\soundgen-multiPlay\\note\\E.wav")
F = pygame.mixer.Sound(path + "\\SoundBox\\soundgen-multiPlay\\note\\F.wav")
G = pygame.mixer.Sound(path + "\\SoundBox\\soundgen-multiPlay\\note\\G.wav")
H = pygame.mixer.Sound(path + "\\SoundBox\\soundgen-multiPlay\\note\\H.wav")

while True:
    # Append serial read to data
    data.append(str(arduino.readline()))
    
    # Convert from raw serial data
    data = clean_data(data)
    note = data[0]
    print("note: ",note)

    # Play music from file
    try:
        # print(notePath) # Debugging notePath
        if note == 'A': pygame.mixer.Sound.play(A)
        elif note == 'B': pygame.mixer.Sound.play(B)
        elif note == 'C': pygame.mixer.Sound.play(C)
        elif note == 'D': pygame.mixer.Sound.play(D)
        elif note == 'E': pygame.mixer.Sound.play(E)
        elif note == 'F': pygame.mixer.Sound.play(F)
        elif note == 'G': pygame.mixer.Sound.play(G)
        elif note == 'H': pygame.mixer.Sound.play(H)
    except:
        print("File not found!")

    # Reset data buffer
    data = []