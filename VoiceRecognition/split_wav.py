import wave
import struct
import math
import os
from numpy import frombuffer, int16

# make directory to put split data
if os.path.exists("voiceset") == False:
    os.mkdir("voiceset")

def cut_wav(file_name, time, speaker, out_num):

    # load voice data
    wavf = file_name + ".wav"
    wr = wave.open(wavf, "r")

    # get attribute from wave files
    ch = wr.getnchannels()
    width = wr.getsampwidth()
    fr = wr.getframerate()
    fn = wr.getnframes()
    total_time = 1.0 * fn / fr
    integer = math.floor(total_time)
    t = int(time)
    frames = int(ch * fr * t)
    num_cut = int(integer // t)

    # confirmation
    print("Channel: ", ch)
    print("Sample width: ", width)
    print("Frame Rate: ", fr)
    print("Frame num: ", fn)
    print("Params: ", wr.getparams())
    print("Total time: ", total_time)
    print("Total time(integer)",integer)
    print("Time: ", t) 
    print("Frames: ", frames) 
    print("Number of cut: ",num_cut)

    # quantify voice data
    data = wr.readframes(wr.getnframes())
    wr.close()
    X = frombuffer(data, dtype=int16)
    print(X)

    for i in range(num_cut):
        print(i)
        outf = "voiceset/" + speaker + "_" + str(i + out_num) + ".wav"
        start_cut = i * frames
        end_cut = i * frames + frames
        print(start_cut)
        print(end_cut)
        Y = X[start_cut : end_cut]
        outd = struct.pack("h" * len(Y), *Y)

        # write data
        ww = wave.open(outf, "w")
        ww.setnchannels(ch)
        ww.setsampwidth(width)
        ww.setframerate(fr)
        ww.writeframes(outd)
        ww.close()

print("input name = ")
tmp = input()
print("input number = ")
num = input()
print("start of output number = ")
out_num = int(input())
f_name = tmp + "_" + num
print("cut time = ")
cut_time = input()

cut_wav(f_name, cut_time, tmp, out_num)
