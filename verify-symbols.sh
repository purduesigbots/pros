#!/usr/bin/env bash

shopt -s globstar
# Convenience script to manually verify that all functions in the public headers exist in the source directory
vimdiff <(ctags --sort=yes --c-kinds=f --extra=-f+q --fields=S -f - src/**/* | awk '{if(index($0, "signature:")) print $1, substr($0, index($0, "signature:"))}' | sort ) <(ctags --sort=yes --c-kinds=p --extra=-f+q --fields=S -f - include/pros/**/* | awk '{if(index($0, "signature:")) print $1, substr($0, index($0, "signature:"))}' | sort)
