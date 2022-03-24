#!/bin/sh
bindir=$(pwd)
cd /home/dorian/Master/M1/s8/HAI819I_MoteurJeux/TP3_update/TP3/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/dorian/Master/M1/s8/HAI819I_MoteurJeux/TP3_update/build/TP3 
	else
		"/home/dorian/Master/M1/s8/HAI819I_MoteurJeux/TP3_update/build/TP3"  
	fi
else
	"/home/dorian/Master/M1/s8/HAI819I_MoteurJeux/TP3_update/build/TP3"  
fi
