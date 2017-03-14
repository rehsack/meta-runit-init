#!/bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/sbin:/usr/local/bin

. /etc/default/rcS

[ -z "$SUSHELL" ] && SUSHELL=/sbin/sushell
export SUSHELL

[ -z "$CONSOLE" ] && CONSOLE=/dev/console

exec <$CONSOLE >$CONSOLE 2>$CONSOLE

shiftok=1
sulogin_time=${SULOGIN_TIMEOUT}

while [ $shiftok -eq 1 ]; do
	p="$1"
	if [ "$p" = "-t" ]; then
		if shift; then
			sulogin_time="$1"
			break
		else
			shiftok=0
			break
		fi
	fi
	shift 2>/dev/null || shiftok=0
done

curtime=0

keypress=""
gotkey=0

readkey() {
	echo ""
	echo "Press one of the following keys followed by [ENTER]"
	echo "e - within $((sulogin_time - $curtime)) seconds for emergency root login"
	echo "q - to continue bootinging imediately"
	echo -n "[eq]: "
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
