#!/bin/sh
# update the function prototypes lists across the sources:

tmp=$(mktemp)
trap 'rm -rf "$tmp"' INT TERM EXIT

# for every "/** <filename-pattern> **/" found, update the following content 
# inline by scanning the <filename-pattern> for function declarations.

find src -type f | while read file; do
	awk '
		{ print }
		$1 == "/**" && $NF == "**/" {
			system("awk -f util/proto.awk "$2)
			while (getline skip) if (skip ~ /^$/) break
			print ""
		}
	' "$file" >"$tmp"
	mv "$tmp" "$file"
done
