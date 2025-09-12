import os
import re
import subprocess
import sys
import time
import traceback

include_directory = "include/pros"
is_error = False

def print_and_exit(message):
    print(f"Failed to check example code in pros headers:\n{message}")
    sys.exit(1)

max_subprocesses = os.cpu_count()
current_subprocesses = 0
subprocess_list = []

def compile_code(code_text, filename, is_cpp):
    """
    Compiles the given code, discarding the output file.
    
    Args:
        code_text (str): The C/C++ source code to compile.
        filename (str): The name of the file.
        is_cpp (bool): Whether the code is C++ or not.
    """
    global max_subprocesses, current_subprocesses, subprocess_list
    while current_subprocesses == max_subprocesses:
        for i in range(len(subprocess_list)):
            proc, (code_snippet, filename) = subprocess_list[i]
            if isinstance(proc, subprocess.Popen):
                retcode = proc.poll()
                if retcode is not None:
                    current_subprocesses -= 1
                    success = (retcode == 0)
                    if not success:
                        global is_error
                        is_error = True
                    stdout, stderr = proc.stdout.read(), proc.stderr.read()
                    subprocess_list[i] = (success, stdout, stderr), (code_snippet, filename)
        time.sleep(1)
    while len(subprocess_list) > 0 and not isinstance(subprocess_list[0][0], subprocess.Popen):
        (success, stdout, stderr), (code_snippet, filename) = subprocess_list.pop(0)
        if not success:
            print(f"=== example code from {filename} failed to compile: ===")
            print(code_snippet)
            print("=== compiler output below: ===")
            print(stderr)
    flags = [
        "arm-none-eabi-gcc",
        "-x", "c++" if is_cpp else "c",
        "-std=c++23" if is_cpp else "-std=c2x",
        "-I", "include",
        "-D", "_PROS_KERNEL_SUPPRESS_LLEMU_WARNING",
        "-Wfatal-errors",
        "-",                                         # Read input from stdin
        "-S",                                        # Generate assembly (avoids linker errors)
        "-o", os.devnull                             # Discard the output file
    ]
    process = subprocess.Popen(
        flags,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    process.stdin.write(code_text)
    process.stdin.close()
    subprocess_list.append((process, (code_text, filename)))
    current_subprocesses += 1

#def compile_code__(code_text, is_cpp):
#    """
#    Compiles the given code, discarding the output file.
#    
#    Args:
#        code_text (str): The C/C++ source code to compile.
#        is_cpp (bool): Whether the code is C++ or not.
#        
#    Returns:
#        (success: bool, stdout: str, stderr: str)
#    """
#    flags = [
#        "arm-none-eabi-gcc",
#        "-x", "c++" if is_cpp else "c",
#        "-std=c++23" if is_cpp else "-std=c2x",
#        "-I", "include",
#        "-D", "_PROS_KERNEL_SUPPRESS_LLEMU_WARNING",
#        "-",                                         # Read input from stdin
#        "-S",                                        # Generate assembly (avoids linker errors)
#        "-o", os.devnull                             # Discard the output file
#    ]
#
#    process = subprocess.Popen(
#        flags,
#        stdin=subprocess.PIPE,
#        stdout=subprocess.PIPE,
#        stderr=subprocess.PIPE
#    )
#    stdout, stderr = process.communicate(input=code_text.encode("utf-8"))
#    success = (process.returncode == 0)
#
#    return success, stdout.decode(), stderr.decode()

def check_example_code(filename, header_text, is_cpp):
    """
    Checks all the example code snippets in the header file and ensures they compile.

    Args:
        filename (str): The name of the header file.
        header_text (str): The content of the file.
        is_cpp (bool): Whether the file is C++ or not.
    """
    print(f"checking all examples in {filename} ...", file=sys.stderr)
    # This pattern matches all lines between the \code and \endcode markers
    pattern = r"(^.+\\code\n)((.*\n)+?)(^.+\\endcode)"
    for match in re.finditer(pattern, header_text, re.MULTILINE):
        code_block = match.group(2)
        lines = code_block.splitlines()
        # Remove the leading * from each line
        cleaned_lines = [re.sub(r"^\s*\*", "", line) for line in lines]
        code_snippet = "#include \"main.h\"\n"+"\n".join(cleaned_lines)
        compile_code(code_snippet, filename, is_cpp)

try:
    for filename in os.listdir(include_directory):
        file_path = os.path.join(include_directory, filename)
        if os.path.isfile(file_path):
            try:
                with open(file_path, "r") as f:
                    check_example_code(filename, f.read(), filename.endswith(".hpp"))
            except Exception as e:
                print_and_exit(f"Error reading file '{filename}': {e}")
except Exception as e:
    print_and_exit(f"Error accessing directory '{include_directory}': {e}")

if is_error:
    sys.exit(1)
