FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/banner.run \
		file://runit-init/bootmisc.run \
		file://runit-init/checkfs.run \
		file://runit-init/checkroot.run \
		file://runit-init/devpts.run \
		file://runit-init/dmesg.run \
		file://runit-init/hostname.run \
		file://runit-init/mountall.run \
		file://runit-init/mountnfs.run \
		file://runit-init/populate-volatile.run \
		file://runit-init/read-only-rootfs-hook.run \
		file://runit-init/rpcbind.run \
		file://runit-init/sysfs.run \
		file://runit-init/urandom.run \
		"

SRC_URI_append_arm = " file://runit-init/alignment.run"

RUNIT_SERVICES += " \
		banner.once \
		bootmisc.once.log \
		checkfs.once.log \
		checkroot.once.log \
		devpts.once \
		dmesg.once \
		hostname.once \
		mountall.once.log \
		mountnfs.once.log \
		populate-volatile.once.log \
		read-only-rootfs-hook.once.log \
		rpcbind.log \
		sysfs.once \
		urandom.once.log \
		"
RUNIT_SERVICES_append_arm += " alignment.once"
