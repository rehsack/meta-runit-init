#!/bin/sh
### BEGIN INIT INFO
# Provides:          module-init-tools
# Required-Start:    
# Required-Stop:     
# Should-Start:      checkroot
# Should-stop:
# Default-Start:     S
# Default-Stop:
# Short-Description: Process /etc/modules.
# Description:       Load the modules listed in /etc/modules.
### END INIT INFO

if [ "$(basename "${0}")" = "run" ]; then
    [ -r /etc/init.d/functions-ri ] && . /etc/init.d/functions-ri
fi

domarkran() {
    if [ "$(basename "${0}")" = "run" ]; then
        markran
    fi
    exit 0
}

if [ "$(basename "${0}")" = "run" ]; then
    prereqok 1-checkroot || exit 1
fi

LOAD_MODULE=modprobe
[ -f /proc/modules ] || domarkran
[ -f /etc/modules ] || [ -d /etc/modules-load.d ] || domarkran
[ -e /sbin/modprobe ] || LOAD_MODULE=insmod

if [ ! -f /lib/modules/`uname -r`/modules.dep ]; then
	[ "$VERBOSE" != no ] && echo "Calculating module dependencies ..."
	depmod -Ae
fi

loaded_modules=" "

process_file() {
	file=$1

	(cat $file; echo; ) |
	while read module args
	do
		case "$module" in
			\#*|"") continue ;;
		esac
		[ -n "$(echo $loaded_modules | grep " $module ")" ] && continue
		[ "$VERBOSE" != no ] && echo -n "$module "
		eval "$LOAD_MODULE $module $args >/dev/null 2>&1"
		loaded_modules="${loaded_modules}${module} "
	done
}

[ "$VERBOSE" != no ] && echo -n "Loading modules: "
[ -f /etc/modules ] && process_file /etc/modules

[ -d /etc/modules-load.d ] || domarkran

for f in /etc/modules-load.d/*.conf; do
	process_file $f
done
[ "$VERBOSE" != no ] && echo

domarkran
