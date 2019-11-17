import pygame
import time

pygame.mixer.pre_init(44100, -16, 2, 4096)
pygame.mixer.init()
try:
    a = pygame.mixer.Sound("soundgen-multiPlay/note/A.wav")
    pygame.mixer.Sound.play(a)
except:
    print("Error play")
    pass
pygame.time.delay(1000)