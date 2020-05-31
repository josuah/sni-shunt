#!/bin/sh -e
# update content.mk with all source files that does not have a main()

exec >content.mk

find src -name '*.c' -exec echo src = {} + \
 | fold -w 70 -s \
 | sed '2,$ s/^/  /; s/$/ \\/; $ s/ \\//'

echo

find src -name '*.h' -exec echo inc = {} + \
 | fold -w 70 -s \
 | sed '2,$ s/^/  /; s/$/ \\/; $ s/ \\//'

echo

echo bin = *.c \
 | sed 's/\.c//g' \
 | fold -w 70 -s \
 | sed '2,$ s/^/  /; s/$/ \\/; $ s/ \\//'
