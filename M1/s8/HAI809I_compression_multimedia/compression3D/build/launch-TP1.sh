#!/bin/sh
bindir=$(pwd)
cd /home/e20180001518/Master/M1/s8/HAI809I_compression_multimedia/compression3D/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/e20180001518/Master/M1/s8/HAI809I_compression_multimedia/compression3D/build/TP1 
	else
		"/home/e20180001518/Master/M1/s8/HAI809I_compression_multimedia/compression3D/build/TP1"  
	fi
else
	"/home/e20180001518/Master/M1/s8/HAI809I_compression_multimedia/compression3D/build/TP1"  
fi
