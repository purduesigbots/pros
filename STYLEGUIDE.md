# PROS Kernel Styleguide

Maintaining a consistent coding style throughout the PROS kernel is important for future developers and students to learn about the PROS kernel. An inconsistent style can lead to confusion for everyone.

The PROS style is based on the Google C++ coding style (with some modifications noted a bit farther down), which allows `clang-format` to be used on every file and get correctly formatted code.

Some additional notes follow:

## File extensions
- C source files will have the extension `.c`
- C++ source files will have the extension `.cpp`
- C header files will have the extension `.h`
- C++ header files will have the extension `.hpp`

## Naming Conventions
The PROS kernel follows these naming conventions:

- Structures: `structname_s`:
  - Structure members: these do not have any special convention, as you will never see a structure member without seeing that it is in a structure.
- Enumerated types: `enumname_e`
  - Enumerated type members: `E_ENUMNAME_MEMBERNAME`

    For example, in `task_state_e`, you might have members such as `E_TASK_STATE_RUNNING`. Note that "ENUMNAME" can be multiple words, separated by underscores.
- Function pointers: `funcname_fn`
- Type definitions: append `_t` to the end of the structure/enum/function pointer name
- C++ classes: these should be named in UpperCamelCase (also known as PascalCase)
  - Class members: use lower_snake_case as normal, and use good sense when naming

This section provides only a brief overview of the naming conventions used in the PROS kernel. For more about the motivations behind these, see section 4 (Naming) below.

## Documentation
Use Doxygen-style comments as shown in the sections below.

### File-Level Comments
These should be placed at the very start of a file.
```c
/**
 * \file filename.h
 *
 * Short description of the file
 *
 * Extended description goes here. This should explain what the functions (etc)
 * in the file contains and a general description of what they do (no specifics,
 * but they should all have something in common anyway).
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright Copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
```

### Enum-Level Comments
These should be placed immediately before the declaration of the enum.
```c
/*
 * Short description of the enum
 *
 * Extended description of the enum goes here. This should explain general usage
 * patterns for the enum.
 */
enum my_enum {
        E_MEMBER_0, // short description of member 0 goes here.
        E_MEMBER_1, // these can be omitted if it's painfully obvious
        E_MEMBER_2, // what each is for, or if there are just so many of
        E_MEMBER_3, // them it doesn't make practical sense to
        E_MEMBER_4  // document them all.
}
```
_Note: in the above example, the comments describing each member of the `enum` run together and form complete sentences for effect. Please do not do this in your code!_

### Function-Level Comments
These should be placed immediately before the function prototype they are describing in a header file.

```c
/**
 * Brief description of the function.
 *
 * An extended description of the function (if applicable).
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ERRNO_VALUE - Description of what causes this error
 *
 * \param parameter_name
 *        The parameter description
 * \param other_parameter_name
 *        The parameter description
 *
 * \return The description of the return value, if this is longer than one line
 * then it will wrap around under the return statement
 */
```

### Inline Implementation Comments
Sometimes it is necessary to explain a particularly complex statement or series of statements. In this case, you should use inline comments, placed either immediately before or trailing the line or lines in question. In general, prefer placing such comments before offending lines, unless the comment is quite short. These comments should start with a `//` followed by a space. If they are placed trailing a line, they should be separated from the end of the line by one space.
```c
float Q_rsqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
    x2 = number * 0.5F;
    y  = number;
    // perform some absolute magic on these numbers to get the inverse square root
    i  = *(long*)&y; // evil floating point bit level hacking
    i  = 0x5f3759df - (i >> 1); // what the [heck]?
    y  = *(float*)&i;
    y  = y * (threehalfs - (x2 * y * y)); // 1st iteration
    //y  = y * (threehalfs - (x2 * y * y)); // 2nd iteration, this can be removed
    return y;
}
```
_Note: in the above example, there is a line of code that has been commented out. This is fine to do while testing, but any commented out lines of code should be removed before any merge into the master branch takes place, unless a compelling reason can be presented for them to remain._

All that being said, try to avoid code that is so complex that it requires inline comments for its purpose to be clear.

#### Notes to Other Developers (Or Yourself)
When writing code, it can sometimes be useful to leave notes to other developers or to yourself in the future. Examples of these include:

- `// TODO: something that should be done`
- `// NOTE: a note about something in the code`
  - `// NB: this is the same as NOTE, but it's in Latin, so it's fancier`
- `// HACK: used to describe a particularly nasty way of solving a problem-- could be improved, but it works for now`
- `// FIXME: this code is broken and should be fixed`
  - `// XXX: this is like FIXME, but it's worse (note that this has also been used in the same way as HACK, so you should use that or FIXME to clarify what is meant)`
  - `// BUG: this is used to mark a line of code that is known to cause a bug. kind of like FIXME, though it may include some more specific information than FIXME would`

While it is not strictly necessary to use these keywords in comments, they can be helpful-- modern editors (like Atom or VSCode) either highlight some of these keywords by default or have extensions that do. This can make certain comments stand out even more when developers are "grepping" the codebase (visually or otherwise).

### Word Choice and Tense

Content for the docs should follow the
[Google Developer Documentation Style Guide](https://developers.google.com/style/),
and the [API Reference Comments Section](https://developers.google.com/style/api-reference-comments)
in particular.

While the Google Style Guide should determine the vast majority of the docs' word choices,
additionally follow these content-specific guides:

- All ports, Smart, ADI, or otherwise, should be referred to as a port, not a channel
  or pin.
- C++ functions should be written as their full function name, without removing the
  `pros::` namespace (e.g. `pros::Motor::move`).
- All enumerated values referenced in the code comments should be written as their
  full name per the PROS Style Guide, not the `PROS_USE_SIMPLE_NAMES` version.
  (e.g. `E_MOTOR_GEARSET_18`, not `E_MOTOR_GEARSET_18`).

### C/C++ Format Guide

The PROS Coding style is based on the [Google C++ Coding Style](https://google.github.io/styleguide/cppguide.html) with the following modifications:

```
TabWidth: 2
UseTab: ForIndentation
ColumnLimit: 120
DerivePointerAlignment: false
PointerAlignment: Left
FixNamespaceComments: true
ReflowComments: true
SortIncludes: true
AccessModifierOffset: 0
AllowShortBlocksOnASingleLine: false
AllowShortCaseLabelsOnASingleLine: false
AllowShortFunctionsOnASingleLine: Empty
```
