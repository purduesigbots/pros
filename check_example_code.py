import os
import re
import subprocess
import sys

include_directory = "include/pros"
is_error = False

def print_and_exit(message):
    print(f"Failed to check example code in pros headers:\n{message}")
    sys.exit(1)

def compile_code(code_text, is_cpp):
    """
    Compiles the given code, discarding the output file.
    
    Args:
        code_text (str): The C/C++ source code to compile.
        is_cpp (bool): Whether the code is C++ or not.
        
    Returns:
        (success: bool, stdout: str, stderr: str)
    """
    flags = [
        "arm-none-eabi-gcc",
        "-x", "c++" if is_cpp else "c",
        "-std=c++23" if is_cpp else "-std=c2x",
        "-I", "include",
        "-D", "_PROS_KERNEL_SUPPRESS_LLEMU_WARNING",
        "-",                                         # Read input from stdin
        "-S",                                        # Generate assembly (avoids linker errors)
        "-o", os.devnull                             # Discard the output file
    ]

    process = subprocess.Popen(
        flags,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    stdout, stderr = process.communicate(input=code_text.encode("utf-8"))
    success = (process.returncode == 0)

    return success, stdout.decode(), stderr.decode()

def check_example_code(filename, header_text, is_cpp):
    """
    Checks all the example code snippets in the header file and ensures they compile.

    Args:
        filename (str): The name of the header file.
        header_text (str): The content of the file.
        is_cpp (bool): Whether the file is C++ or not.
    """
    print(f"checking example in {filename} ...")
    # This pattern matches all lines between the \code and \endcode markers
    pattern = r"(^.+\\code\n)((.*\n)+?)(^.+\\endcode)"
    for match in re.finditer(pattern, header_text, re.MULTILINE):
        code_block = match.group(2)
        lines = code_block.splitlines()
        # Remove the leading * from each line
        cleaned_lines = [re.sub(r"^\s*\*", "", line) for line in lines]
        code_snippet = "#include \"main.h\"\n"+"\n".join(cleaned_lines)
        is_success, compiler_stdout, compiler_stderr = compile_code(code_snippet, is_cpp)
        if not is_success:
            print(f"=== example code from {filename} failed to compile: ===")
            print(code_snippet)
            print("=== compiler output below: ===")
            print(compiler_stderr)
            global is_error
            is_error = True

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
