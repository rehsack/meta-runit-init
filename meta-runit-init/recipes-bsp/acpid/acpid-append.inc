FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/acpid.run \
		"
RUNIT_SERVICES += "acpid.log"
