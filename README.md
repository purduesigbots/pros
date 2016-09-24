![Build Status](http://jenkins.purduesigbots.com/job/PROS%20Kernel%20Build/badge/icon)

@mainpage Welcome to PROS!

@section Top Table of Contents

* @ref Overview
* @ref Installation
 * @ref Windows
 * @ref Linux
 * @ref Mac
* @ref Started
 * @ref Creating
 * @ref Writing
 * @ref Compiling
 * @ref Uploading
 * @ref Debugging
 * @ref Management
 * @ref Updating
* @ref Tutorial
 * @ref C
 * @ref Project
 * @ref Declaration
 * @ref Templates
  * @ref Sensors
  * @ref Actuators
  * @ref Tasks
 * @ref Common
 * @ref FAQ
* @ref Advanced
 * @ref Git
  * @ref GitSetup
  * @ref GitExternal
  * @ref GitAdd
  * @ref GitCommit
  * @ref GitUsage
 * @ref OutsideIDE
  * @ref CmdLine
  * @ref VisualStudio

@section Overview Overview and API Reference

PROS is a lightweight and fast alternative operating system for the VEX Cortex Microcontroller.
It features multitasking, low-level control, and Wiring compatible functions to harness the
full power of the Cortex.

Detailed, function by function PROS documentation can be found in the file references for API.h
and main.h.

@subsection Installation Installation

@subsubsection Windows Windows

* Obtain the latest version of PROS for Windows from the
  [download site](https://github.com/purduesigbots/pros/releases/latest). Select the 32 or 64-
  bit version to match the running version of Windows.
* Run the downloaded installer. If a dialog box appears requesting permission to modify the
  computer, select Yes; otherwise, PROS will not be able to install the IDE.
  ![Click YES on User Account Control pop-ups in Windows Vista, 7, and 8](@ref uac_install.png)
* Accept the license agreement and select an installation location. Recent revisions can be
  installed to any location; 2b03 and earlier must be installed to a user writable directory.
* On the install options page, select "Java" if Java is not installed. Select "Drivers" if
  the drivers required for the VEX Programming Cable have not been installed before.
  ![Java and the VEX drivers can be optionally installed by PROS](@ref driver_select.png)
* Click `Next` to install PROS. Additional prompts may appear for the installation of Java
  or the VEX drivers.
  ![Prompt to install VEX drivers for old programming cable](@ref pl2303_driver_windows.png)
  ![Prompt to install VEX drivers for new programming cable](@ref vex_driver_windows.png)
* After installation, PROS can be started via the Start Menu shortcut.

@ref Top

@subsubsection Linux Linux

* Obtain the latest version of PROS for Linux from the
  [download site](https://github.com/purduesigbots/pros/releases/latest). Select the 32 or 64-
  bit version to match the running version of Linux. *Some 64-bit computers may have 32-bit
  Linux installed. To find out for sure, use* `uname -m`.
  ![Use 'uname -m' to determine Linux 32/64 bit version](@ref unamem.png)
* In some versions of Ubuntu, double-clicking the downloaded `.deb` file will successfully
  install PROS and all its dependencies. If this does not work, open a terminal in the
  directory where PROS was downloaded and run `sudo dpkg -i prose*.deb; sudo apt-get -f install`
  ![Open a terminal in download location to install](@ref terminal_install.png)
* PROS will be installed to `/opt/pros`. Device rules and symlinks to the included toolchain
  may also be installed in `/usr/local/bin` and `/usr/lib`.
* After installation, PROS can be started through the `Development > PROS IDE` shortcut in the
  `Applications` or `Start` menu, or through the search feature in the desktop environment.
  ![PROS will appear in the main menu](@ref pros_shortcut_linux.png)
* *The direct tether to the VEX Cortex is not supported at this time, due to a lack of Linux
  driver support.* In Linux kernels `3.2` and newer (Ubuntu `12.04` and newer), both the new
  orange VEX Programming Kit and the older serial Programming Kit will work without installing
  any drivers.

@ref Top

@subsubsection Mac Mac OS X

* Obtain the latest version of PROS for Mac OS X from the
  [download site](https://github.com/purduesigbots/pros/releases/latest).
* Run the downloaded installer `.pkg`. Accept the license agreement to install PROS.
  ![PROS installation screen on Mac](@ref install_mac.png)
* Install compatible programming cable drivers from
  [the Prolific website](http://prolificusa.com/pl-2303hx-drivers/). *Restart the computer.*
* After installation, PROS can be started through the `Applications > PROS` shortcut or through
  the Spotlight or Applications List.
* *The direct tether to the VEX Cortex is not supported at this time, due to a lack of Mac OS X
  driver support.* The new orange VEX Programming Kit works by default, but the older serial
  cables can be unreliable on some machines.

@ref Top

@subsection License PROS License Agreement

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. You can obtain a copy of the license at http://mozilla.org/MPL/2.0/ or
https://github.com/purduesigbots/purdueros/blob/master/LICENSE.

@ref Top

@section Started Getting started

* After the PROS splash screen appears, a `Workspace Launcher` prompt may appear. The default
  location is usually acceptable, but if it serves better to move it to an easy-to-remember
  place, it can be moved here. The workspace is the default location of new PROS projects.
  ![Select a workspace, or check the checkbox to avoid on every startup](@ref workspace_select.png)
* If starting PROS for the first time, a warning may appear about *EGit could not detect where
  Git is installed* or *Check Home Directory*. These can be ignored, as they are related to
  configuration for the source control features of PROS. PROS will operate correctly with or
  without `Git` installed.
  ![Dialog box can be ignored](@ref git_warning.png)
  ![Dialog box can be ignored if the value of HOME is reasonable](@ref check_homedir.png)
* Go to the main PROS window from the home screen by pressing the `Workbench` button.
  ![Skip the start page](@ref goto_workbench.png)
* The PROS main window will appear.
  ![The PROS main window](@ref pros_noproject.png)


@ref Top

@subsection Creating Creating a new project

* To create a new, blank PROS project for the VEX Cortex, select `File > New > VEX Cortex PROS
  Project`. Enter a short name *without spaces or punctuation* into the dialog box. To change
  the location where the project will be saved, deselect `Use default location` and enter an
  alternative location (*preferably accessible to the whole team...*). Click `Finish` to create
  the project.
  ![Creating a new PROS project](@ref pros_newprojectmenu.png)
  ![Enter the project's name to create in workspace](@ref pros_newprojectwizard.png)
* The new project will appear in the `Project Explorer` view. This view is useful to navigate
  among the multiple files present in each PROS project. To make code easier to maintain, the
  autonomous and driver control are split by default into two files, `auto.c` and `opcontrol.c`.
  ![PROS default project layout](@ref pros_defaultproject.png)
* The project is self-contained; copying the folder and all the files inside is sufficient for
  another PROS user to work on the code.

@ref Top

@subsection Writing Writing code

* Double-clicking on a file in the `Project Explorer` will open it in a text editor in the main
  window.
* PROS uses standard C code. How to code in PROS is explained in @ref Tutorial. Syntax
  highlighting, auto-indent, and auto-complete (accessible using `CTRL + SPACE`) can make coding
  fast and joyful.
* *Remember to* save (`CTRL + S`) the code often. PROS will auto-save files with a confirmation
  dialog, but this popup gets annoying quickly.

@ref Top

@subsection Compiling Compiling the project

* Before code can be run, it must be *compiled*, which translates the coder's intentions into a
  format that the Cortex Microcontroller can understand. To compile code, save any open files
  and press `CTRL + B`.
* At this time, PROS IDE will compile the code and look for errors. *Not all errors will be
  caught at this stage*; e.g. division by zero will often not be found until runtime. But
  mistakes in syntax such as leaving out a `;` will cause the code to fail compilation.
* A summary of all errors and warnings is available in the `Problems` tab on the bottom.
  Double-click a message to go to the location where the error occurred.
  ![Errors and warnings shown in the Problems dialog](@ref error_messages.png)
  ![Double-clicking goes to the error line with a red X marker](@ref error_description.png)
* One error can cause others. Try fixing the first error on the list and recompiling before
  tackling the others, unless the cause of subsequent errors is obvious.

@ref Top

@subsection Uploading Uploading the project

* When the project compiles correctly, upload the project to the robot by using either the
  `Upload` toolbar button, the `VEX > Upload to Cortex` menu item, or the `F7` keyboard
  shortcut.
  ![Two methods of uploading to the Cortex Microcontroller, or just press F7](@ref pros_upload.png)
* Uploading, by default, will erase all files on the PROS File System. If file data stored on
  the VEX Cortex should be preserved, use the `VEX > File System > Upload Preserving Filesystem`
  menu item; *this will be slower than a regular upload*.
* Files can be transferred to and from the VEX Cortex using the other options in the
  `File System` sub-menu. Copying files to the VEX Cortex can be accomplished with
  `Write File to Cortex`, while copying files from the VEX Cortex is done with
  `Read File from Cortex`. *The PROS File System cannot store more than one program at a time
  and is intended for storing data or logs.*
  ![The `File System` menu transfers files to and from the VEX Cortex](@ref pros_upload_fs.png)
* Robots must be plugged in and turned on to upload code. If an error occurs stating that no
  device could be found, make sure that all connections are plugged in tightly and try again.
  ![Error message when robots are unplugged or powered off](@ref pros_noplug.png)
* Uploading performance may be poor over wireless VEXnet if there is a significant amount of
  active wireless devices in the area. Consider moving the robot close to the joystick when
  uploading, or using the orange tether cable. *Uploading over VEXnet with a VEX Competition
  Switch plugged in may fail*; remove any connection to the Competition port if uploading
  over VEXnet.
* Terminal programs such as CoolTerm, PuTTY, or screen using the port must be closed before
  uploading. If a terminal is open from inside the PROS IDE, it will be automatically closed.
* VEX Driver support can be poor on Mac OS X and Linux. The newer VEX Programming Cable works
  well on all platforms, but the VEX Cortex direct connection to the PC only works on Windows.
  The older orange serial programming cables work on Windows and Linux but may have problems
  on Mac OS X.

@ref Top

@subsection Debugging Debugging and feedback

* Several functions, including @ref printf and @ref puts, will output data to the PC for
  debugging purposes. This data can be viewed with a standard serial port viewer such as
  [CoolTerm](http://freeware.the-meiers.org/) or [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/).
  It can also be viewed inside the PROS IDE:
* Select `Window > Show View > Terminal`; if `Terminal` is unavailable, select
  `Window > Show View > Other...` and select `Terminal` from the list.
  ![Opening a Terminal view](@ref terminal_open.png)
* Select the `Connect` button. If the terminal has been used before, it will connect
  immediately; otherwise, a prompt will appear.
  ![Opening a new Terminal connection](@ref terminal_connect.png)
* Change the `Baud Rate` in the pop up dialog to `115200`. The default choice for `Port` is
  usually correct on Windows. For Linux and Mac OS X, the most likely correct `Port` is the
  entry containing "usb" or "acm" in the name. If the suggested option fails, experiment to
  find which one works.
  ![Configuring to set Baud Rate to 115200](@ref terminal_config.png)
* Press the *space bar* if using VEXnet wireless!
* The terminal should be automatically closed upon uploading and re-opened when the upload is
  complete, so these steps only need to be taken once. The `Connect` and `Disconnect` icons
  can be used to manually open and close the connection.
* If the `Port` of the VEX connection changes, simply close the `Terminal` view and re-open a
  new one to connect to a different port.

@ref Top

@subsection Management Project management

* Multiple projects can be open at once. However, to avoid confusion, it may be advisable to
  close old or unnecessary projects to keep the workspace clean. To close a project and any
  files it has open, right-click the project in the `Project Explorer` and select `Close Project`.
  ![Closing an unnecessary PROS project](@ref close_projects.png)
* Projects closed in this way are not deleted. The project can be re-opened in the same state
  that it had when it was closed by right-clicking the project in the `Project Explorer` and
  selecting `Open Project`.
  ![Opening a PROS project](@ref open_projects.png)
* Projects can be deleted by selecting them in the `Project Explorer` and pressing the `DELETE`
  key. *This may not be the same as the `BACKSPACE` key on Macintosh machines*. Unless the
  `Delete project contents from disk` option is selected, the files will not actually be removed
  from the directory where the project is stored. Deleting projects should be *a last resort*
  unless one plans to immediately add them back through @ref Git. Old code can be *very* useful.

@ref Top

@subsection Updating Updating a project

Projects by default remain on the version of PROS that was used to create them. Therefore,
users of different versions of PROS will still run that project in the same way. However,
individual old projects can be updated to incorporate new PROS features using the
`VEX > Switch project to PROS ...`. Check the confirmation dialog to ensure that the correct
project is selected for upgrade!
![Update a project to another revision of PROS](@ref switch_version.png)

PROS also automatically checks for new versions. This check occurs on startup and can be
disabled by using `Window > Preferences...` (Windows/Linux) or `Eclipse > Preferences` (Mac),
then selecting `Startup and Shutdown` under the `General` tab and deselecting
`PROS Project Manager and Update`:
![Disabling the automatic update check](@ref disable_update_check.png)

@ref Top

@section Tutorial PROS Tutorial

@subsection C C Basics

PROS is a library built on C. This document does not aspire to teach beginners C. Excellent
tutorials are available on the Internet by searching for "C" in a browser, bringing up results
such as [this tutorial on C](http://www.cprogramming.com/tutorial/c-tutorial.html).

Note that not all information presented in C tutorials is relevant to programming VEX robots
with PROS. For example, pointers are used only occasionally, and there is little need for
topics such as recursion and file I/O.

@ref Top

@subsection Project PROS Project Structure

PROS projects are internally composed of three parts: the *user code*, the *PROS library* and
the *header files*.

The *PROS library* is a single file containing the core PROS routines. This file does not need
to be changed. If there appears to be an issue with a core PROS function, please file an issue
on the [PROS website](http://sourceforge.net/p/purdueros/tickets).

The *header files* are all found in the `include` directory. One header file, `API.h`, is
required to declare the PROS library functions, and also serves as a quick reference. The
other file, `main.h`, is intended for declaring functions and variables shared between the
*user code* files. New header files can be created in the `include` directory, as long as the
name ends with `.h`.
![Creating a new header file](@ref new_header.png)

*User code* has the actual sequential instructions that govern the robot's behavior. PROS
by default splits the user code up into autonomous (auto.c), driver control (opcontrol.c), and
initialization (init.c) files. Code in one file can talk to code in another file using
declarations in the *header files*. New user code files can be created in the `src` directory,
as long as the name ends with `.c` it will be compiled with the others.
![Creating a new source file](@ref new_source.png)

All user code files should start with

@code
#include "main.h"
@endcode

to make the critical declarations in API.h and main.h available in each file.

![PROS default project structure](@ref pros_defaultproject.png)

While more complicated than some environments, splitting up code grants powerful modularity
and code reusability, especially when combined with @ref Git.

@ref Top

@subsection Declaration Declaring Functions and Variables

Functions and variables must be *declared* before use, as described in many of the @ref C
tutorials. Functions and variables can be declared in the typical C manner:

@code
int sensorValue;

void DoIt(int x) {
    motorSet(5, x);
}

int getSensorValue(void) {
    DoIt(127);
    return sensorValue;
}
@endcode

Doing so will make the variables and functions local to the file in which they exist.

Some functions and variables are useful in more than one location (e.g. a function to use
@ref motorSet on all the robot's drive motors with the same value). Such functions can be
declared in any of the three files (*or a custom file in the `src` directory*). To make them
accessible to any other user code file, *prototype* the function in main.h.

Functions can be prototyped by copying the function's declaration line into main.h and replacing
the function code with `;`.

@code
// Prototype the "getSensorValue()" function declared above (in main.h)
int getSensorValue(void);

// Prototype the "DoIt()" function declared above (in main.h)
void DoIt(int x);
@endcode

Any function can be prototyped in main.h regardless of which user code file in which it appears.
Variables are a little more difficult; the word `extern` must be placed *in front* of the
declaration line when it is copied into main.h.

@code
// Prototype the "sensorValue" variable declared above (in main.h)
extern int sensorValue;
@endcode

For pieces of code intended for frequent reuse, it may be better to create a separate header
file with the related declarations, and to include that file in main.h. Therefore, large pieces
of code involving look-up tables, control algorithms, or other complex structures can be quickly
ported between projects by copying one `.c` user code file and one `.h` header file.
![Custom libraries with one header and one source file](@ref library_files.png)


@ref Top

@subsection Templates Examples and templates

@subsubsection Sensors Sensors

Most sensors follow a similar pattern. The sensor is initialized in @ref initialize and
used in multiple places, including @ref autonomous and @ref operatorControl. Typical sensor
initialization code thus looks like:

#### Analog Yaw-Rate Gyro

init.c:

@code
// Multiple gyros can be declared
Gyro gyro;

void initialize() {
    // ...
    // If gyro reads inaccurately, change "0" to desired sensitivity
    // See documentation on gyroInit for up-to-date sensitivity details
    gyro = gyroInit(gyro_port_number, 0);
    // ...
}
@endcode

main.h:

@code
// Variable name matches gyro name(s) in init.c
extern Gyro gyro;
@endcode

opcontrol.c or auto.c:

@code
void ...() {
    // ...
    // Get gyro reading in degrees
    int heading = gyroGet(gyro);
    // Reset gyro to zero
    gyroReset(gyro);
    // ...
}
@endcode

#### Quadrature Encoder

init.c:

@code
// Multiple encoders can be declared
Encoder encoder;

void initialize() {
    // ...
    encoder = encoderInit(top_port_number, bottom_port_number, is_reversed);
    // ...
}
@endcode

main.h:

@code
// Variable name matches encoder name(s) in init.c
extern Encoder encoder;
@endcode

opcontrol.c or auto.c:

@code
void ...() {
    // ...
    // Get encoder reading in degrees
    int counts = encoderGet(encoder);
    // Reset encoder to zero
    encoderReset(encoder);
    // ...
}
@endcode

@ref Top

#### Ultrasonic Sensor

init.c:

@code
// Multiple ultrasonics can be declared
Ultrasonic sonar;

void initialize() {
    // ...
    sonar = ultrasonicInit(orange_port_number, yellow_port_number);
    // ...
}
@endcode

main.h:

@code
// Variable name matches ultrasonic name(s) in init.c
extern Ultrasonic sonar;
@endcode

opcontrol.c or auto.c:

@code
void ...() {
    // ...
    // Get ultrasonic reading in centimeters
    int distance = ultrasonicGet(sonar);
    // ...
}
@endcode

@ref Top

#### Integrated Motor Encoders

init.c:

@code
void initialize() {
    // ...
    // Check count to ensure all IMEs are plugged in!
    int count = imeInitializeAll();
    // ...
}
@endcode

main.h:

@code
// It may be wise to use #define to establish friendly IME names
// Closest IME to the Cortex (first on chain) gets #0, IME connected to that one gets #1,
// and so on
#define IME_LEFT_MOTOR 0
#define IME_RIGHT_MOTOR 1
@endcode

opcontrol.c or auto.c:

@code
void ...() {
    // ...
    // Get IME tick count in the "counts" variable
	// (conversion to rotations varies depending on the motor type in use)
    int counts;
	imeGet(0, &counts);
    // Or if #define was used:
    imeGet(IME_LEFT_MOTOR, &counts);
    // Reset IME to zero
    imeReset(IME_RIGHT_MOTOR);
    // ...
}
@endcode

@ref Top

#### Buttons, Limit Switches

init.c:

@code
void initializeIO() {
    // One of the few things done in initializeIO()
    pinMode(switch_pin, INPUT);
}
@endcode

opcontrol.c or auto.c:

@code
void ...() {
    // ...
    // digitalRead() will return LOW if PRESSED and HIGH if RELEASED
    if (digitalRead(switch_pin) == LOW) {
        // ...
    }
    // ...
}
@endcode

@ref Top

@subsubsection Actuators Actuators

#### VEX Speaker

init.c:

@code
void initialize() {
	speakerInit();
}
@endcode

opcontrol.c or auto.c:

@code
void ...() {
    // ...
    // Play a tune on the speaker
    speakerPlayRtttl("Super Mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,"
		"8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,"
		"8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,"
		"8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,"
		"8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,8p.,"
		"8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,"
		"8p,16d#6,8p,16d6,8p,16c6");
	// Play a polyphonic tune on the speaker with 2 tracks
	const char * rtttl[] = {
		"Super Mario:d=4,o=5,b=100:"
		"16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,"
		"8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,"
		"8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,"
		"8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,"
		"8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,8p.,"
		"8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,"
		"8p,16d#6,8p,16d6,8p,16c6",

		"Super Mario:d=4,o=4,b=100:"
		"16d,16d,32p,8d,16d,8d,8g5,8p,8g,8p,"
		"8g5,16p,8e,16p,8c,16p,8f,8g,16f#,8f,16e.,16c5,16e5,8f5,16d5,8e5,8c5,16a,16b,8g,16p,"
		"8g5,16p,8e,16p,8c,16p,8f,8g,16f#,8f,16e.,16c5,16e5,8f5,16d5,8e5,8c5,16a,16b,8g,16p,"
		"8c,16p,16g,8p,8c5,8f,16p,16c5,16c5,16c5,8f,"
		"8c,16p,16e,8p,16g,16c5,p.,8g,"
		"8c,16p,16g,8p,8c5,8f,16p,16c5,16c5,16c5,8f,"
		"8c,16g#,8p,16a#,8p,16c5",

		NULL
	};
	speakerPlayArray(rtttl);
    // ...
}
@endcode

@ref Top

#### Pneumatic Solenoids

init.c:

@code
void initializeIO() {
    // One of the few things done in initializeIO()
    digitalWrite(solenoid_pin, LOW);
    pinMode(solenoid_pin, OUTPUT);
}
@endcode

opcontrol.c or auto.c:

@code
void ...() {
    // ...
    // Change solenoid to HIGH or LOW
    // The resulting pnematic motion varies depending on tube connections
    digitalWrite(solenoid_pin, new_value);
    // ...
}
@endcode

@ref Top

#### VEX Motors or Servos

opcontrol.c or auto.c:

@code
void ...() {
    // ...
    // Turn motor on at given power level
    // 0 is stop, -127 is full reverse, 127 is full forward
    // For a VEX servo, rotates to a position depending on value
    // 0 is centered, -127 is fully one way, 127 is fully the other way
    motorSet(motor_port, new_value);
    // ...
}
@endcode

@ref Top

#### VEX LCD

init.c:

@code
void initialize() {
    // ...
    // Select "uart1" or "uart2" as appropriate
    lcdInit(uart1);
    lcdClear(uart1);
    // ...
}
@endcode

opcontrol.c or auto.c:

@code
void ...() {
    // ...
    // Print formatted text to LCD (line #1) like printf
    // supposing that "x" is an existing integer variable
    lcdPrint(uart1, 1, "X is %d", x);
    // Print plain text to LCD (much faster than lcdPrint)
    lcdSetText(uart1, 2, "Hello World");
    // ...
}
@endcode

@ref Top

@subsubsection Tasks Tasks

Tasks are a great tool to do multiple things at once, but they can be difficult to use properly.
Each task has a *priority* and a *stack size*. Most of the time, tasks should have the
default stack size of `TASK_DEFAULT_STACK_SIZE`. The higher the priority, the more crucial the
task is considered; tasks of higher priority will *always* run in preference to lower priority
tasks, unless the higher priority task is using `delay()` or some other waiting action.

Tasks are created using taskCreate(), which invokes a user function in the new task:

@code
// This function will execute concurrently with other tasks on the system
// The parameter will receive the argument given in taskCreate()
void startTask(void *ignore) {
    // ...
}

// Creates task which will call the function "startTask()" with default priority and
// stack size; the argument will be "NULL"
taskCreate(startTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
@endcode

@ref Top

#### Best Task Practices

* Limit the number of tasks; five medium sized tasks will run quicker than ten tiny ones. There
  is also a limit of twelve tasks running at once.
* Tasks are usually not automatically stopped if the robot is disabled, unlike `operatorControl`
  and `autonomous`. Tasks running while disabled cannot use the VEX Joystick or VEX Motors.
  If the task should stop when the robot is disabled, use the @ref isOnline and/or the
  @ref isAutonomous function to control the program accordingly. If @ref taskRunLoop is used,
  the task will automatically be cancelled if the robot is disabled or switched between driver
  and autonomous.
* Most tasks should have a priority from `TASK_PRIORITY_LOWEST + 1` to
  `TASK_PRIORITY_HIGHEST - 1`. Tasks of the lowest or highest priority may cause unexpected
  behavior.
* Tasks should wait when there is no work to do so other tasks can run. If a task, especially a
  high-priority one, does not occasionally use @ref delay or similar, other tasks may not run
  or run very slowly. Since hardware such as the VEX LCD and Integrated Motor Encoders is also
  updated by tasks, a run away task can cause unexpected behavior.
* The Cortex Microcontroller is very fast. It is unlikely that any repetitive task will gain
  from running continuously. Even in the fastest scenarios, `delay(2)` will probably make no
  noticable difference, but will prevent future task starvation issues.
* Tasks using motors or joysticks should use `delay(20)` as the motors and joysticks are only
  updated every 20 ms.

@ref Top

#### Synchronization

One problem which often beguiles tasks is synchronization. If two tasks try to read the same
sensor or control the same motor at the same time, unexpected behavior may occur.

Tasks can be designed never to conflict over motors or sensors:

@code
void Task1(void *ignore) {
    // update motor 2 and 4
}
void Task2(void *ignore) {
    // update motor 5 and 6
}
@endcode

If this is impossible, PROS features two types of elements, *mutexes* and *semaphores*, that
can be used to coordinate tasks. Mutexes stand for *mutual exclusion*; only one task can hold
a mutex at any given time. Other tasks trying to use the mutex must wait for the first task
to finish. For this reason, mutexes must always be released after use.

@code
// Create a mutex
Mutex mutex = mutexCreate();
// Acquire the mutex; other tasks using this command will wait until mutex is released
// timeout can specify the maximum time to wait, or MAX_DELAY to wait forever
// If the timeout expires, "false" will be returned, otherwise "true"
mutexTake(mutex, timeout);
// Release the mutex for other tasks
mutexGive(mutex);
@endcode

Semaphores are like signals - one task can take the semaphore to wait for a coordination signal
from another task which gives it. If multiple tasks wait on the same semaphore, the highest
priority one will continue per signal given.

@code
// Create a semaphore
// Warning: semaphore starts available, so the first call to semaphoreTake will return
// immediately
Semaphore semaphore = semaphoreCreate();
// Waits for semaphore to be signalled
// timeout can specify the maximum time to wait, or MAX_DELAY to wait forever
// If the timeout expires, "false" will be returned, otherwise "true"
semaphoreTake(semaphore, timeout);
// Signals the semaphore
semaphoreGive(semaphore);
@endcode

@ref Top

@subsection Common Common tasks

### Typical operator control loop

@code
void operatorControl() {
    while (1) {
        // Read joystick values
        int joystick1 = joystickGetAnalog(1, 3);
        int joystick2 = joystickGetAnalog(1, 4);
        // Optionally process joystick values
        // Send joystick outputs to motors
        motorSet(2, joystick1);
        motorSet(9, joystick2);
        // Joystick and motor values refreshed every 20 ms
        delay(20);
    }
}
@endcode

### Run a particular function in a loop in a new task

@code
// delta_time is in milliseconds, 1000 milliseconds per second
// Function calls will stop if robot is disabled or switched into another mode
taskRunLoop(functionName, delta_time);
@endcode

### Test autonomous without a competition switch

@code
void operatorControl() {
    // Plug a limit switch or button into digital port 1
    // If this button is being held down when the robot is turned on, autonomous will
    // be run before switching to driver control
    if (digitalRead(1) == LOW)
        autonomous();
    while (1) {
        // ...
    }
}
@endcode

@ref Top

@subsection FAQ PROS Coding FAQ

@subsubsection CompileErr Build-time Problems

* `undefined reference to ...` or `implicit declaration of function ...`: A function name
  is not spelled correctly, or the function was not correctly [declared](@ref Declaration)
  in a header file. Custom headers must be included in `main.h` or in the file in which they
  are used; declarations do not appear like magic.
* `format ... expects argument of type ..., but argument has type ...`: The value provided
  to a function like printf() or lcdPrint() does not match the expected type inferred from the
  format string. Some instances of this warning can be ignored, but crashes can occur if
  `double` or `long long` is mixed with another type.
* `assignment makes pointer from integer without a cast`: Typically caused by a C pointer
  having the wrong number of asterisks to dereference it, or when accidentally assigning a
  constant to `pointer` (instead of to `*pointer`).

@ref Top

@subsubsection RuntimeErr Run-time Problems

* __VEX LCD updates very slowly or is "frozen"__: A task is not waiting using `delay()`. The LCD
  usually is less important than how well the robot is running, so PROS prioritizes user tasks
  over the LCD. Only if all other tasks are waiting is the LCD updated.
* __Some tasks are running, others are not__: A task is not waiting using `delay()`. Higher
  or equal priority tasks will still run, but lower priority tasks will not.
* __Neither autonomous nor driver control code starts__: The `initialize()` function may be
  still running. Some tasks such as @ref gyroInit or @ref analogCalibrate take time. If the
  `initialize()` function implements some type of loop or autonomous selection routine, verify
  that it actually has a means of ending.
* __Code restarts unexpectedly__: A run-time error has crashed the program. Using a terminal
  application (see [debugging](@ref Debugging)) may reveal the cause of the error. Examine any
  newly added code for possible problems that may occur at run time.
  * `Segmentation Fault` indicates that an invalid C pointer has been used. Check for
    confusion between pointers and regular variables, or that an invalid pointer has not been
    passed to a PROS API function.
  * `Stack Overflow` often indicates infinite recursion, or that the stack size for a custom
    task is too small. Calling layers of functions and declaring large local variables can
    require large amounts of space on the stack. If this error occurs in a default task like
    autonomous(), consider changing code to reduce the stack requirements, or creating a new
    task with a larger stack using taskCreate() to handle large jobs. Large arrays declared
    inside functions can be declared globally to alleviate pressure on stack space.
  * `System Task Failure` means that too many tasks were running for the system to start a new
    one. Disable or merge unnecessary tasks to eliminate this error.
* __Red blinking ROBOT light after download__: Turn the Cortex Microcontroller off and on again.
  This usually resolves the problem, and is good practice to re-initialize the robot in a
  manner similar to what happens at most competitions. If the error persists, see "Code restarts
  unexpectedly".
* __printf() is not working!__: @ref printf prints to the PC over the debug terminal. See the
  section on [debugging](@ref Debugging). @ref printf will not print to the VEX LCD; use @ref lcdPrint
  instead. If VEXnet wireless is in use, press the space bar in the debug terminal to reveal
  the output.

@ref Top

@section Advanced Advanced topics

@subsection Git Source control

@subsubsection GitSetup Initial setup

* PROS makes it easy to write *a lot* of code. Collaborating with other team members may be
  difficult. Plus, it might be handy to go back and see what the code looked like a week ago.
  *Source control* solves these problems. A great tutorial on Git source control can be found
  at [the GIT website](http://git-scm.com/docs/gittutorial), but here's the express tutorial to
  get a repository up and running quick.
* Select `File > New > Other...` and select `Git Repository` from the dialog.
  ![Create a Git Repository](@ref git_newrepo.png)
* Enter a name and a folder to store the repository. The workspace should probably be used
  as the parent directory; enter a short but descriptive name such as `2013Code` as the name.
  Multiple projects can (and should) be stored in the same Git repository. Do *not* check
  `Create as bare repository`.
* Press `Finish`. Nothing appears to happen, but some useful files have been created.

@ref Top

@subsubsection GitExternal Use with an online service

* Teams which collaborate using an online service such as [BitBucket](http://www.bitbucket.org)
  or [GitHub](https://github.com/) can integrate code sharing with PROS!
* Set up a public/private key pair if required by the website. The private key file "id_rsa"
  will need to be placed in the ".ssh" folder in the user's home directory.
* Find the "clone URL" location on the website that allows read-write access:
  ![Find URL to clone project](@ref git_github.png)
* Select `File > Import...` in the PROS IDE. In the pop-up dialog, select `Projects from Git`.
  ![Importing an existing project into the IDE](@ref importitems.png)
  ![Importing projects from Git](@ref git_importselect.png)
* Click `URI` and enter the clone URL from the website:
  ![Settings for remote project import](@ref git_importremote.png)
* Click `Yes` if a warning appears about "host key cannot be verified" and enter the public key
  passphrase or user password if required.
* Select `Import existing projects` in the next dialog box and navigate to the project
  directory. Click `Finish` and the project will appear in the `Project Explorer`.
  ![Selecting the project for Git import](@ref git_importproject.png)

@ref Top

@subsubsection GitAdd Adding a project to source control

* Find the project to be added to source control. If it does not exist yet, create it!
* Delete the project from the `Project Explorer`. Do *not* delete the contents from disk. Don't
  worry, it will reappear momentarily!
* Use the operating system to copy the project into the Git repository folder which was just
  created.
  ![Move project into the Git repository next to the '.git' folder](@ref git_pasteproject.png)
* Select `File > Import...` in the PROS IDE. In the pop-up dialog, select `Projects from Git`.
  ![Importing an existing project into the IDE](@ref importitems.png)
  ![Importing projects from Git](@ref git_importselect.png)
* Click `Local` and then the repository previously created.
* Select `Import existing projects` in the next dialog box, then select the folder that was
  just copied into the repository. If the project does not appear, verify that the project was
  copied into the correct location. It should *not* be inside the `.git` directory. Click
  `Finish` and the project will re-appear in the `Project Explorer`.
  ![Selecting the project for Git import](@ref git_importproject.png)
* Add the project to source control by right-clicking on the project in the `Project Explorer`
  and selecting `Team > Add to Index...` in the menu that appears.
  ![Adding the project to the index](@ref git_addtoindex.png)
* Make an initial commit, as described in the next section.

@ref Top

@subsubsection GitCommit Committing changes

* *Committing* changes stores the current state of all PROS project files in the repository.
  This marks a point in time where a message can be stored explaining the author and the state
  of the code. It also marks a point to which all or part of the project can be rolled back
  in case of emergency.
* Commit the project by right-clicking it again and selecting `Team > Commit...` or pressing
  the `CTRL + #` (`CTRL + SHIFT + 3` on most keyboards) keyboard shortcut.
  ![The Commit option in the Team menu](@ref git_commit.png)
* Enter a message for the commit. If the ``Author`` and ``Committer`` fields are blank, fill
  them in with the user's name and e-mail, tremendously helpful for identifying the person to
  credit (or blame) with new code changes. *Make sure to click the check mark button* near
  the list of files between the question mark and the square!
  ![Enter the commit message and name in the Commit dialog and click the check mark!](@ref git_commitmsg.png)
* Press `Commit` and the code changes will be stored away. If a file is changed in the project,
  the icon in the `Project Explorer` will gain a `>` indicating that it was modified.
  ![Identifying changed files from last commit](@ref git_ischanged.png)


@ref Top

@subsubsection GitUsage Using source control

* To see *exactly* what was changed since the last commit (or any past commit, for that matter),
  select the file in question (*or select the project to see changes to all files*),
  right-click, and select `Compare With > Commit...`.
  ![Comparing a file with a Git commit](@ref git_compareto.png)
* Select the desired point in time for comparison, and a beautiful dialog pops up showing
  everything that was changed during that time (as well as a copy of the file as it appeared
  back then).
  ![Selecting a target commit for comparison/replacement](@ref git_selectcommit.png)
  ![Results of comparison showing exactly what was changed](@ref git_comparison.png)
* Rolling back changes can be as simple as selecting a few lines, copying from the old, and
  pasting in the current version. For quick, wholesale rollback of an entire file, select
  `Replace With > Commit...` and select the commit to which to roll the file back. *This will
  wipe out any changes since the last commit.* It might be better to commit the current code
  and then roll back the changes.
  ![Replacing the file with an old version](@ref git_destroychanges.png)
* Source control can be tough. But using it right can save many hours of rework.

@ref Top

@section OutsideIDE Using PROS without the PROS IDE

@subsection CmdLine Using PROS on the command line

PROS can be used with just command line utilities, in Windows, Linux, and Mac OS X. *Make sure
to install the PROS IDE before trying*, as important tools such as the ARM cross compiler are
installed on the system path when installing the base PROS IDE. Any text editor can be used to
edit source code and header files.

* Compile a project: `make`
* Clean a project: `make clean`
* Upload a project to a VEX Cortex: `make upload` *Java must be installed*

![Using the make utility to compile PROS projects on the command line](@ref pros_cmdline.png)

@ref Top

@subsection VisualStudio Using PROS with Visual Studio

A copy of Visual Studio and Windows is required. Visual Studio 2010 was used for this document,
but the same or similar directions should apply to Visual Studio 2008 or 2013.

* Open the PROS IDE. [Create a blank PROS Project](@ref Creating) to extract the project
  template, then exit the PROS IDE.
* Start Visual Studio. Create a new `Makefile Project` in the `Visual C++` category in the
  desired directory.
  ![Creating a blank Makefile project in Visual Studio](@ref pros_vs2010_create.png)
* Click `Finish` in the wizard to create a blank project.
* Copy and paste the contents of the blank PROS project created in step one into the project
  directory. *This is inside a subfolder of the directory created in the last two steps*.
  ![Copying and pasting the project to the Visual Studio project folder](@ref pros_vs2010_copy.png)
* Add the `.h` files in `include` to the `Header Files` section, and the `.c` and `.cpp` files
  in `src` to the `Source Files` section, using `Add > Existing Item...`:
  ![Adding files to the source and include directories in Visual Studio](@ref pros_vs2010_add.png)
* In `Project > Properties...` add the include directory to the `Include Directories` in the
  `VC++ Directories` panel:
  ![Adding the include folder to the Visual Studio search path](@ref pros_vs2010_config3.png)
* In the `NMake` panel, add references to `make` and `make clean` where prompted:
  ![Configuring the Visual Studio make utility for PROS](@ref pros_vs2010_config1.png)
* Create a file named `prosload.cmd` in a convenient location (preferably where it will not be
  accidentally moved or committed to @ref Git) containing the following text: <pre>
\@echo off
cd /d %~dps1
make -k upload
</pre>
* Use `Tools > External Tools` to create a tool named "PROS Upload" with the following settings:
  ![Configuration settings for PROS Upload tool](@ref pros_vs2010_config2.png)
* Another external tool can optionally be configured to update PROS projects. The batch file
  must contain (*where path-to-PROS-IDE-installation is the directory selected when installing,
  usually* `C:\Program Files\PROS`): <pre>
\@echo off
cd /d %~dps1
PROS=path-to-PROS-IDE-installation
for \%\%f in (\%PROS\%\\plugins\\com.purduesigbots.newcortexproject_*.jar) do java -jar \%\%f .
</pre>The VS settings for the external tool are the same as the previous example (with the
  command name changed).
* PROS setup with Visual Studio is complete. The standard VS `Build` functionality will compile
  the project; the `PROS Upload` option in the `Tools` menu will upload the project to an
  attached VEX Cortex. *Not all warning and error messages will be the same as in the
  Eclipse-based PROS IDE*.

@ref Top
