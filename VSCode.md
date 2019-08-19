Use the following include paths for intellisense completion:

"${env:PROS_TOOLCHAIN}/arm-none-eabi/include",
"${env:PROS_TOOLCHAIN}/lib/gcc/arm-none-eabi/6.3.1/include",
"${workspaceRoot}",
"${workspaceRoot}/include"

Add "_INTELLISENSE" to your defines to fix certain intellisense errors that will actually compile without issue