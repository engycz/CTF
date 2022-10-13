from pathlib import Path
from PIL import Image

files = list(Path(".").rglob("*.png"))

cnt = 0
for file in files:
    cnt = cnt + 1
    if cnt % 1000 == 0:
        print(str(cnt)+'/'+str(len(files)))

    img = Image.open(str(file))
    px = img.load()
    if px[0,0] == (242, 121, 48):
        if px[125, 125] == (0, 133, 71):
            print(file)