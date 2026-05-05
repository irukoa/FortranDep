#!/bin/bash
# $1: source code.
# $2: object directory.
# Optional: can receive GCC's options for preprocessing.
if [ -z "$2" ]; then
	./tools/FortranDep $1
else
	#Appends "$2/" at the beginning of words with suffix .o, .d, .mod and .smod.
	./tools/FortranDep -pdl $1 | sed -E "s@(^|[[:space:]])([^[:space:]]+\.(o|d|mod|smod))@\1$2/\2@g"
fi
