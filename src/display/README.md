# Display

## Implementation of the TMEI (Touch Management Engine/Interface)

The TMEI is broken down into three layers:
1. Layer 0 (C-only): this layer is a very low-level interface that provides basic functions for displaying graphics on the integrated LCD touch display. Functions available include foreground/background color control functions, some shape drawing functions (pixel, line, rectangle, circle), and string display functions.
2. Layer 1 (C++): this layer builds on top of layer 0, providing a set of widgets (buttons, modal dialogs, etc.) as well as a context for interacting with the display.
3. Layer 2 (C++): this layer abstracts graphics control even further, facilitating the creation of full GUIs that are described in JSON files.