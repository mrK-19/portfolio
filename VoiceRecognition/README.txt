Voice Recognition  -- How to use --

1. record.py
You can record your voice or a video sound with any length you want.
After "How long do you want to record? ", type any integer (the unit is second).
And you can record any voice or sound by following "recording...".

2. split_wav.py
This code can split a wav file into some files with certain length.
Before spliting a wav file, your wav file's name must be "(name)_(number).wav".
(If you want to make this code adjust to any file name, you can change the variable "f_name" in line 70.)

3. voice_id_random.py
This code will split voice files in voiceset into training data and testing data.
Then, AI extracts the feature called MFCC from training data.
Finally, AI identifies the speaker of each testing voice file and output acuuracy of identification.

4. voice_id_specific.py
This code is basically the same as code 3, but you can identify a specific voice file, not randomly.
After "input filename = ", type the file name without ".wav".

5. voice_realtime.py
This code is basically the same as code 3, but you can identify the voice in real time.
After "recording...", let the computer listen to any voice you want to identify.

6. voiceset
In this project, I took voices of 5 voice actresses (Kana Hanazawa, Ayana Taketatsu, Miku Ito, Ayane Sakura, Inori Minase).
Note that each file name is "(name)_(number).wav".
/* I took these voices on the Internet. Be careful that the copyright belongs to each voice actress. */
