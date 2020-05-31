#!/usr/bin/awk -f
# print the function and variable prototypes from a C source file

# functions

args {
	sub(/^[ \t]*/, " ")
	args = args $0
}

/^[a-zA-Z0-9_]+\([][)(a-z_A-Z0-9*,. \t]*$/ {
	if (match(type, "static") || match($0, ";$"))
		next

	symbol = $0
	sub(/\(.*/, "", symbol)
	sub(/[a-zA-Z0-9_]*\(/, "", $0)
	if (symbol == "main")
		next

	args = $0
	sub(/^[a-z]*\(/, "", args)
}

args && /\)$/ {
	gsub(/[\n \t]+/, " ", args)

	sub(/\)$/, "", args)

	printf("%s %s(%s);\n", type, symbol, args)

	args = ""
	next
}

!args {
	type = $0
}

# variables

/^[a-zA-Z][][ \t*a-z_A-Z0-9]*=.*[;{]$/ && $1 != "static" && $1 != "enum" {
	sub(/ *=.*/, ";")
	sub(/[ \t]*;$/, ";");
	print
}
