#!/bin/env bash

echo 'char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ " " __TIME__; char const * const _PROS_COMPILE_DIRECTORY = "$(pwd | tail -c 23)";' \
    | $1 -c -x c $2 -o $3 -