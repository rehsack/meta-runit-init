#!/bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

[ -x /etc/init.d/hwclock.sh ] && /etc/init.d/hwclock.sh stop 2>/dev/null

( [ -w /etc/timestamp ] || [ -w /etc ] ) && date -u +%4Y%2m%2d%2H%2M%2S >/etc/timestamp

if test -c /dev/urandom; then
	RANDOM_SEED_FILE=/var/lib/urandom/random-seed

	. /etc/default/rcS
	[ -f /etc/default/urandom ] && . /etc/default/urandom

	# Carry a random seed from shut-down to start-up;
	# see documentation in linux/drivers/char/random.c
	test "$VERBOSE" != no && echo "Saving random seed..."
	umask 077
	dd if=/dev/urandom of=$RANDOM_SEED_FILE count=1 \
		>/dev/null 2>&1 || echo "urandom stop: failed."
fi

# Write a reboot record to /var/log/wtmp before unmounting
halt -w

echo "Deactivating swap..."
[ -x /sbin/swapoff ] && swapoff -a

# We leave /proc mounted.
echo "Unmounting local filesystems..." >/dev/console
grep -q /mnt/ram /proc/mounts && mount -o remount,ro /mnt/ram
mount -o remount,ro /

umount -f -a -r > /dev/null 2>&1
