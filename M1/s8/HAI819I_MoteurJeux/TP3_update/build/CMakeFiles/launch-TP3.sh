#!/bin/sh
bindir=$(pwd)
cd /home/dorian/TP3_moteur_copy/TP3/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/dorian/TP3_moteur_copy/build/TP3 
	else
		"/home/dorian/TP3_moteur_copy/build/TP3"  
	fi
else
	"/home/dorian/TP3_moteur_copy/build/TP3"  
fi
