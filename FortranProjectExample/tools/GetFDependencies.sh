#!/bin/bash
File="$1"
Dir="$2"
shift 2
#Appends "$Dir/" at the beginning of words with suffix .o, .d, .mod and .smod.
./tools/FortranDep -d $File $@ | sed -E "s@(^|[[:space:]])([^[:space:]]+\.(o|d|mod|smod))@\1$Dir/\2@g"
