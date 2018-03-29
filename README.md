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
Arcanist is a command line interface to some of phabricator's tools, including Differential which we will be using extensively for this project. The following are steps for setting up arcanist on your machine.

_Note: If on Windows, it is recommended to use [Ubuntu on WSL](https://msdn.microsoft.com/en-us/commandline/wsl/install-win10) to set up and use Arcanist._

1. Install php and clang-format (`sudo apt install php php-curl clang-format`)
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

## PROS 3.0 Workflow
When working on PROS 3.0-related stuff, please adhere to the following workflow:
1. Checkout a new branch from the latest master. Try to name your branch after whatever it is you're working on.
  ```
  git pull
  git checkout -b new-v5-feature
  ```
2. Work on your branch, committing frequently so other people can see the branch's history.
3. When you think your feature branch is ready to be merged into master, run the merge locally:
  ```
  git checkout master
  git pull
  git merge new-v5-feature
  ```
4. After you merge and fix all merge conflicts (if there were any), run `arc diff` to start the review process on phabricator.
5. If/when reviewers request changes, simply make your changes, commit them, then re-run `arc diff`.
6. When your differential is accepted by the reviewers, run `arc land` to push everything to master.
