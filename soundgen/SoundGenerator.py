import numpy as np
import simpleaudio as sa
import time

class SoundGenerator:
    def __init__(self, f, sr, dur): # receive input of frequency, sampling rate, duration, and mode: 'wait' or 'skip'
        self.frequency = f  # Played note frequency
        self.fs = sr  # 44100 samples per second is the default, use this number if you don't have idea about
        self.seconds = dur  # Note duration in second


        # Generate array with seconds*sample_rate steps, ranging between 0 and seconds
        self.t = np.linspace(0, self.seconds, self.seconds * self.fs, False)

        # Generate a sine wave with input frequency
        self.note = np.sin(self.frequency * self.t * 2 * np.pi)

        # Ensure that highest value is in 16-bit range
        self.audio = self.note * (2**15 - 1) / np.max(np.abs(self.note))
        # Convert to 16-bit data
        self.audio = self.audio.astype(np.int16)

    def play(self, config):
        # Start playback
        play_obj = sa.play_buffer(self.audio, 1, 2, self.fs)

        # Wait for playback to finish before exiting if mode = wait
        if config == "wait":
            play_obj.wait_done()