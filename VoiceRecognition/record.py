import sounddevice as sd
from scipy.io.wavfile import write

fs = 44100

name = input("Whose voice do you want to record? ")
second = int(input("How long do you want to record? "))

print("recording...")
record_voice = sd.rec(int(second * fs), samplerate=fs, channels=2)
sd.wait()
print("stop recording...")
write(f"{name}_0.wav", fs, record_voice)
