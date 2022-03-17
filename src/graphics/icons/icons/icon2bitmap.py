import os
import shutil
from PIL import Image
import numpy as np

input_folder = "./images"
output_folder = "./bitmaps"


def write_header(name, width, height):
    with open(f"{output_folder}/{name}.h", 'w') as f:
        f.write(
f"""// THIS IS AN AUTOGENERATED FILE

#ifndef {name.upper()}_H
#define {name.upper()}_H

#include "../icon.h"

class {name} : public Icon {{
    public:
        {name}() : Icon({width}, {height}) {{}};
        const uint16_t* getBitmap();
        void draw(TFT_eSprite* sprite);
}};

#endif
""")

def write_cpp(name, width, height, data):
    with open(f"{output_folder}/{name}.cpp", 'w') as f:
        f.write(
f"""// THIS IS AN AUTOGENERATED FILE

#include "{name}.h"

static const uint16_t bitmap[] PROGMEM = {{ """)
        count = 0
        for i in range(len(data)):
            if count % width == 0:
                f.write('\n\t')
            f.write("0x%0.4x, " % data[i])
            count += 1
        f.write(
f"""
}};

const uint16_t* {name}::getBitmap() {{ return bitmap; }}

void {name}::draw(TFT_eSprite* sprite) {{ _draw(bitmap, sprite); }}
""")


if __name__ == '__main__':
    # check if input folder exists
    if not os.path.isdir(input_folder):
        print("Input folder is empty.")
        quit()

    # check if input folder contains any images
    files = os.listdir(input_folder)
    for file in files:
        if not ".png" in file:
            files.remove(file)
    print(f"Images found: {len(files)}")
    if len(files) == 0:
        quit()

    # clear output folder
    if os.path.isdir(output_folder):
        shutil.rmtree(output_folder)
    os.mkdir(output_folder)

    for file in files:
        image = Image.open(f"{input_folder}/{file}", 'r')

        if image.mode != "RGBA":
            print(f"{file} is not in RGBA format.")
            continue

        width, height = image.size
        data = []
        for y in range(height):
            for x in range(width):
                r, g, b, a = image.getpixel((x, y))
                pxl = int(a == 255) << 5
                pxl = (pxl + int(31 * r / 255)) << 5
                pxl = (pxl + int(31 * g / 255)) << 5
                pxl += int(31 * b / 255)
                data.append(pxl)
        name = file.split('.')[0]
        write_header(name, width, height)
        write_cpp(name, width, height, data)

        print(f"Converted {file}")
