#!/bin/sh
bindir=$(pwd)
cd /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP2/TP2/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP2/build/TP2 
	else
		"/home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP2/build/TP2"  
	fi
else
	"/home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP2/build/TP2"  
fi
