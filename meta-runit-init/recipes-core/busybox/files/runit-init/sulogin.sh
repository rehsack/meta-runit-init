#!/bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/sbin:/usr/local/bin

. /etc/default/rcS

[ -x /sbin/sushell ] &&  [ -z "$SUSHELL" ] && SUSHELL=/bin/sh
export SUSHELL

shiftok=1
sulogin_time=${SULOGIN_TIMEOUT}

while [ $shiftok -eq 1 ]; do
	p="$1"
	if [ "$p" = "-t" ]; then
		if shift; then
			sulogin_time="$1"
			break
		else
			[ -n "$p" ] && CONSOLE="$p"
			shiftok=0
			break
		fi
	fi
	shift 2>/dev/null || shiftok=0
done

[ -z "$CONSOLE" ] && CONSOLE=/dev/console

curtime=0

keypress="l"
gotkey=0

readkey() {
	if [ -n "$keypress" ]; then
	    echo ""
	    echo "Press one of the following keys followed by [ENTER]"
	    echo "e - within $((sulogin_time - $curtime)) seconds for emergency root login"
	    echo "q - to continue booting imediately"
	    echo -n "[eq]: "
         fi
	read -t 1 keypress
}

readkey
curtime=$((curtime + 1))
while [ "$keypress" != "q" ] && [ "$keypress" != "e" ] && [ $curtime -lt $sulogin_time ]; do
	readkey
	curtime=$((curtime + 1))
done

if [ "$keypress" = "e" ]; then
	exec sulogin $CONSOLE
fi

exit 0
