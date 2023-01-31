# PROS Kernel Training Branch - IMU Modulo
[![Build Status](https://dev.azure.com/purdue-acm-sigbots/Kernel/_apis/build/status/purduesigbots.pros?branchName=develop)](https://dev.azure.com/purdue-acm-sigbots/Kernel/_build/latest?definitionId=5&branchName=develop)

## This is a branch for new members to our team
It is intended for training purposes only, not for development. 

## Problem Description
The code for the IMU device currently uses if statements to keep the returned headings in a range from 0 to IMU_HEADING_MAX (inclusive). This leads to longer code, and can be slower due to the overheads of branching.

Your task is to switch the code to use the [`fmod()`](https://en.cppreference.com/w/c/numeric/math/fmod) function instead of the if statements. This function is likely to be more optimized than our current solution and makes our code cleaner. 

There are six functions in the IMU's code that need changed:
```cpp
double imu_get_heading(uint8_t port)
quaternion_s_t imu_get_quaternion(uint8_t port)
euler_s_t imu_get_euler(uint8_t port) 

double imu_get_pitch(uint8_t port)
double imu_get_roll(uint8_t port)
double imu_get_yaw(uint8_t port)
```

## Building and Testing The Kernel
1) Push your code to Github under a branch titled `training/modulo/<your name>`
2) On Github an orange circle will appear to indicate that the CI is building
3) When the orange circle turns into a red x or green checkmark:
   1) Click the symbol
   2) Click "Details"
   3) Click "# errors / # warnigns"
4) Click "BuildTemplate"
5) If the build failed, the right terminal will show an error/warning log. Otherwise, it will say: `1 artifact Produced`
   1) Click that text
6) Click the 3 dot menu on the right and click "Download Artifacts". This will download a zip file.
7) Inside the zip file is a folder called "template", and inside that folder is another zip file with the name `kernel@<version>.zip`, where `<version>` is the version you just built. Copy this somewhere.
8) You need to fetch the Kernel template before you can add it to a project:
   1) In the PROS integrated terminal, navigate to the folder you downloaded the zip
   2) Type `pros c fetch kernel@<version>.zip`
9) To apply it to your project:
   1)  In the PROS integrated terminal, navigate to your pros project
   2)  Type `pros c apply --force-apply kernel@<version>`. Notice that there is no `.zip` at the end of the file this time
10) The project should now have the Kernel you just built, and you can test it. 

## Problem Solution
The solution to this training excercise can be found in [solution.c](solution.c)

Please implement your solution before looking at this one