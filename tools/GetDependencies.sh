#!/bin/bash
# $1: source code.
# $2: object directory.
if [ -z "$2" ]; then
	gcc -MM -MP -I. $1 | sed '1s|^\([^:]*\)\.o:|\1.d \1.o:|'
else
	#Appends "$2/" at the beginning.
	gcc -MM -MP -I. $1 | sed "1s|^|$2/|" | sed '1s|^\([^:]*\)\.o:|\1.d \1.o:|'
fi
