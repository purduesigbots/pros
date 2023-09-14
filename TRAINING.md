Task 1. 
A user on the most recent PROS 4 Beta release has found that their Motor stops responding after trying to get basic telemetry from their robot when they are using motor groups.

Your task: 

1. Create a new PROS 4 project using the CLI (use the --beta flag when creating a project)
2. Refrence the PROS 4 docs page to find likely methods that they have been using. <https://purduesigbots.github.io/pros-doxygen-docs/>
3. Try to get the motor to stop responding. 
4. make a branch off of THIS BRANCH (training-task1) on the kernel repo with the naming scheme 'YOUR_NAME/training1'
5. Attempt to fix the bug by edit the relvant kernel .c/.cpp files.
6. Write a very brief summary in this doc (on your branch) on what the bug was, and how you solved it (2-3 sentances MAX) And push your code. 
7. If you can, Test your code. Not many of you have NDAs yet, so Feel free to ask me to upload your branch onto a brain to see if it works.

A few quick notes, Please try to not use a controller in your testing code to find the bug (due to how many we have availible to use).

If you want a starting point, for the testing program, here is some starter code that should replace the opcontrol method in main.cpp of your new project. (Make sure you add the ports that you are using for the motors.)

```cpp
void opcontrol() {
	pros::MotorGroup motors({});
	while (true) {
    	
    	motors.move(50);
		//Telemetry to the terminal using printf goes here. 
    	pros::delay(1000);
    	motors.move(-50);
		pros::delay(1000);
		//IF you see the motor Switching directions, You have not encountered the bug. 
  }
}
```
