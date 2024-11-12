from os import listdir, makedirs, walk
from os.path import isfile, join, exists, dirname, realpath, splitext

def print_and_exit(message):
    print(message)
    print("Failed to amalgamate files")
    exit(1)

# get the current directory
try:
    current_dir = dirname(realpath(__file__))
except:
    print_and_exit("Could not get working directory!")

src_dir = current_dir + "/src"
amalgamate_src_dir = current_dir + "/amalgamate-src"

if not exists(amalgamate_src_dir):
    try:
        makedirs(amalgamate_src_dir)
    except:
        print_and_exit("Could not create directory for amalgamation!")

c_src_files = [join(dp, f) for dp, dn, filenames in walk(f"{current_dir}/src") for f in filenames if
                  splitext(f)[1].lower() == '.c']
cpp_src_files = [join(dp, f) for dp, dn, filenames in walk(f"{current_dir}/src") for f in filenames if
                  splitext(f)[1].lower() == '.cpp']

with open(amalgamate_src_dir + "/amalgamate.c", "w") as dest:
    for c_file in c_src_files:
        if "/test/" in c_file:
            continue
        with open(c_file) as f: 
            contents = f.read()
            dest.write(f"#line 1 \"{c_file}\"\n")
            dest.write(contents + "\n")

with open(amalgamate_src_dir + "/amalgamate.cpp", "w") as dest:
    for cpp_file in cpp_src_files:
        if "/test/" in cpp_file or "main.cpp" in cpp_file:
            continue
        with open(cpp_file) as f: 
            contents = f.read()
            dest.write(f"#line 1 \"{cpp_file}\"\n")
            dest.write(contents + "\n")

with open(amalgamate_src_dir + "/main.cpp", "w") as dest:
    with open(src_dir + "/main.cpp", "r") as src:
        dest.write(src.read())
