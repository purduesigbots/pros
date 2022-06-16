# PROS Kernel for the VEX V5 Brain

[![Build Status](https://dev.azure.com/purdue-acm-sigbots/Kernel/_apis/build/status/purduesigbots.pros?branchName=develop)](https://dev.azure.com/purdue-acm-sigbots/Kernel/_build/latest?definitionId=5&branchName=develop)

### What is PROS?
PROS is a lightweight and fast alternative open source operating system for the VEX V5 Brain. PROS is built with developers in mind and with a focus on providing an environment for industry-applicable experience.

Primary maintenance of PROS is done by students at Purdue University through [Purdue ACM SIGBots](http://purduesigbots.com). Inspiration for this project came from several computer science and engineering students itching to write code for the extended autonomous period. We created PROS to leverage this opportunity.

All PROS development is open sourced and available for the community to inspect. We believe that sharing source improves PROS through community contributions and allows students to learn by example.

PROS is built using the GCC toolchain and standard C/C++ practices (C11 & C++17 w/GNU extensions) to make the learning curve small. Structures, pointers, dynamic memory allocation, and function pointers are all available. Additionally, code is run on bare metal, allowing you to take full advantage of the microcontroller's power.

You can develop code on Windows, OS X, or Linux. Code is compiled using GCC and the PROS CLI is available on most operating systems. The PROS development team makes distributions for Windows, OS X, and Debian-based Linux distributions.

The PROS team develops a plugin for Atom to making developing projects in PROS the best possible experience. The highly customizable editor designed for the 21st century enables students to learn how to code in a modern environment.

### What's the difference between PROS 2 and PROS 3?
PROS 2 refers to the kernel that runs on the [VEX Arm Cortex-based Microcontroller](https://www.vexrobotics.com/276-2194.html). The source for this kernel is still available on the `cortex-master` branch. The future development for this version of the PROS kernel will be focused on maintenance and critical bugfixes.

PROS 3 refers to the kernel that runs on the [VEX V5](https://www.vexrobotics.com/vexedr/v5) microcontroller platform. The majority of our development focus will be on this version of the PROS kernel.

### Does PROS support C++?
- PROS 3.x (V5) officially supports C++. We're still working on enabling all of the features of C++ (particularly in the I/O area).
- PROS 2.x (Cortex) does not officially support C++. Some users have found ways around this, but be warned: we will not be able to help if you run into issues doing this.

### Cool, how do I get it?
Pay a visit to our website, [pros.cs.purdue.edu](https://pros.cs.purdue.edu), to download our latest installer or view installation instructions for your preferred platform.

### How do I use it?
We have a number of resources available on our website, including
- [A basic tutorial to get you acquainted with the PROS ecosystem](https://pros.cs.purdue.edu/v5/getting-started/new-users.html)
- [A series of tutorials on how to use some of the more advanced features of PROS](https://pros.cs.purdue.edu/v5/tutorials/index.html)
- [Extensive documentation on the kernel's API](https://pros.cs.purdue.edu/v5/api/index.html)

### I still have questions!
Drop us a line
- at pros_development@cs.purdue.edu
- on the [VEX Forum](https://www.vexforum.com/)
- on [VEX Teams of the World Discord](https://discord.gg/xddjWGj)

### I think I found a bug!
We maintain GitHub repositories for the three major components of the PROS ecosystem:
- The Command Line Interface (cli) at [purduesigbots/pros-cli](https://github.com/purduesigbots/pros-cli)
- The VS Code plugin at [purduesigbots/pros-vsc](https://github.com/purduesigbots/pros-vsc)
- The kernel [here](https://github.com/purduesigbots/pros)

If you find a problem with our documentation or tutorials, we have a repository for that, too, at [purduesigbots/pros-docs](https://github.com/purduesigbots/pros-docs).

### Hey! Why can't I build the PROS 3 kernel?
The PROS 3 kernel depends on VEX's proprietary Software Development Kit (SDK), which is not publicly available.
