FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/sshd.run \
		"
RUNIT_SERVICES += "sshd.log"
