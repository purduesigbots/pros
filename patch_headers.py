'''
Adds __attribute__((pcs("aapcs")) to all sdk function declarations to allow properly linking
with the sdk (which uses the soft float abi) when building pros with the hard float abi
'''

from os import listdir, makedirs
from os.path import isfile, join, exists, dirname, realpath

def print_and_exit(message):
    print(message)
    print("Failed to patch libv5rts")
    exit(1)

# get the current directory
try:
    current_dir = dirname(realpath(__file__))
except:
    print_and_exit("Could not get working directory!")
# set the include path and the include path of the patched SDK
include_path = current_dir + "/firmware/libv5rts/sdk/vexv5/include/"
patched_include_path = current_dir + "/firmware/libv5rts/sdk/vexv5/patched_include/"
# create the include path if it does not exist already
if exists(patched_include_path):
    print("libv5rts already patched")
    exit(0)
else:
    try:
        makedirs(patched_include_path)
    except:
        print_and_exit("Could not create directory for patched libv5rts")

# get a list of files in the include path
try:
    all_files = listdir(include_path)
except:
    print_and_exit("Error getting directories and files in include path")
files = []
for f in all_files:
    if isfile(join(include_path, f)) and str(f).endswith(".h"):
        files.append(f)
        
# patch headers
for file_name in files:
    header_file = ""
    output = ""
    skip_next_line = False
    try:
        with open(join(include_path, file_name), "r") as file:
            header_file = file.read()
    except:
        print_and_exit("Failed to open file in v5rts")
    for line in header_file.splitlines():
        if skip_next_line:
            # This line is a macro continuation
            if not line.endswith(("\\")):
                # macro continuation ends here
                skip_next_line = False
            continue
        if line.strip().startswith(("/", "*")):
            # it's a comment
            output += f"{line}\n"
            continue
        if line.strip().startswith(("#")):
            # it's a preprocessor directive
            output += f"{line}\n"
            if line.endswith(("\\")):
                skip_next_line = True
            continue
        if line.strip().startswith(("}", "{", "extern")):
            # it's an extern c block or struct definition
            output += f"{line}\n"
            continue
        if line.strip().startswith(("typedef", "struct", "enum")):
            # it's a struct/enum definition or typedef
            output += f"{line}\n"
            continue
        if not line or line.isspace():
            # it's blank
            output += f"{line}\n"
            continue
        if ");" not in line:
            # just in case something that isn't a function slips past the previous checks
            # ");" should only occur in a function definition
            output += f"{line}\n"
            continue
        output += f"__attribute__((pcs(\"aapcs\"))) {line}\n"
    try:
        with open(join(patched_include_path, file_name), "w") as file:
            file.write(output)
    except:
        print_and_exit("Failed to write patched file")

print("libv5rts patched successfully")