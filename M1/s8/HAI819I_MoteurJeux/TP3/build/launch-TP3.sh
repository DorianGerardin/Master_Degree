#!/bin/sh
bindir=$(pwd)
cd /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP3/TP3/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP3/build/TP3 
	else
		"/home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP3/build/TP3"  
	fi
else
	"/home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP3/build/TP3"  
fi
