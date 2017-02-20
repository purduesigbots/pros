# PROS Kernel Styleguide

Maintaining a consistent coding style throughout the PROS kernel is important for future developers and students to learn about the PROS kernel. An inconsistent style can lead to confusion for everyone.

The PROS style is based off of the Linux coding style (with some modifications noted a bit farther down), which allows `clang-format` to be used on every file and get correctly formatted code.

Some additional notes follow:

## File extensions
C source files will have the extension `.c`
C++ source files will have the extension `.cpp`
C header files will have the extension `.h`
C++ header files will have the extension `.hpp`

## File comments
```c
/*****************************************************************************
 * kernel.c - PROS kernel and I/O initialization
 *
 * This file contains functions to help startup PROS
 * Also contained within this file are some cstdlib overloads to make the
 * kernel restart whenever there's a fault
 *
 * Copyright (c) 2017, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *****************************************************************************/
```

## Linux Coding Style modifications

- K&R are merely right (not **right**): functions should be defined by:
```c
int foo() {
  bar();
}
```

- Typedefs may also be used to reduce `struct my_struct` to `my_struct_t` and `enum my_enum` to `my_enum_t`. Neither significantly remove information conveyed by the type name. If you're not sure if it's struct or enum by inference, then either rename the type or it doesn't matter.

- Pointers go with the type. `char* str` not `char *str`. I probably want to know that it's a pointer when looking at the type, not what it's pointing to and only maybe find out it's a pointer.

The [Linux Coding style](https://github.com/torvalds/linux/blob/master/Documentation/process/coding-style.rst) is reproduced below with our modifications:

## PROS coding style

This is a short document describing the preferred coding style for the linux kernel. Coding style is very personal, and I won't force my views on anybody, but this is what goes for anything that I have to be able to maintain, and I'd prefer it for most other things too. Please at least consider the points made here.

First off, I'd suggest printing out a copy of the GNU coding standards, and NOT read it. Burn them, it's a great symbolic gesture.

Anyway, here goes:

## 1) Indentation
Tabs are 8 characters, and thus indentations are also 8 characters. There are heretic movements that try to make indentations 4 (or even 2!) characters deep, and that is akin to trying to define the value of PI to be 3.

Rationale: The whole idea behind indentation is to clearly define where a block of control starts and ends. Especially when you've been looking at your screen for 20 straight hours, you'll find it a lot easier to see how the indentation works if you have large indentations.

Now, some people will claim that having 8-character indentations makes the code move too far to the right, and makes it hard to read on a 80-character terminal screen. The answer to that is that if you need more than 3 levels of indentation, you're screwed anyway, and should fix your program.

In short, 8-char indents make things easier to read, and have the added benefit of warning you when you're nesting your functions too deep. Heed that warning.

The preferred way to ease multiple indentation levels in a switch statement is to align the switch and its subordinate case labels in the same column instead of double-indenting the case labels. E.g.:

```c
switch (suffix) {
case 'G':
case 'g':
        mem <<= 30;
        break;
case 'M':
case 'm':
        mem <<= 20;
        break;
case 'K':
case 'k':
        mem <<= 10;
        // fall through
default:
        break;
}
```

Don't put multiple statements on a single line unless you have something to hide:

```c
if (condition) do_this;
  do_something_everytime;
```

Don't put multiple assignments on a single line either. Kernel coding style is super simple. Avoid tricky expressions.

Outside of comments, documentation and except in Kconfig, spaces are never used for indentation, and the above example is deliberately broken.

Get a decent editor and don't leave whitespace at the end of lines.

## 2) Breaking long lines and strings
Coding style is all about readability and maintainability using commonly available tools.

The limit on the length of lines is 80 columns and this is a strongly preferred limit.

Statements longer than 80 columns will be broken into sensible chunks, unless exceeding 80 columns significantly increases readability and does not hide information. Descendants are always substantially shorter than the parent and are placed substantially to the right. The same applies to function headers with a long argument list. However, never break user-visible strings such as printk messages, because that breaks the ability to grep for them.

## 3) Placing Braces and Spaces

The other issue that always comes up in C styling is the placement of braces. Unlike the indent size, there are few technical reasons to choose one placement strategy over the other, but the preferred way, as shown to us by the prophets Kernighan and Ritchie, is to put the opening brace last on the line, and put the closing brace first, thusly:
```c
if (x is true) {
        we do y
}
```
This applies to all non-function statement blocks (if, switch, for, while, do). E.g.:
```c
switch (action) {
case KOBJ_ADD:
        return "add";
case KOBJ_REMOVE:
        return "remove";
case KOBJ_CHANGE:
        return "change";
default:
        return NULL;
}
```
Functions also follow this standard:
```c
int function(int x) {
  body of function
}
```

Note that the closing brace is empty on a line of its own, except in the cases where it is followed by a continuation of the same statement, ie a while in a do-statement or an else in an if-statement, like this:
```c
do {
        body of do-loop
} while (condition);
```
and
```c
if (x == y) {
        ..
} else if (x > y) {
        ...
} else {
        ....
}
```
Also, note that this brace-placement also minimizes the number of empty (or almost empty) lines, without any loss of readability. Thus, as the supply of new-lines on your screen is not a renewable resource (think 25-line terminal screens here), you have more empty lines to put comments on.

Do not unnecessarily use braces where a single statement will do.
```c
if (condition)
        action();
```
and
```c
if (condition)
        do_this();
else
        do_that();
```
This does not apply if only one branch of a conditional statement is a single statement; in the latter case use braces in both branches:
```c
if (condition) {
        do_this();
        do_that();
} else {
        otherwise();
}
```

### 3.1) Spaces
Linux kernel style for use of spaces depends (mostly) on function-versus-keyword usage. Use a space after (most) keywords. The notable exceptions are sizeof, typeof, alignof, and __attribute__, which look somewhat like functions (and are usually used with parentheses in PROS, although they are not required in the language, as in: sizeof info after struct fileinfo info; is declared).

So use a space after these keywords:
```
if, switch, case, for, do, while
```
but not with `sizeof`, `typeof`, `alignof`, or `__attribute__`. E.g.,

```
s = sizeof(struct file);
```
Do not add spaces around (inside) parenthesized expressions. This example is bad:
```
s = sizeof( struct file );
```
When declaring pointer data or a function that returns a pointer type, the preferred use of * is adjacent to the type name and not adjacent to the data name or function name. Examples:
```c
char* linux_banner;
unsigned long long memparse(char* ptr, char** retptr);
char* match_strdup(substring_t* s);
```
Use one space around (on each side of) most binary and ternary operators, such as any of these:
```
=  +  -  <  >  *  /  %  |  &  ^  <=  >=  ==  !=  ?  :
```
but no space after unary operators:
```
&  *  +  -  ~  !  sizeof  typeof  alignof  __attribute__  defined
```
no space before the postfix increment & decrement unary operators:
```
++  --
```
no space after the prefix increment & decrement unary operators:
```
++  --
```
and no space around the `.` and `->` structure member operators.

Do not leave trailing whitespace at the ends of lines. Some editors with smart indentation will insert whitespace at the beginning of new lines as appropriate, so you can start typing the next line of code right away. However, some such editors do not remove the whitespace if you end up not putting a line of code there, such as if you leave a blank line. As a result, you end up with lines containing trailing whitespace.

Git will warn you about patches that introduce trailing whitespace, and can optionally strip the trailing whitespace for you; however, if applying a series of patches, this may make later patches in the series fail by changing their context lines.

## 4) Naming
C is a Spartan language, and so should your naming be. Unlike Modula-2 and Pascal programmers, C programmers do not use cute names like ThisVariableIsATemporaryCounter. A C programmer would call that variable tmp, which is much easier to write, and not the least more difficult to understand.

HOWEVER, while mixed-case names are frowned upon, descriptive names for global variables are a must. To call a global function foo is a shooting offense.

GLOBAL variables (to be used only if you really need them) need to have descriptive names, as do global functions. If you have a function that counts the number of active users, you should call that count_active_users() or similar, you should not call it cntusr().

Encoding the type of a function into the name (so-called Hungarian notation) is brain damaged - the compiler knows the types anyway and can check those, and it only confuses the programmer. No wonder MicroSoft makes buggy programs.

LOCAL variable names should be short, and to the point. If you have some random integer loop counter, it should probably be called i. Calling it loop_counter is non-productive, if there is no chance of it being mis-understood. Similarly, tmp can be just about any type of variable that is used to hold a temporary value.

If you are afraid to mix up your local variable names, you have another problem, which is called the function-growth-hormone-imbalance syndrome. See chapter 6 (Functions).

## 5) Typedefs

Don't use typedefs to obscure the meaning of type.

When you see a
```c
vps_t a;
```
in the source, what does it mean? In contrast, if it says
```c
struct virtual_container* a;
```
or
```c
virtual_container_t* a;
```
you can actually tell what a is.

Use typedefs when:

1. totally opaque objects (where the typedef is actively used to **hide** what the object is).

Example: `pte_t` etc. opaque objects that you can only access using the proper accessor functions.

2. Clear integer types, where the abstraction helps avoid confusion whether it is `int` or `long`.

If something is `unsigned long`, then there's no reason to do
```c
typedef unsigned long myflags_t;
```
but if there is a clear reason for why it under certain circumstances might be an unsigned int and under other configurations might be unsigned long, then by all means go ahead and use a typedef.

3. Alias away `struct` or `enum` (but no more than this). If you're doing a bunch of casting, seeing
```c
struct my_data data = (struct my_data*)malloc(sizeof(struct my_data));
```
is less readable than
```c
my_data_t data = (my_data_t*)malloc(sizeof(my_data_t));
```

The intention is still clear, and does not hide any pointers.

## 6) Functions

Functions should be short and sweet, and do just one thing. They should fit on one or two screenfuls of text (the ISO/ANSI screen size is 80x24, as we all know), and do one thing and do that well.

The maximum length of a function is inversely proportional to the complexity and indentation level of that function. So, if you have a conceptually simple function that is just one long (but simple) case-statement, where you have to do lots of small things for a lot of different cases, it's OK to have a longer function.

However, if you have a complex function, and you suspect that a less-than-gifted first-year high-school student might not even understand what the function is all about, you should adhere to the maximum limits all the more closely. Use helper functions with descriptive names (you can ask the compiler to in-line them if you think it's performance-critical, and it will probably do a better job of it than you would have done).

Another measure of the function is the number of local variables. They shouldn't exceed 5-10, or you're doing something wrong. Re-think the function, and split it into smaller pieces. A human brain can generally easily keep track of about 7 different things, anything more and it gets confused. You know you're brilliant, but maybe you'd like to understand what you did 2 weeks from now.

In source files, separate functions with one blank line. In function prototypes, include parameter names with their data types. Although this is not required by the C language, it is preferred in Linux because it is a simple way to add valuable information for the reader.

## 7) Centralized exiting of functions
Albeit deprecated by some people, the equivalent of the goto statement is used frequently by compilers in form of the unconditional jump instruction.

The goto statement comes in handy when a function exits from multiple locations and some common work such as cleanup has to be done. If there is no cleanup needed then just return directly.

Choose label names which say what the goto does or why the goto exists. An example of a good name could be out_free_buffer: if the goto frees buffer. Avoid using GW-BASIC names like err1: and err2:, as you would have to renumber them if you ever add or remove exit paths, and they make correctness difficult to verify anyway.

The rationale for using gotos is:
- unconditional statements are easier to understand and follow
- nesting is reduced
- errors by not updating individual exit points when making modifications are prevented
- saves the compiler work to optimize redundant code away ;)

```c
int fun(int a)
{
        int result = 0;
        char* buffer;

        buffer = malloc(SIZE, GFP_KERNEL);
        if (!buffer)
                return -ENOMEM;

        if (condition1) {
                while (loop1) {
                        ...
                }
                result = 1;
                goto out_free_buffer;
        }
        ...
out_free_buffer:
        free(buffer);
        return result;
}
```

A common type of bug to be aware of is one err bugs which look like this:
```c
err:
        free(foo->bar);
        free(foo);
        return ret;
```
The bug in this code is that on some exit paths `foo` is `NULL`. Normally the fix for this is to split it up into two error labels `err_free_bar:` and `err_free_foo:`:
```c
err_free_bar:
       kfree(foo->bar);
err_free_foo:
       kfree(foo);
       return ret;
```

Ideally you should simulate errors to test all exit paths.

## 8) Commenting

Comments are good, but there is also a danger of over-commenting. NEVER try to explain HOW your code works in a comment: it's much better to write the code so that the working is obvious, and it's a waste of time to explain badly written code.

Generally, you want your comments to tell WHAT your code does, not HOW. Also, try to avoid putting comments inside a function body: if the function is so complex that you need to separately comment parts of it, you should probably go back to chapter 6 for a while. You can make small comments to note or warn about something particularly clever (or ugly), but try to avoid excess. Instead, put the comments at the head of the function, telling people what it does, and possibly WHY it does it.

The preferred style for long (multi-line) comments is:
```c
/*
 * This is the preferred style for multi-line
 * comments in the Linux kernel source code.
 * Please use it consistently.
 *
 * Description:  A column of asterisks on the left side,
 * with beginning and ending almost-blank lines.
 */
```
It's also important to comment data, whether they are basic types or derived types. To this end, use just one data declaration per line (no commas for multiple data declarations). This leaves you room for a small comment on each item, explaining its use.

## 9) You've made a mess of it

You can use `clang-format` to format correctly. We've provided a `.clang-format` which will clean up all of the formatting, but won't comment your code sadly.

## 10) Macros, Enums, and RTL

Names of macros defining constants and labels in enums are capitalized.
```c
#define CONSTANT 0x12345
```

Enums are preferred when defining several related constants.

CAPITALIZED macro names are appreciated but macros resembling functions may be named in lower case.

Generally, inline functions are preferable to macros resembling functions.

Macros with multiple statements should be enclosed in a do - while block:
```c
#define macrofun(a, b, c)                       \
        do {                                    \
                if (a == 5)                     \
                        do_this(b, c);          \
        } while (0)
```
Things to avoid when using macros:

1. macros that affect control flow:
```c
#define FOO(x)                                  \
        do {                                    \
                if (blah(x) < 0)                \
                        return -EBUGGERED;      \
        } while (0)
```
is a very bad idea. It looks like a function call but exits the calling function; don't break the internal parsers of those who will read the code.

2. macros that depend on having a local variable with a magic name:
```c
#define FOO(val) bar(index, val)
```
might look like a good thing, but it's confusing as hell when one reads the code and it's prone to breakage from seemingly innocent changes.

3. macros with arguments that are used as l-values: `FOO(x) = y;` will bite you if somebody e.g. turns FOO into an inline function.

4. forgetting about precedence: macros defining constants using expressions must enclose the expression in parentheses. Beware of similar issues with macros using parameters.
```c
#define CONSTANT 0x4000
#define CONSTEXP (CONSTANT | 3)
```
5) namespace collisions when defining local variables in macros resembling functions:
```c
#define FOO(x)                          \
({                                      \
        typeof(x) ret;                  \
        ret = calc_ret(x);              \
        (ret);                          \
})
```
`ret` is a common name for a local variable - `__foo_ret` is less likely to collide with an existing variable.

The cpp manual deals with macros exhaustively. The gcc internals manual also covers RTL which is used frequently with assembly language in the kernel.

## 11) Allocating Memory
The preferred form for passing a size of a struct is the following:
```c
p = malloc(sizeof(*p), ...);
```
The alternative form where struct name is spelled out hurts readability and introduces an opportunity for a bug when the pointer variable type is changed but the corresponding sizeof that is passed to a memory allocator is not.

Casting the return value which is a void pointer is redundant. The conversion from void pointer to any other pointer type is guaranteed by the C programming language.

The preferred form for allocating an array is the following:
```c
p = calloc(n, sizeof(...), ...);
```
This checks for overflow on the allocation size n * sizeof(...), and return NULL if that occurred.

## 12) The inline disease
There appears to be a common misperception that gcc has a magic "make me faster" speedup option called inline. While the use of inlines can be appropriate (for example as a means of replacing macros, see Chapter 10), it very often is not. Abundant use of the inline keyword leads to a much bigger kernel, which in turn slows the system as a whole down, due to a bigger icache footprint for the CPU and simply because there is less memory available for the pagecache. Just think about it; a pagecache miss causes a disk seek, which easily takes 5 milliseconds. There are a LOT of cpu cycles that can go into these 5 milliseconds.

A reasonable rule of thumb is to not put inline at functions that have more than 3 lines of code in them. An exception to this rule are the cases where a parameter is known to be a compiletime constant, and as a result of this constantness you know the compiler will be able to optimize most of your function away at compile time. For a good example of this later case, see the `malloc()` inline function.

Often people argue that adding inline to functions that are static and used only once is always a win since there is no space tradeoff. While this is technically correct, gcc is capable of inlining these automatically without help, and the maintenance issue of removing the inline when a second user appears outweighs the potential value of the hint that tells gcc to do something it would have done anyway.

## 16) Function return values and names
Functions can return values of many different kinds, and one of the most common is a value indicating whether the function succeeded or failed. Such a value can be represented as an error-code integer (-Exxx = failure, 0 = success) or a succeeded boolean (0 = failure, non-zero = success).

Mixing up these two sorts of representations is a fertile source of difficult-to-find bugs. If the C language included a strong distinction between integers and booleans then the compiler would find these mistakes for us... but it doesn't. To help prevent such bugs, always follow this convention:

```
If the name of a function is an action or an imperative command,
the function should return an error-code integer.  If the name
is a predicate, the function should return a "succeeded" boolean.
```
For example, add work is a command, and the add_work() function returns 0 for success or -EBUSY for failure. In the same way, PCI device present is a predicate, and the pci_dev_present() function returns 1 if it succeeds in finding a matching device or 0 if it doesn't.

All EXPORTed functions must respect this convention, and so should all public functions. Private (static) functions need not, but it is recommended that they do.

Functions whose return value is the actual result of a computation, rather than an indication of whether the computation succeeded, are not subject to this rule. Generally they indicate failure by returning some out-of-range result. Typical examples would be functions that return pointers; they use NULL or the ERR_PTR mechanism to report failure.

## 17) Inline assembly
In architecture-specific code, you may need to use inline assembly to interface with CPU or platform functionality. Don't hesitate to do so when necessary. However, don't use inline assembly gratuitously when C can do the job. You can and should poke hardware from C when possible.

Consider writing simple helper functions that wrap common bits of inline assembly, rather than repeatedly writing them with slight variations. Remember that inline assembly can use C parameters.

Large, non-trivial assembly functions should go in .S files, with corresponding C prototypes defined in C header files. The C prototypes for assembly functions should use asmlinkage.

You may need to mark your asm statement as volatile, to prevent GCC from removing it if GCC doesn't notice any side effects. You don't always need to do so, though, and doing so unnecessarily can limit optimization.

When writing a single inline assembly statement containing multiple instructions, put each instruction on a separate line in a separate quoted string, and end each string except the last with nt to properly indent the next instruction in the assembly output:
```c
asm ("magic %reg1, #42\n\t"
     "more_magic %reg2, %reg3"
     : /* outputs */ : /* inputs */ : /* clobbers */);
```

## 18) Conditional Compilation
Wherever possible, don't use preprocessor conditionals (#if, #ifdef) in .c files; doing so makes code harder to read and logic harder to follow. Instead, use such conditionals in a header file defining functions for use in those .c files, providing no-op stub versions in the #else case, and then call those functions unconditionally from .c files. The compiler will avoid generating any code for the stub calls, producing identical results, but the logic will remain easy to follow.

Prefer to compile out entire functions, rather than portions of functions or portions of expressions. Rather than putting an ifdef in an expression, factor out part or all of the expression into a separate helper function and apply the conditional to that function.

If you have a function or variable which may potentially go unused in a particular configuration, and the compiler would warn about its definition going unused, mark the definition as `__maybe_unused` rather than wrapping it in a preprocessor conditional. (However, if a function or variable always goes unused, delete it.)

At the end of any non-trivial #if or #ifdef block (more than a few lines), place a comment after the #endif on the same line, noting the conditional expression used. For instance:
```c
#ifdef CONFIG_SOMETHING
...
#endif /* CONFIG_SOMETHING */
```
