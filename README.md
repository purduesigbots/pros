# PROS for VEX v5

This repository hosts the kernel for PROS. The PROS kernel is the operating system and core features on the VEX v5 microcontroller.

The PROS kernel has the following features:
 * FreeRTOS Scheduler
 * FreeRTOS Heap
 * JINX, a graphical debugger
 * VDML, the end-user interface between CPU0 and CPU1
 * TMEI, the end-user graphical interface
 * Shell, a console for interacting with the microcontroller in real-time and provides real-time scripting capabilities

More documentation soon :)

## Using Arcanist
If on Windows, I highly recommend using Bash on Windows to set up Arcanist. It will likely be much less of a headache.

1. Install php and clang-format (`sudo apt install php clang-format`)
2. Download arcanist, libphutil, and clang-format-linter into a directory you won't delete... `~` will probably suffice for Bash on Windows
```
git clone https://github.com/phacility/libphutil.git
git clone https://github.com/phacility/arcanist.git
git clone https://github.com/vhbit/clang-format-linter.git
```
3. Add arcanist to PATH by appending to `~/.bashrc` or `~/.zshrc` or similar file
```
export PATH="$PATH:/somewhere/arcanist/bin"
```
Be sure to source the file after editing it by running `source ~/.bashrc`
4. Configure Conduit keys by running `arc install-certificate`


To propose changes, edit files as you normally would, then when ready to push/merge into master:
1. Complete merge/commit locally
2. Run `arc diff`

