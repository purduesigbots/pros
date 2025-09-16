import asyncio
import os
import re
import subprocess
import sys
import tempfile

include_directory = "include/pros"
precompiled_include_directory = ""
error_count = 0
gcc_semaphore = asyncio.Semaphore(value=os.cpu_count())

def print_and_exit(message):
    print(f"Failed to check example code in pros headers:\n{message}", file=sys.stderr)
    sys.exit(1)

def precompile_header():
    flags = [
        "-x", "c++-header",
        "include/main.h",
        "-std=c++23",
        "-I", "include",
        "-D", "_PROS_KERNEL_SUPPRESS_LLEMU_WARNING",
        "-Wfatal-errors",
        "-o", os.path.join(precompiled_include_directory, "main.h.gch")
    ]
    subprocess.run(["arm-none-eabi-gcc", *flags], check=True)

async def compile_code(code_text, filename, is_cpp):
    """
    Compiles the given code, discarding the output file.
    
    Args:
        code_text (str): The C/C++ source code to compile.
        is_cpp (bool): Whether the code is C++ or not.
        
    Returns:
        success (bool): Whether the compilation succeeded or not.
    """
    flags = [
        "-x", "c++" if is_cpp else "c",
        "-std=c++23" if is_cpp else "-std=c2x",
        # Only need precompiled headers for C++
        *(("-I", precompiled_include_directory) if is_cpp else ()),
        "-I", "include",
        # Prevent spurious warnings
        "-D", "_PROS_KERNEL_SUPPRESS_LLEMU_WARNING",
        # Stop compilation on first error
        "-Wfatal-errors",
        # Read input from stdin
        "-",
        # Generate assembly (avoids linker errors)
        "-S",
        # Discard the output file
        "-o", os.devnull
    ]
    async with gcc_semaphore:
        process = await asyncio.create_subprocess_exec(
            "arm-none-eabi-gcc",
            *flags,
            stdin=asyncio.subprocess.PIPE,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE
        )
        stdout, stderr = await process.communicate(input=code_text.encode("utf-8"))
        success = (process.returncode == 0)

        if not success:
            global error_count
            error_count += 1
            print(f"=== example code from {filename} failed to compile ===")
            print(code_text)
            print("=== compiler output below: ===")
            print(stderr.decode())
            print("========")
        
        return success

def example_code_generator():
    try:
        files = os.listdir(include_directory)
    except:
        print_and_exit(f"Error accessing directory '{include_directory}': {e}")
    try:
        for filename in files:
            file_path = os.path.join(include_directory, filename)
            if os.path.isfile(file_path):
                try:
                    with open(file_path, "r") as f:
                        header_text = f.read()
                        is_cpp = filename.endswith(".hpp")
                        # This pattern matches all lines between the \code and \endcode markers
                        pattern = r"(^.+\\code\n)((.*\n)+?)(^.+\\endcode)"
                        for match in re.finditer(pattern, header_text, re.MULTILINE):
                            code_block = match.group(2)
                            lines = code_block.splitlines()
                            # Remove the leading * from each line
                            cleaned_lines = [re.sub(r"^\s*\* ?", "", line) for line in lines]
                            include_apix = "#include \"pros/apix.h\"" if filename == "apix.h" else ""
                            code_snippet = f"#include \"main.h\"\n{include_apix}\n"+"\n".join(cleaned_lines)
                            yield code_snippet, filename, is_cpp
                except Exception as e:
                    print_and_exit(f"Error reading file '{filename}': {e}")
    except Exception as e:
        print_and_exit(f"Error accessing directory '{include_directory}': {e}")

async def main():
    with tempfile.TemporaryDirectory() as tmpdirname:
        global precompiled_include_directory
        precompiled_include_directory = tmpdirname
        precompile_header()
        async with asyncio.TaskGroup() as tg:
            tasks = [tg.create_task(compile_code(*item)) for item in example_code_generator()]
    results = [task.result() for task in tasks]
    global error_count
    if error_count > 0:
        print_and_exit(f"{error_count} compile errors encountered")

if __name__ == "__main__":
    asyncio.run(main())