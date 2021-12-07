#!/bin/sh
bindir=$(pwd)
cd /home/e20180001518/Master/M1/s7/HAI714I_mode/TP_GMAP/src/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/e20180001518/Master/M1/s7/HAI714I_mode/TP_GMAP/build/gmap 
	else
		"/home/e20180001518/Master/M1/s7/HAI714I_mode/TP_GMAP/build/gmap"  
	fi
else
	"/home/e20180001518/Master/M1/s7/HAI714I_mode/TP_GMAP/build/gmap"  
fi
