#!/usr/bin/env bash

# when running this script, there should be nothing red or dark blue on the right panel (what's the in header files) with the following exceptions

# the pros::c::* functions. They're #ifdef __cplusplus'd out into a namespace. The C linkages demonsrtate that it's fine
# Default arguments exist in the constructor in the header file, but not in the implementation

shopt -s globstar
# Convenience script to manually verify that all functions in the public headers exist in the source directory
vimdiff <(ctags --sort=yes --c-kinds=f --extra=-f+q --fields=S -f - src/**/* | awk '{if(index($0, "signature:")) print $1, substr($0, index($0, "signature:"))}' | sort ) <(ctags --sort=yes --c-kinds=p --extra=-f+q --fields=S -f - include/pros/**/* | awk '{if(index($0, "signature:")) print $1, substr($0, index($0, "signature:"))}' | sort)
