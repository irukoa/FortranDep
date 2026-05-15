#!/bin/bash
File="$1"
Dir="$2"
shift 2
#Appends "$Dir/" at the beginning.
gcc -MM -MP -I. $@ $File | sed "1s|^|$Dir/|" | sed '1s|^\([^:]*\)\.o:|\1.d \1.o:|'
