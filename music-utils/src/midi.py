import os
from mido import MidiFile, tick2second

# ----------------------------
# SETTINGS
# ----------------------------
FRAME_MS = 20
FRAME_SEC = FRAME_MS / 1000

A4 = 440.0

def midi_to_freq(n):
    return int(round(A4 * (2 ** ((n - 69) / 12))))

# ----------------------------
# LOAD MIDI
# ----------------------------
INPUT_FILE = "midi_files/Megalovania.mid"
midi = MidiFile(INPUT_FILE)

events = []

# ----------------------------
# EXTRACT NOTE EVENTS (WITH SUSTAIN)
# ----------------------------
for track in midi.tracks:
    abs_time = 0
    tempo = 500000

    for msg in track:
        abs_time += msg.time

        if msg.type == "set_tempo":
            tempo = msg.tempo

        if msg.type == "note_on" and msg.velocity > 0:
            t = tick2second(abs_time, midi.ticks_per_beat, tempo)
            events.append((t, "on", msg.note))

        elif msg.type == "note_off" or (msg.type == "note_on" and msg.velocity == 0):
            t = tick2second(abs_time, midi.ticks_per_beat, tempo)
            events.append((t, "off", msg.note))

# ----------------------------
# SORT EVENTS
# ----------------------------
events.sort(key=lambda x: x[0])

if not events:
    print("No notes found")
    exit()

# ----------------------------
# BUILD FRAME GRID
# ----------------------------
end_time = events[-1][0]
num_frames = int(end_time / FRAME_SEC) + 1

active = set()
frames = []

i = 0

for f in range(num_frames):
    t = f * FRAME_SEC

    while i < len(events) and events[i][0] <= t:
        _, typ, note = events[i]

        if typ == "on":
            active.add(note)
        else:
            active.discard(note)

        i += 1

    if active:
        frames.append(midi_to_freq(max(active)))
    else:
        frames.append(0)

# ----------------------------
# RUN-LENGTH ENCODING (RLE)
# ----------------------------
song = []

if frames:
    prev = frames[0]
    count = 1

    for i in range(1, len(frames)):
        if frames[i] == prev:
            count += 1
        else:
            song.append((prev, count))
            prev = frames[i]
            count = 1

    song.append((prev, count))

# ----------------------------
# OUTPUT FILE SETUP
# ----------------------------
os.makedirs("results", exist_ok=True)

base_name = os.path.splitext(os.path.basename(INPUT_FILE))[0]
output_file = f"results/{base_name}.txt"

# ----------------------------
# WRITE OUTPUT
# ----------------------------
with open(output_file, "w") as f:

    # DEBUG INFO
    f.write("--- DEBUG INFO ---\n")
    f.write(f"Input file: {INPUT_FILE}\n")
    f.write(f"Total MIDI events: {len(events)}\n")
    f.write(f"Total frames (raw): {len(frames)}\n")
    f.write(f"RLE entries: {len(song)}\n")
    f.write(f"Frame size (ms): {FRAME_MS}\n")
    f.write(f"Total duration (sec): {end_time:.2f}\n")
    f.write("-------------------\n\n")

    # ARRAY OUTPUT
    f.write("const Note song[] = {\n")

    for i, (freq, length) in enumerate(song):
        if i % 5 == 0:
            f.write("    ")

        f.write(f"{{{freq}, {length}}}, ")

        if i % 5 == 4:
            f.write("\n")

    f.write("\n};\n")

    # LENGTH OUTPUT
    f.write(f"\nconst int song_length = {len(song)};\n")

print(f"Saved output to: {output_file}")