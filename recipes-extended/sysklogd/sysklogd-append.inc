FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/syslog.run \
		file://runit-init/klogd.run \
		"
RUNIT_SERVICES += "syslog klogd"
