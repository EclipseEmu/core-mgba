import os
import shutil


prefix_path = "./Sources/mGBA/header_prefix.h"
src = "./Sources/mGBA/mGBA/include/"
dst = "./Sources/mGBA/include/"

try:
    shutil.rmtree(dst)
except:
    pass

os.makedirs("./Sources/mGBA/include", exist_ok=True)
shutil.copyfile("./Sources/mGBA/noop_log.h", "./Sources/mGBA/include/noop_log.h")

for root, dirs, files in os.walk(src):
    for file in files:
        src_filename = root + os.sep + file
        dst_filename = root.replace(src, dst) + os.sep + file

        if file == "getopt.h":
            continue

        os.makedirs(os.path.dirname(dst_filename), exist_ok=True)
        with open(dst_filename, "w") as f:
            with open(prefix_path, "r") as pf:
                f.write(pf.read())
            with open(src_filename, "r") as sf:
                f.write(sf.read())
