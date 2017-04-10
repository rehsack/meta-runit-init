FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/avahi-daemon.run \
		"
RUNIT_SERVICES += "avahi-daemon.log"
